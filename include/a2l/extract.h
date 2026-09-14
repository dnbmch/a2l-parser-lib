#ifndef A2L_EXTRACT_H
#define A2L_EXTRACT_H

#include "a2l/a2l.pb.h"

namespace a2lfile {
struct A2lFile;
}

namespace a2l::extract {

// Build a typed document from the caller-owned raw parse tree.
a2l::A2lFile extractFile(a2lfile::A2lFile* file);

} // namespace a2l::extract

#endif // A2L_EXTRACT_H
