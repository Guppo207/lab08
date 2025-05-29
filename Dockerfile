FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    lcov \
    g++ \
    libgtest-dev \
    libgmock-dev \
    && rm -rf /var/lib/apt/lists/*

COPY . /app
WORKDIR /app
RUN git submodule init && git submodule update

RUN mkdir -p build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON && \
    cmake --build . --config Release --parallel $(nproc)


RUN cd build && \
    ./RunTest --gtest_output=xml:test_results.xml


RUN cd build && \
    cpack -G TGZ && \
    cpack -G DEB && \
    cpack -G RPM


CMD ["bash"]
