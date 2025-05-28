name: Build and Package

on: 
  push:
    branches: [ master ]
  pull_request:
    branches: [ master ]

jobs:
  linux-build:
    runs-on: ubuntu-latest
    
    steps:
    - name: Checkout code
      uses: actions/checkout@v4
      with:
        submodules: recursive
        
    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y \
          build-essential \
          cmake \
          dpkg-dev \
          rpm
        
    - name: Configure project
      run: cmake -B build -DCMAKE_BUILD_TYPE=Release
      
    - name: Build project
      run: cmake --build build --config Release --parallel $(nproc)
      
    - name: Run tests
      run: |
        cd build
        ctest --output-on-failure
        
    - name: Create Linux packages
      run: |
        cd build
        cpack -G TGZ
        cpack -G DEB
        cpack -G RPM
        
    - name: Upload Linux artifacts
      uses: actions/upload-artifact@v4
      with:
        name: linux-packages
        path: |
          build/*.tar.gz
          build/*.deb
          build/*.rpm
        retention-days: 5

  windows-build:
    runs-on: windows-latest
    
    steps:
    - name: Checkout code
      uses: actions/checkout@v4
      with:
        submodules: recursive
        
    - name: Install WiX Toolset (для MSI)
      run: choco install wixtoolset -y
      
    - name: Configure project
      run: cmake -B build -DCMAKE_BUILD_TYPE=Release
      
    - name: Build project
      run: cmake --build build --config Release --parallel 2
      
    - name: Create MSI package
      run: |
        cd build
        cpack -G WIX
        
    - name: Upload Windows artifacts
      uses: actions/upload-artifact@v4
      with:
        name: windows-packages
        path: build/*.msi
        retention-days: 5

  macos-build:
    runs-on: macos-latest
    
    steps:
    - name: Checkout code
      uses: actions/checkout@v4
      with:
        submodules: recursive
        
    - name: Configure project
      run: cmake -B build -DCMAKE_BUILD_TYPE=Release
      
    - name: Build project
      run: cmake --build build --config Release --parallel 2
      
    - name: Create DMG package
      run: |
        cd build
        cpack -G DragNDrop 
      
    - name: Upload macOS artifacts
      uses: actions/upload-artifact@v4
      with:
        name: macos-packages
        path: build/*.dmg
        retention-days: 5
