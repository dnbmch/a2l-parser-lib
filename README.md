# a2l-parser-lib

Prebuilt static library, public headers, and protobuf schema for the **a2l-parser** — a C++17 library that parses ASAP2 (ASAM MCD-2MC) A2L files into Protocol Buffer messages.

**Author:** Danube Mechatronics Kft.

## Contents

| Directory | Description |
|-----------|-------------|
| `include/` | Public C++ headers (`a2lfile.h`, `extract.h`, enum headers) |
| `proto/` | Protobuf schema files (`.proto`) for multi-language binding generation |
| `examples/` | Example applications (basic summary, JSON export) |

## Quick Start

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
