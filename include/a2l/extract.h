#ifndef A2L_EXTRACT_H
#define A2L_EXTRACT_H

#include "a2l/a2lfile.h"
#include "a2l/a2l.pb.h"
#include "a2l/common.pb.h"
#include "a2l/axis.pb.h"
#include "a2l/record_layout.pb.h"
#include "a2l/xcp.pb.h"
#include "a2l/ccp.pb.h"

#include <string>
#include <map>
#include <functional>
#include <optional>

namespace a2l::extract {

// ===== Top-level extraction entry point =====

a2l::A2lFile extractFile(a2lfile::A2lFile* file);

// ===== Diagnostics accumulator =====
// Per-thread sink. Lossy/guessed extraction events are recorded here by the
// central numeric helpers and record-skip sites. extractFile() clears the sink
// on entry and drains it into A2lFile.diagnostics on return, so each call
// observes only its own events and concurrent extractions on different threads
// never interleave. Losslessly-preserved unknown blocks never emit.

void clearDiagnostics();
void recordDiagnostic(a2l::Severity severity, const std::string& location,
                      const std::string& message);
// Drains: returns the calling thread's accumulated diagnostics and empties the sink.
std::vector<a2l::Diagnostic> takeDiagnostics();

// ===== Per-concept extraction (module.cpp) =====

a2l::Module extractModule(a2lfile::Block* block);
a2l::ModCommon extractModCommon(a2lfile::Block* block);
a2l::ModPar extractModPar(a2lfile::Block* block);
a2l::MemorySegment extractMemorySegment(a2lfile::Block* block);
a2l::Unit extractUnit(a2lfile::Block* block);

// ===== Per-concept extraction (compu.cpp) =====

a2l::CompuMethod extractCompuMethod(a2lfile::Block* block);
a2l::CompuTab extractCompuTab(a2lfile::Block* block);
a2l::CompuVtab extractCompuVtab(a2lfile::Block* block);
a2l::CompuVtabRange extractCompuVtabRange(a2lfile::Block* block);

// ===== Per-concept extraction (record_layout.cpp) =====

a2l::RecordLayout extractRecordLayout(a2lfile::Block* block);

// ===== Per-concept extraction (axis.cpp) =====

a2l::Measurement extractMeasurement(a2lfile::Block* block);
a2l::Characteristic extractCharacteristic(a2lfile::Block* block);
a2l::AxisDescr extractAxisDescr(a2lfile::Block* block);
a2l::AxisPts extractAxisPts(a2lfile::Block* block);

// ===== Per-concept extraction (ifdata.cpp) =====

a2l::IfData extractIfData(a2lfile::Block* block);

// ===== Per-concept extraction (xcp.cpp) =====

a2l::IfDataXcp extractIfDataXcp(a2lfile::Block* block);
a2l::XcpProtocolLayer extractXcpProtocolLayer(a2lfile::Block* block);
a2l::XcpDaq extractXcpDaq(a2lfile::Block* block);
a2l::XcpDaqEventChannel extractXcpDaqEventChannel(a2lfile::Block* block);
a2l::XcpDaqEvent extractXcpDaqEvent(a2lfile::Block* block);
a2l::XcpTimestampSupported extractXcpTimestamp(a2lfile::Block* block);

// ===== Per-concept extraction (xcp_transport.cpp) =====

a2l::XcpOnCan extractXcpOnCan(a2lfile::Block* block);
a2l::XcpOnIp extractXcpOnIp(a2lfile::Block* block);
a2l::XcpCanFd extractXcpCanFd(a2lfile::Block* block);
a2l::XcpDaqListCanId extractXcpDaqListCanId(a2lfile::Block* block);
a2l::XcpEventCanIdList extractXcpEventCanIdList(a2lfile::Block* block);

// ===== Per-concept extraction (xcp_segment.cpp) =====

a2l::XcpSegment extractXcpSegment(a2lfile::Block* block);
a2l::XcpPage extractXcpPage(a2lfile::Block* block);
a2l::XcpChecksum extractXcpChecksum(a2lfile::Block* block);
a2l::XcpAddressMapping extractXcpAddressMapping(a2lfile::Block* block);

// ===== Per-concept extraction (function_group.cpp) =====

a2l::Function extractFunction(a2lfile::Block* block);
a2l::Group extractGroup(a2lfile::Block* block);
a2l::Annotation extractAnnotation(a2lfile::Block* block);
a2l::TypedefCharacteristic extractTypedefCharacteristic(a2lfile::Block* block);
a2l::TypedefStructure extractTypedefStructure(a2lfile::Block* block);
a2l::StructureComponent extractStructureComponent(a2lfile::Block* block);
a2l::TypedefAxis extractTypedefAxis(a2lfile::Block* block);
a2l::Instance extractInstance(a2lfile::Block* block);

// ===== Per-concept extraction (variant_coding in extract_module.cpp) =====

a2l::VariantCoding extractVariantCoding(a2lfile::Block* block);
a2l::VarCharacteristic extractVarCharacteristic(a2lfile::Block* block);
a2l::VarCriterion extractVarCriterion(a2lfile::Block* block);
a2l::VarForbiddenComb extractVarForbiddenComb(a2lfile::Block* block);

// ===== Per-concept extraction (ccp.cpp) =====

a2l::IfDataCcp extractIfDataCcp(a2lfile::Block* block);
a2l::CcpTpBlob extractCcpTpBlob(a2lfile::Block* block);
a2l::CcpSource extractCcpSource(a2lfile::Block* block);
a2l::CcpQpBlob extractCcpQpBlob(a2lfile::Block* block);
a2l::CcpRaster extractCcpRaster(a2lfile::Block* block);
a2l::CcpEventGroup extractCcpEventGroup(a2lfile::Block* block);
a2l::CcpDefinedPage extractCcpDefinedPage(a2lfile::Block* block);
a2l::CcpKpBlob extractCcpKpBlob(a2lfile::Block* block);

// ===== Enum conversion helpers =====

a2l::DataType dataTypeFromStr(const std::string& s);
a2l::ByteOrder byteOrderFromStr(const std::string& s);
a2l::IndexOrder indexOrderFromStr(const std::string& s);
a2l::CharacteristicType characteristicTypeFromStr(const std::string& s);
a2l::Encoding encodingFromStr(const std::string& s);
a2l::ConversionType conversionTypeFromStr(const std::string& s);
a2l::DepositMode depositModeFromStr(const std::string& s);
a2l::Monotony monotonyFromStr(const std::string& s);
a2l::AxisAttribute axisAttributeFromStr(const std::string& s);
a2l::UnitType unitTypeFromStr(const std::string& s);
a2l::MemoryPrgType memoryPrgTypeFromStr(const std::string& s);
a2l::MemoryType memoryTypeFromStr(const std::string& s);
a2l::MemoryAttribute memoryAttributeFromStr(const std::string& s);
a2l::MatrixLayout matrixLayoutFromStr(const std::string& s);
a2l::CalibrationAccess calibrationAccessFromStr(const std::string& s);
a2l::VarNaming varNamingFromStr(const std::string& s);

// XCP-specific enums
a2l::AddressGranularity xcpAddressGranularityFromStr(const std::string& s);
a2l::DaqConfigType xcpDaqConfigTypeFromStr(const std::string& s);
a2l::DaqOptimisationType xcpDaqOptimisationTypeFromStr(const std::string& s);
a2l::DaqAddressExtensionMode xcpDaqAddressExtModeFromStr(const std::string& s);
a2l::IdentificationFieldType xcpIdentFieldTypeFromStr(const std::string& s);
a2l::OverloadIndication xcpOverloadIndicationFromStr(const std::string& s);
a2l::TimestampSize xcpTimestampSizeFromStr(const std::string& s);
a2l::TimestampUnit xcpTimestampUnitFromStr(const std::string& s);
a2l::ChecksumType xcpChecksumTypeFromStr(const std::string& s);
a2l::SampleRate xcpSampleRateFromStr(const std::string& s);
a2l::DaqListType xcpDaqListTypeFromStr(const std::string& s);
a2l::DaqConsistency xcpDaqConsistencyFromStr(const std::string& s);
a2l::PacketAlignment xcpPacketAlignmentFromStr(const std::string& s);

// CCP-specific enums
a2l::CcpDaqMode ccpDaqModeFromStr(const std::string& s);
a2l::CcpConsistency ccpConsistencyFromStr(const std::string& s);
a2l::CcpPageMemoryType ccpPageMemoryTypeFromStr(const std::string& s);
a2l::CcpSyncEdge ccpSyncEdgeFromStr(const std::string& s);

// CCP 2.1 standard command code for a symbolic name (CONNECT=0x01 ... DNLOAD_6=0x23);
// 0 for an unknown name.
uint32_t ccpCommandCodeFromStr(const std::string& s);

// Helpers for collecting repeated keyword values (purely-numeric operands:
// FIXED / RASTER / EVENT).
std::vector<uint32_t> collectRepeatedKeywordUnsigned(a2lfile::Block* block, const std::string& keyword);

// OPTIONAL_CMD / OPTIONAL_LEVEL1_CMD command lists. A numeric operand is kept
// verbatim; a symbolic name is resolved against the XCP (resp. CCP) standard
// command table, and an unknown symbol is DROPPED with a diagnostic rather than
// silently pushed as 0.
std::vector<uint32_t> collectXcpCommands(a2lfile::Block* block, const std::string& keyword);
std::vector<uint32_t> collectCcpCommands(a2lfile::Block* block, const std::string& keyword);

// OPTIONAL_TL_SUBCMD maps to transport-specific symbolic code sets (CAN vs
// Ethernet); the calling transport extractor selects which. Unknown symbols are
// dropped with a DROPPED diagnostic rather than silently.
enum class XcpTransport { Can, Ethernet };
std::vector<uint32_t> collectTlSubcmds(a2lfile::Block* block, XcpTransport transport);

// RecordLayout-specific enums
a2l::RecordLayoutAxis recordLayoutAxisFromStr(const std::string& s);
a2l::RecordLayoutAlignmentType recordLayoutAlignmentTypeFromStr(const std::string& s);
a2l::RecordLayoutAddressType recordLayoutAddressTypeFromStr(const std::string& s);
a2l::RecordLayoutIndexMode recordLayoutIndexModeFromStr(const std::string& s);
a2l::RecordLayoutDataSize recordLayoutDataSizeFromStr(const std::string& s);

// ===== Block-reading helpers =====

// LineItem-level numeric primitives. Items come from the parse tree, whose
// accessors return the static Invalid sentinel for absent tokens — never null.
// An Invalid token yields a silent 0;
// otherwise a central lossy-check (non-numeric token, sto* failure, or a
// fractional float read into an integer field) records a WARNING, then the
// value is converted. `location` is the block name; keyword operands append
// " <keyword>". Every numeric read in the extractors funnels through these.
uint64_t liUnsigned(a2lfile::LineItem* li, const std::string& location);
double   liDouble(a2lfile::LineItem* li, const std::string& location);
int64_t  liSigned(a2lfile::LineItem* li, const std::string& location);

// Get the text of a LineItem at index, or empty string if out of range.
std::string textAt(a2lfile::Block* block, uint64_t idx);

// Get unsigned value at index, or 0 if out of range.
uint64_t unsignedAt(a2lfile::Block* block, uint64_t idx);

// Get double value at index, or 0.0 if out of range.
double doubleAt(a2lfile::Block* block, uint64_t idx);

// Get signed value at index, or 0 if out of range.
int64_t signedAt(a2lfile::Block* block, uint64_t idx);

// Find an optional keyword and return the next LineItem's text, or empty.
std::string optKeywordText(a2lfile::Block* block, const std::string& keyword);

// Find an optional keyword and return the next LineItem's unsigned value, or nullopt.
std::optional<uint64_t> optKeywordUnsigned(a2lfile::Block* block, const std::string& keyword);

// Find an optional keyword and return the next LineItem's double value, or nullopt.
std::optional<double> optKeywordDouble(a2lfile::Block* block, const std::string& keyword);

// Check if a keyword identifier is present (for boolean-flag keywords like GUARD_RAILS).
bool hasKeyword(a2lfile::Block* block, const std::string& keyword);

// Collect all raw text content of a block into a single string (for raw fallback
// into a named field, where the schema already carries the block identity).
std::string blockToRawString(a2lfile::Block* block);

// Same content wrapped in the block's own "/begin NAME ... /end NAME" — a
// complete, self-identifying, re-parseable fragment. Used for the catch-all
// raw_blocks/raw_extensions collectors, where the entry itself is the only
// record of which block was preserved.
std::string blockToRawNamed(a2lfile::Block* block);

} // namespace a2l::extract

#endif // A2L_EXTRACT_H
