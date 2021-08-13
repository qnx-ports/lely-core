ARG REGISTRY

FROM ${REGISTRY}/build:bullseye

RUN apt-get update -q \
    && DEBIAN_FRONTEND=noninteractive apt-get install -qy --no-install-recommends \
        gcc-arm-none-eabi \
        libstdc++-arm-none-eabi-newlib \
        qemu-system-arm \
    && rm -rf /var/lib/apt/lists/*

ADD CppUTestConfig.h .

RUN git clone --branch v4.0 --depth 1 https://github.com/cpputest/cpputest.git \
    && mv CppUTestConfig.h cpputest/include/CppUTest \
    && cd cpputest/cpputest_build \
    && autoreconf .. -i \
    && ../configure \
       CFLAGS="-g -O2 -mcpu=cortex-m3 -D__NEWLIB__" \
       CXXFLAGS="-g -O2 -mcpu=cortex-m3 -D__NEWLIB__" \
       LDFLAGS="--specs=nosys.specs" \
       --host=arm-none-eabi \
       --disable-memory-leak-detection \
    && make \
    && make install \
    && cd - \
    && rm -rf cpputest
