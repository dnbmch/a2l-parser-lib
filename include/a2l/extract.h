/*
 *  extract: a2lfile Block/LineItem -> protobuf contract.
 *  Copyright: Balint Kurucz - Danube Mechatronics Kft.
 *  License: Dual (GPL or Commercial) - see License.md
 */

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

// ===== Per-concept extraction (module.cpp) =====

a2l::Module extractModule(a2lfile::Block* block);
a2l::ModCommon extractModCommon(a2lfile::Block* block);
a2l::ModPar extractModPar(a2lfile::Block* block);
a2l::MemorySegment extractMemorySegment(a2lfile::Block* block);
a2l::SystemConstant extractSystemConstant(a2lfile::Block* block);
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

// CCP-specific enums
a2l::CcpDaqMode ccpDaqModeFromStr(const std::string& s);
a2l::CcpConsistency ccpConsistencyFromStr(const std::string& s);
a2l::CcpPageMemoryType ccpPageMemoryTypeFromStr(const std::string& s);
a2l::CcpSyncEdge ccpSyncEdgeFromStr(const std::string& s);

// Helpers for collecting repeated keyword values
std::vector<uint32_t> collectRepeatedKeywordUnsigned(a2lfile::Block* block, const std::string& keyword);

// RecordLayout-specific enums
a2l::RecordLayoutAxis recordLayoutAxisFromStr(const std::string& s);
a2l::RecordLayoutAlignmentType recordLayoutAlignmentTypeFromStr(const std::string& s);
a2l::RecordLayoutAddressType recordLayoutAddressTypeFromStr(const std::string& s);
a2l::RecordLayoutIndexMode recordLayoutIndexModeFromStr(const std::string& s);

// ===== Block-reading helpers =====

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

// Collect all raw text content of a block into a single string (for raw fallback).
std::string blockToRawString(a2lfile::Block* block);

} // namespace a2l::extract

#endif // A2L_EXTRACT_H
