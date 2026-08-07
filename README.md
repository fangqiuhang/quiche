# Abstract

A C++ implementation to support Google's [quiche](https://github.com/google/quiche.git)
by importing it as a git submodule without any revision, using GN+Ninja.

Furthermore, implement some benchmark and perf tools.

# Motivation

Inspired by Bilibili's work https://github.com/bilibili/quiche.

# Build and Run

## Linux

Install dependencies:

```
apt install generate-ninja ninja-build   # GN+Ninja
apt install clang llvm                   # If build with "is_clang = true"
```

Synchronize git submodules:
```
git submodule update --init --recursive
```

Then build via GN+Ninja:
```
gn gen out
ninja -C out epoll_quic_client
```

Finally run the simple QUIC client:
```
./out/epoll_quic_client https://www.iqiyi.com/ --quic_version=RFCv1 --disable_certificate_verification
```

## Mac

Install Xcode and Xcode CommandLine Tools, then you will get `gcc` or `clang`.

On M sillicon, if `gn` is little late, the `current_cpu` could be detected as
`x64`, causing mistake. Should specify it by `gn gen out/mac --args="current_cpu = \'arm64\'"`.

## Android(Termux)

Build in Termux. Firstly install Termux app in Android devices, then install
dependencies:
```
pkg install git clang gcc
```

# Reference

- Chromium (https://chromium.googlesource.com/chromium/src/)
- Envoy (https://github.com/envoyproxy/envoy)
- Bilibili quiche (https://github.com/bilibili/quiche)
