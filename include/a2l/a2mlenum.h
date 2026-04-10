/*
 *  Copyright: Bálint Kurucz - Danube Mechatronics Kft.
 *
 *  Available under dual-license A) GPL | B) Commercial.
 *  Accept one, ignore the other.
 *  For GPL see License.md.
 *  For commercial, contact me:
 *  kb@danube-mechatronics.com
 *
 *  Keep this here if you choose A).
 */

#ifndef A2MLENUM_H
#define A2MLENUM_H

#include <map>
#include <stdint.h>
#include <string>

namespace a2lcfg
{

/* Address granularity - size of element at single address */
enum AddressGranurality
{
    ADDRESS_GRANULARITY_BYTE = 1,
    ADDRESS_GRANULARITY_WORD = 2,
    ADDRESS_GRANULARITY_DWORD = 4
};

/* Optional XCP commands supported by slave */
enum OptionalCommand
{
    NOT_AN_OPTIONAL_COMMAND,
    GET_COMM_MODE_INFO = 251,
    GET_ID = 250,
    SET_REQUEST = 249,
    GET_SEED = 248,
    UNLOCK = 247,
    SET_MTA = 246,
    UPLOAD = 245,
    SHORT_UPLOAD = 244,
    BUILD_CHECKSUM = 243,
    TRANSPORT_LAYER_CMD = 242,
    USER_CMD = 241,
    DOWNLOAD = 240,
    DOWNLOAD_NEXT = 239,
    DOWNLOAD_MAX = 238,
    SHORT_DOWNLOAD = 237,
    MODIFY_BITS = 236,
    SET_CAL_PAGE = 235,
    GET_CAL_PAGE = 234,
    GET_PAG_PROCESSOR_INFO = 233,
    GET_SEGMENT_INFO = 232,
    GET_PAGE_INFO = 231,
    SET_SEGMENT_MODE = 230,
    GET_SEGMENT_MODE = 229,
    COPY_CAL_PAGE = 228,
    CLEAR_DAQ_LIST = 227,
    SET_DAQ_PTR = 226,
    WRITE_DAQ = 225,
    SET_DAQ_LIST_MODE = 224,
    GET_DAQ_LIST_MODE = 223,
    START_STOP_DAQ_LIST = 222,
    START_STOP_SYNCH = 221,
    GET_DAQ_CLOCK = 220,
    READ_DAQ = 219,
    GET_DAQ_PROCESSOR_INFO = 218,
    GET_DAQ_RESOLUTION_INFO = 217,
    GET_DAQ_LIST_INFO = 216,
    GET_DAQ_EVENT_INFO = 215,
    FREE_DAQ = 214,
    ALLOC_DAQ = 213,
    ALLOC_ODT = 212,
    ALLOC_ODT_ENTRY = 211,
    PROGRAM_START = 210,
    PROGRAM_CLEAR = 209,
    PROGRAM = 208,
    PROGRAM_RESET = 207,
    GET_PGM_PROCESSOR_INFO = 206,
    GET_SECTOR_INFO = 205,
    PROGRAM_PREPARE = 204,
    PROGRAM_FORMAT = 203,
    PROGRAM_NEXT = 202,
    PROGRAM_MAX = 201,
    PROGRAM_VERIFY = 200
};

/* XCP_ON_CAN sample rate */
enum SampleRate
{
    SINGLE,
    TRIPLE
};

/* DAQ_LIST_CAN_ID length type */
enum DaqListLengthType
{
    VARIABLE,
    FIXED
};

/* XCP DAQ configuration type */
enum DaqType
{
    STATIC,
    DYNAMIC
};

/* DAQ optimization method preference */
enum OptimizationType
{
    OPTIMISATION_TYPE_DEFAULT = 0,
    OPTIMISATION_TYPE_ODT_TYPE_16 = 1,
    OPTIMISATION_TYPE_ODT_TYPE_32 = 2,
    OPTIMISATION_TYPE_ODT_TYPE_64 = 3,
    OPTIMISATION_TYPE_ODT_TYPE_ALIGNMENT = 4,
    OPTIMISATION_TYPE_MAX_ENTRY_SIZE = 5
};

/* Address extension scope */
enum AddressExtension
{
    ADDRESS_EXTENSION_FREE,
    ADDRESS_EXTENSION_ODT,
    ADDRESS_EXTENSION_DAQ
};

/* DAQ packet identification field type */
enum IdentificationFieldType
{
    IDENTIFICATION_FIELD_TYPE_ABSOLUTE = 0,
    IDENTIFICATION_FIELD_TYPE_RELATIVE_BYTE = 1,
    IDENTIFICATION_FIELD_TYPE_RELATIVE_WORD = 2,
    IDENTIFICATION_FIELD_TYPE_RELATIVE_WORD_ALIGNED = 3
};

/* ODT entry size granularity */
enum GranuralityOdtEntrySize
{
    GRANULARITY_ODT_ENTRY_SIZE_DAQ_BYTE = 1,
    GRANULARITY_ODT_ENTRY_SIZE_DAQ_WORD = 2,
    GRANULARITY_ODT_ENTRY_SIZE_DAQ_DWORD = 4,
    GRANULARITY_ODT_ENTRY_SIZE_DAQ_DLONG = 8
};

/* Overload indication method */
enum OverloadIndication
{
    NO_OVERLOAD_INDICATION,
    PID_OVERLOAD_INDICATION,
    EVENT_OVERLOAD_INDICATION
};

/* DAQ list type */
enum DaqListType
{
    DAQ,
    STIM,
    DAQ_STIM
};

/* Communication mode */
enum CommunicationModeSupported
{
    BLOCK,
    INTERLEAVED
};

/* Block communication mode roles */
enum CommunicationModeSupportedBlock
{
    SLAVE,
    MASTER
};

/* Timestamp size in bytes */
enum TimestampSize
{
    SIZE_BYTE = 1,
    SIZE_WORD = 2,
    SIZE_DWORD = 4
};

/* Timestamp ticks per unit */
enum TsTicksPerUnit
{
    UNIT_1NS,
    UNIT_10NS,
    UNIT_100NS,
    UNIT_1US,
    UNIT_10US,
    UNIT_100US,
    UNIT_1MS,
    UNIT_10MS,
    UNIT_100MS,
    UNIT_1S,
    UNIT_1PS,
    UNIT_10PS,
    UNIT_100PS
};

/* CAN sync edge */
enum SyncEdge
{
    SINGLE,
    DUAL
};

namespace MIXED
{

enum XcpOnXxx
{
    XCP_ON_TCP_IP,
    XCP_ON_UDP_IP,
    XCP_ON_CAN,
    XCP_ON_USB,
    XCP_ON_FLX,
    XCP_ON_SxI
};

} // namespace MIXED

namespace CCP_XCP_MIXED
{

enum CcpXcpSupported
{
    DAQ_ALTERNATING_SUPPORTED,
    TIMESTAMP_SUPPORTED,
    PID_OFF_SUPPORTED,
    PRESCALER_SUPPORTED,
    RESUME_SUPPORTED
};

} // namespace CCP_XCP_MIXED

namespace CONSIST
{

enum ConsistencyEvDaq
{
    DAQ,
    EVENT,
    ODT,
    NO_CONSISTENCY
};

} // namespace CONSIST

namespace ADDR_EXT
{

enum AddressExtensionType
{
    DAQ,
    ODT,
    FREE
};

} // namespace ADDR_EXT

/* GENERATED FUNCTION PROTOTYPES START */

/* AddressGranurality maps & function prototypes */
extern const std::map<std::string, a2lcfg::AddressGranurality> addressGranuralityStrEnuMap;
extern const std::map<a2lcfg::AddressGranurality, std::string> addressGranuralityEnuStrMap;
a2lcfg::AddressGranurality addressGranuralityFromString(std::string str);
std::string addressGranuralityToString(a2lcfg::AddressGranurality enumVal);

/* OptionalCommand maps & function prototypes */
extern const std::map<std::string, a2lcfg::OptionalCommand> optionalCommandStrEnuMap;
extern const std::map<a2lcfg::OptionalCommand, std::string> optionalCommandEnuStrMap;
a2lcfg::OptionalCommand optionalCommandFromString(std::string str);
std::string optionalCommandToString(a2lcfg::OptionalCommand enumVal);

/* SampleRate maps & function prototypes */
extern const std::map<std::string, a2lcfg::SampleRate> sampleRateStrEnuMap;
extern const std::map<a2lcfg::SampleRate, std::string> sampleRateEnuStrMap;
a2lcfg::SampleRate sampleRateFromString(std::string str);
std::string sampleRateToString(a2lcfg::SampleRate enumVal);

/* DaqListLengthType maps & function prototypes */
extern const std::map<std::string, a2lcfg::DaqListLengthType> daqListLengthTypeStrEnuMap;
extern const std::map<a2lcfg::DaqListLengthType, std::string> daqListLengthTypeEnuStrMap;
a2lcfg::DaqListLengthType daqListLengthTypeFromString(std::string str);
std::string daqListLengthTypeToString(a2lcfg::DaqListLengthType enumVal);

/* DaqType maps & function prototypes */
extern const std::map<std::string, a2lcfg::DaqType> daqTypeStrEnuMap;
extern const std::map<a2lcfg::DaqType, std::string> daqTypeEnuStrMap;
a2lcfg::DaqType daqTypeFromString(std::string str);
std::string daqTypeToString(a2lcfg::DaqType enumVal);

/* OptimizationType maps & function prototypes */
extern const std::map<std::string, a2lcfg::OptimizationType> optimizationTypeStrEnuMap;
extern const std::map<a2lcfg::OptimizationType, std::string> optimizationTypeEnuStrMap;
a2lcfg::OptimizationType optimizationTypeFromString(std::string str);
std::string optimizationTypeToString(a2lcfg::OptimizationType enumVal);

/* AddressExtension maps & function prototypes */
extern const std::map<std::string, a2lcfg::AddressExtension> addressExtensionStrEnuMap;
extern const std::map<a2lcfg::AddressExtension, std::string> addressExtensionEnuStrMap;
a2lcfg::AddressExtension addressExtensionFromString(std::string str);
std::string addressExtensionToString(a2lcfg::AddressExtension enumVal);

/* IdentificationFieldType maps & function prototypes */
extern const std::map<std::string, a2lcfg::IdentificationFieldType> identificationFieldTypeStrEnuMap;
extern const std::map<a2lcfg::IdentificationFieldType, std::string> identificationFieldTypeEnuStrMap;
a2lcfg::IdentificationFieldType identificationFieldTypeFromString(std::string str);
std::string identificationFieldTypeToString(a2lcfg::IdentificationFieldType enumVal);

/* GranuralityOdtEntrySize maps & function prototypes */
extern const std::map<std::string, a2lcfg::GranuralityOdtEntrySize> granuralityOdtEntrySizeStrEnuMap;
extern const std::map<a2lcfg::GranuralityOdtEntrySize, std::string> granuralityOdtEntrySizeEnuStrMap;
a2lcfg::GranuralityOdtEntrySize granuralityOdtEntrySizeFromString(std::string str);
std::string granuralityOdtEntrySizeToString(a2lcfg::GranuralityOdtEntrySize enumVal);

/* OverloadIndication maps & function prototypes */
extern const std::map<std::string, a2lcfg::OverloadIndication> overloadIndicationStrEnuMap;
extern const std::map<a2lcfg::OverloadIndication, std::string> overloadIndicationEnuStrMap;
a2lcfg::OverloadIndication overloadIndicationFromString(std::string str);
std::string overloadIndicationToString(a2lcfg::OverloadIndication enumVal);

/* DaqListType maps & function prototypes */
extern const std::map<std::string, a2lcfg::DaqListType> daqListTypeStrEnuMap;
extern const std::map<a2lcfg::DaqListType, std::string> daqListTypeEnuStrMap;
a2lcfg::DaqListType daqListTypeFromString(std::string str);
std::string daqListTypeToString(a2lcfg::DaqListType enumVal);

/* CommunicationModeSupported maps & function prototypes */
extern const std::map<std::string, a2lcfg::CommunicationModeSupported> communicationModeSupportedStrEnuMap;
extern const std::map<a2lcfg::CommunicationModeSupported, std::string> communicationModeSupportedEnuStrMap;
a2lcfg::CommunicationModeSupported communicationModeSupportedFromString(std::string str);
std::string communicationModeSupportedToString(a2lcfg::CommunicationModeSupported enumVal);

/* CommunicationModeSupportedBlock maps & function prototypes */
extern const std::map<std::string, a2lcfg::CommunicationModeSupportedBlock> communicationModeSupportedBlockStrEnuMap;
extern const std::map<a2lcfg::CommunicationModeSupportedBlock, std::string> communicationModeSupportedBlockEnuStrMap;
a2lcfg::CommunicationModeSupportedBlock communicationModeSupportedBlockFromString(std::string str);
std::string communicationModeSupportedBlockToString(a2lcfg::CommunicationModeSupportedBlock enumVal);

/* TimestampSize maps & function prototypes */
extern const std::map<std::string, a2lcfg::TimestampSize> timestampSizeStrEnuMap;
extern const std::map<a2lcfg::TimestampSize, std::string> timestampSizeEnuStrMap;
a2lcfg::TimestampSize timestampSizeFromString(std::string str);
std::string timestampSizeToString(a2lcfg::TimestampSize enumVal);

/* TsTicksPerUnit maps & function prototypes */
extern const std::map<std::string, a2lcfg::TsTicksPerUnit> tsTicksPerUnitStrEnuMap;
extern const std::map<a2lcfg::TsTicksPerUnit, std::string> tsTicksPerUnitEnuStrMap;
a2lcfg::TsTicksPerUnit tsTicksPerUnitFromString(std::string str);
std::string tsTicksPerUnitToString(a2lcfg::TsTicksPerUnit enumVal);

/* SyncEdge maps & function prototypes */
extern const std::map<std::string, a2lcfg::SyncEdge> syncEdgeStrEnuMap;
extern const std::map<a2lcfg::SyncEdge, std::string> syncEdgeEnuStrMap;
a2lcfg::SyncEdge syncEdgeFromString(std::string str);
std::string syncEdgeToString(a2lcfg::SyncEdge enumVal);

/* XcpOnXxx maps & function prototypes */
extern const std::map<std::string, a2lcfg::MIXED::XcpOnXxx> xcpOnXxxStrEnuMap;
extern const std::map<a2lcfg::MIXED::XcpOnXxx, std::string> xcpOnXxxEnuStrMap;
a2lcfg::MIXED::XcpOnXxx xcpOnXxxFromString(std::string str);
std::string xcpOnXxxToString(a2lcfg::MIXED::XcpOnXxx enumVal);

/* CcpXcpSupported maps & function prototypes */
extern const std::map<std::string, a2lcfg::CCP_XCP_MIXED::CcpXcpSupported> ccpXcpSupportedStrEnuMap;
extern const std::map<a2lcfg::CCP_XCP_MIXED::CcpXcpSupported, std::string> ccpXcpSupportedEnuStrMap;
a2lcfg::CCP_XCP_MIXED::CcpXcpSupported ccpXcpSupportedFromString(std::string str);
std::string ccpXcpSupportedToString(a2lcfg::CCP_XCP_MIXED::CcpXcpSupported enumVal);

/* ConsistencyEvDaq maps & function prototypes */
extern const std::map<std::string, a2lcfg::CONSIST::ConsistencyEvDaq> consistencyEvDaqStrEnuMap;
extern const std::map<a2lcfg::CONSIST::ConsistencyEvDaq, std::string> consistencyEvDaqEnuStrMap;
a2lcfg::CONSIST::ConsistencyEvDaq consistencyEvDaqFromString(std::string str);
std::string consistencyEvDaqToString(a2lcfg::CONSIST::ConsistencyEvDaq enumVal);

/* AddressExtensionType maps & function prototypes */
extern const std::map<std::string, a2lcfg::ADDR_EXT::AddressExtensionType> addressExtensionTypeStrEnuMap;
extern const std::map<a2lcfg::ADDR_EXT::AddressExtensionType, std::string> addressExtensionTypeEnuStrMap;
a2lcfg::ADDR_EXT::AddressExtensionType addressExtensionTypeFromString(std::string str);
std::string addressExtensionTypeToString(a2lcfg::ADDR_EXT::AddressExtensionType enumVal);

/* GENERATED FUNCTION PROTOTYPES END */

} // namespace a2lcfg

#endif // A2MLENUM_H
