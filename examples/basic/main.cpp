/*
 *  a2lparser basic example.
 *  Loads an A2L file, extracts to protobuf, prints a human-readable summary.
 *
 *  Build:
 *    cmake -B build
 *    cmake --build build
 *
 *  Run:
 *    ./build/a2l_basic path/to/file.a2l
 */

#include <iostream>
#include <string>

#include "a2lfile.h"
#include "extract.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <file.a2l>" << endl;
        return 1;
    }

    // 1. Parse the A2L file into a block tree
    auto file = a2lfile::Loader::readA2lFile(argv[1]);
    if (!file) {
        cerr << "Failed to load: " << argv[1] << endl;
        return 1;
    }

    // 2. Extract into the protobuf contract
    a2l::v1::A2lFile result = extract::extractFile(file.get());

    // 3. Walk the extracted data
    for (const auto& mod : result.modules()) {
        cout << "Module: " << mod.name() << endl;
        cout << "  Measurements:    " << mod.measurements_size() << endl;
        cout << "  Characteristics: " << mod.characteristics_size() << endl;

        // Print measurements with address and data type
        for (const auto& m : mod.measurements()) {
            cout << "  MEASUREMENT " << m.name()
                 << "  type=" << a2l::v1::DataType_Name(m.datatype())
                 << "  conv=" << m.conversion();
            if (m.has_ecu_address())
                cout << "  addr=0x" << hex << m.ecu_address() << dec;
            cout << "  [" << m.lower_limit() << " .. " << m.upper_limit() << "]";
            cout << endl;
        }

        // Print XCP connection parameters if present
        for (const auto& ifd : mod.if_datas()) {
            if (!ifd.has_xcp()) continue;
            const auto& xcp = ifd.xcp();

            if (xcp.has_protocol_layer()) {
                const auto& pl = xcp.protocol_layer();
                cout << "  XCP Protocol: max_cto=" << pl.max_cto()
                     << " max_dto=" << pl.max_dto()
                     << " cmds=" << pl.optional_cmds_size() << endl;
            }

            for (const auto& can : xcp.xcp_on_cans()) {
                cout << "  XCP CAN:";
                if (can.has_can_id_master())
                    cout << " master=0x" << hex << can.can_id_master() << dec;
                if (can.has_can_id_slave())
                    cout << " slave=0x" << hex << can.can_id_slave() << dec;
                if (can.has_baudrate())
                    cout << " baud=" << can.baudrate();
                cout << endl;
            }
        }

        // Print CCP connection parameters if present
        for (const auto& ifd : mod.if_datas()) {
            if (!ifd.has_ccp()) continue;
            const auto& ccp = ifd.ccp();

            if (ccp.has_tp_blob()) {
                const auto& tp = ccp.tp_blob();
                cout << "  CCP: station=0x" << hex << tp.station_address() << dec
                     << " crm=0x" << hex << tp.crm_can_id() << dec
                     << " dto=0x" << hex << tp.dto_can_id() << dec;
                if (tp.has_baudrate())
                    cout << " baud=" << tp.baudrate();
                cout << endl;
            }
        }
    }

    // 4. Serialize to binary (ready for storage or transmission)
    string binary;
    result.SerializeToString(&binary);
    cout << "Serialized: " << binary.size() << " bytes" << endl;

    return 0;
}
