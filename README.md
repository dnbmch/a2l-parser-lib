# a2l-parser-lib

Prebuilt static library, public headers, and protobuf schema for the **a2l-parser** — a C++17 library that parses ASAP2 (ASAM MCD-2MC) A2L files into Protocol Buffer messages.

**Author:** Danube Mechatronics Kft.

## Downloads

Prebuilt static libraries are available on the [Releases](https://github.com/dnbmch/a2l-parser-lib/releases) page:

| Artifact | Platform |
|----------|----------|
| `a2lextract-x86_64-windows-mingw` | Windows MinGW GCC (.a) |
| `a2lextract-x86_64-linux-gnu` | Linux x86_64 (.a) |
| `a2lextract-aarch64-linux-gnu` | Linux ARM64 (.a) |
| `a2lextract-headers` | Public headers and proto files |

## Quick Start

```bash
# 1. Clone this repo
git clone https://github.com/dnbmch/a2l-parser-lib.git
cd a2l-parser-lib

# 2. Download and extract the prebuilt library for your platform
#    (from the Releases page, extract into lib/)
mkdir -p lib
tar xzf a2lextract-x86_64-linux-gnu-v0.1.0.tar.gz -C lib/

# 3. Build the examples
cmake -B build -DA2L_LIB_DIR=lib
cmake --build build

# 4. Run
./build/a2l_basic path/to/file.a2l
```

## Contents

| Directory | Description |
|-----------|-------------|
| `include/` | Public C++ headers (`a2lfile.h`, `extract.h`, enum headers) |
| `proto/` | Protobuf schema files (`.proto`) for multi-language binding generation |
| `examples/` | Example applications (basic summary, JSON export) |

## Integration

```cpp
#include "a2lfile.h"
#include "extract.h"

auto file = a2lfile::Loader::readA2lFile("path/to/file.a2l");
a2l::v1::A2lFile result = extract::extractFile(file.get());

for (const auto& mod : result.modules()) {
    // Access measurements, characteristics, XCP/CCP config, etc.
}
```

## Build Requirements

- C++17 compiler (GCC, Clang, or MSVC)
- Protocol Buffers (protobuf) runtime library

## License

Dual licensed: GPL-2.0 or Commercial. See [LICENSE.md](LICENSE.md).
