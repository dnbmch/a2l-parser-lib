# a2l-parser-lib

History: I came into this slightly pre-damaged by a project where I had to deliver HEXplorer A2L parser extensions / fixes for years. That cost me a fair amount of nerves, so I eventually decided to dump that frustration somewhere useful and, long before the LLM era, built a fast, block-based, sort-of lexer-free A2L parser that is actually easy to use.

You can use it freely in any open-source project. If the platform your heart desires is missing, write to me and I’ll try to add it quickly. If you are considering integrating it into your product, avoid the frustration and use something that works plug and play. Feel free to contact me and we can work out a deal that fits. In that case, you would not be limited to a static library, you would get the parser source code as well.

Now, with the arrival of coding agents, I have extended the library and added a viewer to showcase what it can do. Try it.

Prebuilt static library, public headers, and protobuf schema for the **a2l-parser** — a C++17 library that parses ASAP2 (ASAM MCD-2MC) A2L files into Protocol Buffer messages.

**Author:** Danube Mechatronics Kft.

## Downloads

Prebuilt static libraries are available on the [Releases](https://github.com/dnbmch/a2l-parser-lib/releases) page:

| Artifact | Platform |
|----------|----------|
| `a2lparser-x86_64-windows-mingw` | Windows MinGW GCC (.a) |
| `a2lparser-x86_64-linux-gnu` | Linux x86_64 (.a) |
| `a2lparser-aarch64-linux-gnu` | Linux ARM64 (.a) |
| `a2lparser-x86_64-windows-msvc` | Windows MSVC (.lib) |
| `a2lparser-headers` | Public headers and proto files |

## Quick Start

```bash
# 1. Clone this repo
git clone https://github.com/dnbmch/a2l-parser-lib.git
cd a2l-parser-lib

# 2. Download and extract the prebuilt library for your platform
#    (from the Releases page, extract into lib/)
mkdir -p lib
tar xzf a2lparser-x86_64-linux-gnu-v0.4.0.tar.gz -C lib/

# 3. Build the examples
cmake -B build -DA2L_LIB_DIR=lib
cmake --build build

# 4. Run
./build/a2l_basic path/to/file.a2l
```

## Contents

| Directory | Description |
|-----------|-------------|
| `include/` | Public C++ headers (`a2l/a2lfile.h`, `a2l/extract.h`, enum headers) |
| `proto/` | Protobuf schema files (`.proto`) for multi-language binding generation |
| `examples/` | Example applications (basic summary, JSON export) |

## Integration

```cpp
#include "a2l/a2lfile.h"
#include "a2l/extract.h"

auto file = a2lfile::Loader::readA2lFile("path/to/file.a2l");
a2l::A2lFile result = a2l::extract::extractFile(file.get());

for (const auto& mod : result.modules()) {
    // Access measurements, characteristics, XCP/CCP config, etc.
}
```

## Build Requirements

- C++17 compiler (GCC, Clang, or MSVC)
- Protocol Buffers (protobuf) runtime library

## License

Dual licensed: GPL-2.0 or Commercial. See [LICENSE.md](LICENSE.md).
