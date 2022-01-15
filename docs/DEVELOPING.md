# Directories

## Sub modules
- `net/third_party/quiche/src/` for [quiche](https://github.com/google/quiche.git)
- `third_party/boringssl/src/` for [boringssl](https://github.com/google/boringssl.git)

## Quiche implementations and dependencies

- `net/quic/platform/`
- `net/quiche/common/platform/impl/`
- `net/third_party/quiche/overrides/quiche_platform_impl/`
- `third_party/abseil-cpp/`
- `third_party/protobuf/`
- `tools/protoc_wrapper`

These directories and files are quiche's dependencies from chromium, whose
headers included by quiche.
In order to avoid revising quiche, keep their structures.

# Build

## Tools
- ninja
- gn
- gcc
- gcc-c++
- protoc
  - Download from: https://github.com/protocolbuffers/protobuf/releases
  - Just choose `protoc-{VERSION}-linux-x86_64.zip` for linux
  - Install: `unzip protoc-{VERSION}-linux-x86_64.zip && sudo mv bin/* /usr/local/bin/ && sudo mv include/* /usr/local/include/`

## Dependencies

- zlib-dev