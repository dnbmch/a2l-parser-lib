/*
 *  extract: a2lfile Block/LineItem -> protobuf contract.
 *  Copyright: Balint Kurucz - Danube Mechatronics Kft.
 *  License: Dual (GPL or Commercial) - see License.md
 */

#ifndef EXTRACT_H
#define EXTRACT_H

#include "a2lfile.h"
#include "a2l.pb.h"
#include "common.pb.h"
#include "axis.pb.h"
#include "record_layout.pb.h"
#include "xcp.pb.h"
#include "ccp.pb.h"

#include <string>
#include <map>
#include <functional>

namespace extract {

// ===== Top-level extraction entry point =====

a2l::v1::A2lFile extractFile(a2lfile::A2lFile* file);

// ===== Per-concept extraction (module.cpp) =====

a2l::v1::Module extractModule(a2lfile::Block* block);
a2l::v1::ModCommon extractModCommon(a2lfile::Block* block);
a2l::v1::ModPar extractModPar(a2lfile::Block* block);
a2l::v1::MemorySegment extractMemorySegment(a2lfile::Block* block);
a2l::v1::SystemConstant extractSystemConstant(a2lfile::Block* block);
a2l::v1::Unit extractUnit(a2lfile::Block* block);

// ===== Per-concept extraction (compu.cpp) =====

a2l::v1::CompuMethod extractCompuMethod(a2lfile::Block* block);
a2l::v1::CompuTab extractCompuTab(a2lfile::Block* block);
a2l::v1::CompuVtab extractCompuVtab(a2lfile::Block* block);
a2l::v1::CompuVtabRange extractCompuVtabRange(a2lfile::Block* block);

// ===== Per-concept extraction (record_layout.cpp) =====

a2l::v1::RecordLayout extractRecordLayout(a2lfile::Block* block);

// ===== Per-concept extraction (axis.cpp) =====

a2l::v1::Measurement extractMeasurement(a2lfile::Block* block);
a2l::v1::Characteristic extractCharacteristic(a2lfile::Block* block);
a2l::v1::AxisDescr extractAxisDescr(a2lfile::Block* block);
a2l::v1::AxisPts extractAxisPts(a2lfile::Block* block);

// ===== Per-concept extraction (ifdata.cpp) =====

a2l::v1::IfData extractIfData(a2lfile::Block* block);

// ===== Per-concept extraction (xcp.cpp) =====

a2l::v1::IfDataXcp extractIfDataXcp(a2lfile::Block* block);
a2l::v1::XcpProtocolLayer extractXcpProtocolLayer(a2lfile::Block* block);
a2l::v1::XcpDaq extractXcpDaq(a2lfile::Block* block);
a2l::v1::XcpDaqEventChannel extractXcpDaqEventChannel(a2lfile::Block* block);
a2l::v1::XcpDaqEvent extractXcpDaqEvent(a2lfile::Block* block);
a2l::v1::XcpTimestampSupported extractXcpTimestamp(a2lfile::Block* block);

// ===== Per-concept extraction (xcp_transport.cpp) =====

a2l::v1::XcpOnCan extractXcpOnCan(a2lfile::Block* block);
a2l::v1::XcpOnIp extractXcpOnIp(a2lfile::Block* block);
a2l::v1::XcpCanFd extractXcpCanFd(a2lfile::Block* block);
a2l::v1::XcpDaqListCanId extractXcpDaqListCanId(a2lfile::Block* block);
a2l::v1::XcpEventCanIdList extractXcpEventCanIdList(a2lfile::Block* block);

// ===== Per-concept extraction (xcp_segment.cpp) =====

a2l::v1::XcpSegment extractXcpSegment(a2lfile::Block* block);
a2l::v1::XcpPage extractXcpPage(a2lfile::Block* block);
a2l::v1::XcpChecksum extractXcpChecksum(a2lfile::Block* block);
a2l::v1::XcpAddressMapping extractXcpAddressMapping(a2lfile::Block* block);

// ===== Per-concept extraction (function_group.cpp) =====

a2l::v1::Function extractFunction(a2lfile::Block* block);
a2l::v1::Group extractGroup(a2lfile::Block* block);
a2l::v1::Annotation extractAnnotation(a2lfile::Block* block);
a2l::v1::TypedefCharacteristic extractTypedefCharacteristic(a2lfile::Block* block);
a2l::v1::TypedefStructure extractTypedefStructure(a2lfile::Block* block);
a2l::v1::StructureComponent extractStructureComponent(a2lfile::Block* block);
a2l::v1::TypedefAxis extractTypedefAxis(a2lfile::Block* block);
a2l::v1::Instance extractInstance(a2lfile::Block* block);

// ===== Per-concept extraction (variant_coding in extract_module.cpp) =====

a2l::v1::VariantCoding extractVariantCoding(a2lfile::Block* block);
a2l::v1::VarCharacteristic extractVarCharacteristic(a2lfile::Block* block);
a2l::v1::VarCriterion extractVarCriterion(a2lfile::Block* block);
a2l::v1::VarForbiddenComb extractVarForbiddenComb(a2lfile::Block* block);

// ===== Per-concept extraction (ccp.cpp) =====

a2l::v1::IfDataCcp extractIfDataCcp(a2lfile::Block* block);
a2l::v1::CcpTpBlob extractCcpTpBlob(a2lfile::Block* block);
a2l::v1::CcpSource extractCcpSource(a2lfile::Block* block);
a2l::v1::CcpQpBlob extractCcpQpBlob(a2lfile::Block* block);
a2l::v1::CcpRaster extractCcpRaster(a2lfile::Block* block);
a2l::v1::CcpEventGroup extractCcpEventGroup(a2lfile::Block* block);
a2l::v1::CcpDefinedPage extractCcpDefinedPage(a2lfile::Block* block);
a2l::v1::CcpKpBlob extractCcpKpBlob(a2lfile::Block* block);

// ===== Enum conversion helpers =====

a2l::v1::DataType dataTypeFromStr(const std::string& s);
a2l::v1::ByteOrder byteOrderFromStr(const std::string& s);
a2l::v1::IndexOrder indexOrderFromStr(const std::string& s);
a2l::v1::CharacteristicType characteristicTypeFromStr(const std::string& s);
a2l::v1::ConversionType conversionTypeFromStr(const std::string& s);
a2l::v1::DepositMode depositModeFromStr(const std::string& s);
a2l::v1::Monotony monotonyFromStr(const std::string& s);
a2l::v1::AxisAttribute axisAttributeFromStr(const std::string& s);
a2l::v1::UnitType unitTypeFromStr(const std::string& s);
a2l::v1::MemoryPrgType memoryPrgTypeFromStr(const std::string& s);
a2l::v1::MemoryType memoryTypeFromStr(const std::string& s);
a2l::v1::MemoryAttribute memoryAttributeFromStr(const std::string& s);
a2l::v1::MatrixLayout matrixLayoutFromStr(const std::string& s);
a2l::v1::CalibrationAccess calibrationAccessFromStr(const std::string& s);
a2l::v1::VarNaming varNamingFromStr(const std::string& s);

// XCP-specific enums
a2l::v1::AddressGranularity xcpAddressGranularityFromStr(const std::string& s);
a2l::v1::DaqConfigType xcpDaqConfigTypeFromStr(const std::string& s);
a2l::v1::DaqOptimisationType xcpDaqOptimisationTypeFromStr(const std::string& s);
a2l::v1::DaqAddressExtensionMode xcpDaqAddressExtModeFromStr(const std::string& s);
a2l::v1::IdentificationFieldType xcpIdentFieldTypeFromStr(const std::string& s);
a2l::v1::OverloadIndication xcpOverloadIndicationFromStr(const std::string& s);
a2l::v1::TimestampSize xcpTimestampSizeFromStr(const std::string& s);
a2l::v1::TimestampUnit xcpTimestampUnitFromStr(const std::string& s);
a2l::v1::ChecksumType xcpChecksumTypeFromStr(const std::string& s);
a2l::v1::SampleRate xcpSampleRateFromStr(const std::string& s);

// CCP-specific enums
a2l::v1::CcpDaqMode ccpDaqModeFromStr(const std::string& s);
a2l::v1::CcpConsistency ccpConsistencyFromStr(const std::string& s);
a2l::v1::CcpPageMemoryType ccpPageMemoryTypeFromStr(const std::string& s);
a2l::v1::CcpSyncEdge ccpSyncEdgeFromStr(const std::string& s);

// Helpers for collecting repeated keyword values
std::vector<uint32_t> collectRepeatedKeywordUnsigned(a2lfile::Block* block, const std::string& keyword);

// RecordLayout-specific enums
a2l::v1::RecordLayoutAxis recordLayoutAxisFromStr(const std::string& s);
a2l::v1::RecordLayoutAlignmentType recordLayoutAlignmentTypeFromStr(const std::string& s);
a2l::v1::RecordLayoutAddressType recordLayoutAddressTypeFromStr(const std::string& s);
a2l::v1::RecordLayoutIndexMode recordLayoutIndexModeFromStr(const std::string& s);

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

} // namespace extract

#endif // EXTRACT_H
