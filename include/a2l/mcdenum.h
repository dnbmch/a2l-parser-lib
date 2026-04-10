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

#ifndef MCDENUM_H
#define MCDENUM_H

#include <map>
#include <stdint.h>
#include <string>

namespace a2lcfg
{

/* Data types for ECU variables */
enum DataType : uint8_t
{
    DATA_TYPE_UNKNOWN,
    UBYTE,
    SBYTE,
    UWORD,
    SWORD,
    ULONG,
    SLONG,
    A_UNIT64_T,
    A_INT64_T,
    FLOAT16_IEEE,
    FLOAT32_IEEE,
    FLOAT64_IEEE
};

enum DataSize
{
    BYTE,
    WORD,
    LONG
};

enum AddrType
{
    PBYTE,
    PWORD,
    PLONG,
    PLONGLONG,
    DIRECT
};

/* MSB_LAST = Intel (little endian), MSB_FIRST = Motorola (big endian) */
enum ByteOrder
{
    BYTE_ORDER_UNKNOWN,
    LITTLE_ENDIAN,
    BIG_ENDIAN,
    MSB_LAST,
    MSB_FIRST,
    MSB_FIRST_MSW_LAST,
    MSB_LAST_MSW_FIRST
};

enum IndexOrder
{
    INDEX_INCR,
    INDEX_DECR
};

enum Attribute
{
    CURVE_AXIS,
    COM_AXIS,
    FIX_AXIS,
    RES_AXIS,
    STD_AXIS
};

enum CalibrationAccess
{
    CALIBRATION,
    NO_CALIBRATION,
    NOT_IN_MCD_SYSTEM,
    OFFLINE_CALIBRATION
};

enum CharacteristicType
{
    ASCII,
    CURVE,
    MAP,
    CUBOID,
    CUBE_4,
    CUBE_5,
    VAL_BLK,
    VALUE
};

/* Conversion method types */
enum ConversionType : uint8_t
{
    CONV_TYPE_UNKNOWN,
    IDENTICAL,
    FORM,
    LINEAR,
    RAT_FUNC,
    TAB_INTP,
    TAB_NOINTP,
    TAB_VERB
};

enum CompuTabDefValueType
{
    DEFAULT_VALUE,  /* string used as OutVal when ECU value is out of range */
    DEFAULT_VALUE_NUMERIC
};

enum BitOperation
{
    LEFT_SHIFT,
    RIGHT_SHIFT,
    SIGN_EXTEND
};

enum DepositMode
{
    NOT_A_DEPOSIT_MODE,
    ABSOLUTE,
    DIFFERENCE
};

enum Encoding
{
    UTF8,
    UTF16,
    UTF32
};

/* Layout mode for 2D characteristic maps */
enum IndexMode
{
    ALTERNATE_CURVES,
    ALTERNATE_WITH_X,
    ALTERNATE_WITH_Y,
    COLUMN_DIR,
    ROW_DIR
};

/* Memory segment attributes */
enum MemoryAttribute
{
    INTERN,
    EXTERN
};

enum MemorySegmentType
{
    CALIBRATION_VARIABLES,
    CODE,
    DATA,
    EXCLUDE_FROM_FLASH,
    OFFLINE_DATA,
    RESERVED,
    SERAM,
    VARIABLES
};

enum PrgType
{
    PRG_CODE,
    PRG_DATA,
    PRG_RESERVED
};

/* Axis monotonicity */
enum Monotony
{
    MON_DECREASE,
    MON_INCREASE,
    MONOTONOUS,
    NOT_MON,
    STRICT_DECREASE,
    STRICT_INCREASE,
    STRICT_MON
};

/* MOD_COMMON alignment options */
enum ModCommonAlignment
{
    ALIGNMENT_BYTE,
    ALIGNMENT_WORD,
    ALIGNMENT_LONG,
    ALIGNMENT_INT64,
    ALIGNMENT_FLOAT16_IEEE,
    ALIGNMENT_FLOAT32_IEEE,
    ALIGNMENT_FLOAT64_IEEE
};

/* MOD_COMMON deposit mode (DAMOS standard deposit mode) */
enum DamosStdDepositMode
{
    DAMOS_SST,
    DAMOS_STD
};

/* Used block types in MODULE */
enum UsedBlockModule
{
    NOT_A_SUPPORTED_BLOCK,
    AXIS_PTS,
    CHARACTERISTIC,
    COMPU_METHOD,
    COMPU_TAB,
    COMPU_VTAB,
    COMPU_VTAB_RANGE,
    IF_DATA,
    MEASUREMENT,
    MOD_COMMON
};

/* GENERATED FUNCTION PROTOTYPES START */

/* DataType maps & function prototypes */
extern const std::map<std::string, a2lcfg::DataType> dataTypeStrEnuMap;
extern const std::map<a2lcfg::DataType, std::string> dataTypeEnuStrMap;
a2lcfg::DataType dataTypeFromString(std::string str);
std::string dataTypeToString(a2lcfg::DataType enumVal);

/* DataSize maps & function prototypes */
extern const std::map<std::string, a2lcfg::DataSize> dataSizeStrEnuMap;
extern const std::map<a2lcfg::DataSize, std::string> dataSizeEnuStrMap;
a2lcfg::DataSize dataSizeFromString(std::string str);
std::string dataSizeToString(a2lcfg::DataSize enumVal);

/* AddrType maps & function prototypes */
extern const std::map<std::string, a2lcfg::AddrType> addrTypeStrEnuMap;
extern const std::map<a2lcfg::AddrType, std::string> addrTypeEnuStrMap;
a2lcfg::AddrType addrTypeFromString(std::string str);
std::string addrTypeToString(a2lcfg::AddrType enumVal);

/* ByteOrder maps & function prototypes */
extern const std::map<std::string, a2lcfg::ByteOrder> byteOrderStrEnuMap;
extern const std::map<a2lcfg::ByteOrder, std::string> byteOrderEnuStrMap;
a2lcfg::ByteOrder byteOrderFromString(std::string str);
std::string byteOrderToString(a2lcfg::ByteOrder enumVal);

/* IndexOrder maps & function prototypes */
extern const std::map<std::string, a2lcfg::IndexOrder> indexOrderStrEnuMap;
extern const std::map<a2lcfg::IndexOrder, std::string> indexOrderEnuStrMap;
a2lcfg::IndexOrder indexOrderFromString(std::string str);
std::string indexOrderToString(a2lcfg::IndexOrder enumVal);

/* Attribute maps & function prototypes */
extern const std::map<std::string, a2lcfg::Attribute> attributeStrEnuMap;
extern const std::map<a2lcfg::Attribute, std::string> attributeEnuStrMap;
a2lcfg::Attribute attributeFromString(std::string str);
std::string attributeToString(a2lcfg::Attribute enumVal);

/* CalibrationAccess maps & function prototypes */
extern const std::map<std::string, a2lcfg::CalibrationAccess> calibrationAccessStrEnuMap;
extern const std::map<a2lcfg::CalibrationAccess, std::string> calibrationAccessEnuStrMap;
a2lcfg::CalibrationAccess calibrationAccessFromString(std::string str);
std::string calibrationAccessToString(a2lcfg::CalibrationAccess enumVal);

/* CharacteristicType maps & function prototypes */
extern const std::map<std::string, a2lcfg::CharacteristicType> characteristicTypeStrEnuMap;
extern const std::map<a2lcfg::CharacteristicType, std::string> characteristicTypeEnuStrMap;
a2lcfg::CharacteristicType characteristicTypeFromString(std::string str);
std::string characteristicTypeToString(a2lcfg::CharacteristicType enumVal);

/* ConversionType maps & function prototypes */
extern const std::map<std::string, a2lcfg::ConversionType> conversionTypeStrEnuMap;
extern const std::map<a2lcfg::ConversionType, std::string> conversionTypeEnuStrMap;
a2lcfg::ConversionType conversionTypeFromString(std::string str);
std::string conversionTypeToString(a2lcfg::ConversionType enumVal);

/* CompuTabDefValueType maps & function prototypes */
extern const std::map<std::string, a2lcfg::CompuTabDefValueType> compuTabDefValueTypeStrEnuMap;
extern const std::map<a2lcfg::CompuTabDefValueType, std::string> compuTabDefValueTypeEnuStrMap;
a2lcfg::CompuTabDefValueType compuTabDefValueTypeFromString(std::string str);
std::string compuTabDefValueTypeToString(a2lcfg::CompuTabDefValueType enumVal);

/* BitOperation maps & function prototypes */
extern const std::map<std::string, a2lcfg::BitOperation> bitOperationStrEnuMap;
extern const std::map<a2lcfg::BitOperation, std::string> bitOperationEnuStrMap;
a2lcfg::BitOperation bitOperationFromString(std::string str);
std::string bitOperationToString(a2lcfg::BitOperation enumVal);

/* DepositMode maps & function prototypes */
extern const std::map<std::string, a2lcfg::DepositMode> depositModeStrEnuMap;
extern const std::map<a2lcfg::DepositMode, std::string> depositModeEnuStrMap;
a2lcfg::DepositMode depositModeFromString(std::string str);
std::string depositModeToString(a2lcfg::DepositMode enumVal);

/* Encoding maps & function prototypes */
extern const std::map<std::string, a2lcfg::Encoding> encodingStrEnuMap;
extern const std::map<a2lcfg::Encoding, std::string> encodingEnuStrMap;
a2lcfg::Encoding encodingFromString(std::string str);
std::string encodingToString(a2lcfg::Encoding enumVal);

/* IndexMode maps & function prototypes */
extern const std::map<std::string, a2lcfg::IndexMode> indexModeStrEnuMap;
extern const std::map<a2lcfg::IndexMode, std::string> indexModeEnuStrMap;
a2lcfg::IndexMode indexModeFromString(std::string str);
std::string indexModeToString(a2lcfg::IndexMode enumVal);

/* MemoryAttribute maps & function prototypes */
extern const std::map<std::string, a2lcfg::MemoryAttribute> memoryAttributeStrEnuMap;
extern const std::map<a2lcfg::MemoryAttribute, std::string> memoryAttributeEnuStrMap;
a2lcfg::MemoryAttribute memoryAttributeFromString(std::string str);
std::string memoryAttributeToString(a2lcfg::MemoryAttribute enumVal);

/* MemorySegmentType maps & function prototypes */
extern const std::map<std::string, a2lcfg::MemorySegmentType> memorySegmentTypeStrEnuMap;
extern const std::map<a2lcfg::MemorySegmentType, std::string> memorySegmentTypeEnuStrMap;
a2lcfg::MemorySegmentType memorySegmentTypeFromString(std::string str);
std::string memorySegmentTypeToString(a2lcfg::MemorySegmentType enumVal);

/* PrgType maps & function prototypes */
extern const std::map<std::string, a2lcfg::PrgType> prgTypeStrEnuMap;
extern const std::map<a2lcfg::PrgType, std::string> prgTypeEnuStrMap;
a2lcfg::PrgType prgTypeFromString(std::string str);
std::string prgTypeToString(a2lcfg::PrgType enumVal);

/* Monotony maps & function prototypes */
extern const std::map<std::string, a2lcfg::Monotony> monotonyStrEnuMap;
extern const std::map<a2lcfg::Monotony, std::string> monotonyEnuStrMap;
a2lcfg::Monotony monotonyFromString(std::string str);
std::string monotonyToString(a2lcfg::Monotony enumVal);

/* ModCommonAlignment maps & function prototypes */
extern const std::map<std::string, a2lcfg::ModCommonAlignment> modCommonAlignmentStrEnuMap;
extern const std::map<a2lcfg::ModCommonAlignment, std::string> modCommonAlignmentEnuStrMap;
a2lcfg::ModCommonAlignment modCommonAlignmentFromString(std::string str);
std::string modCommonAlignmentToString(a2lcfg::ModCommonAlignment enumVal);

/* DamosStdDepositMode maps & function prototypes */
extern const std::map<std::string, a2lcfg::DamosStdDepositMode> damosStdDepositModeStrEnuMap;
extern const std::map<a2lcfg::DamosStdDepositMode, std::string> damosStdDepositModeEnuStrMap;
a2lcfg::DamosStdDepositMode damosStdDepositModeFromString(std::string str);
std::string damosStdDepositModeToString(a2lcfg::DamosStdDepositMode enumVal);

/* UsedBlockModule maps & function prototypes */
extern const std::map<std::string, a2lcfg::UsedBlockModule> usedBlockModuleStrEnuMap;
extern const std::map<a2lcfg::UsedBlockModule, std::string> usedBlockModuleEnuStrMap;
a2lcfg::UsedBlockModule usedBlockModuleFromString(std::string str);
std::string usedBlockModuleToString(a2lcfg::UsedBlockModule enumVal);

/* GENERATED FUNCTION PROTOTYPES END */

} // namespace a2lcfg

#endif // MCDENUM_H
