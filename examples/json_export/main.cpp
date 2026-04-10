/*
 *  a2lparser JSON export example.
 *  Loads an A2L file, extracts to protobuf, serializes to JSON.
 *  The JSON output can be consumed by Python, web UIs, databases, etc.
 *
 *  Build:
 *    cmake -B build
 *    cmake --build build
 *
 *  Run:
 *    ./build/a2l_json path/to/file.a2l              # JSON to stdout
 *    ./build/a2l_json path/to/file.a2l output.json   # JSON to file
 */

#include <fstream>
#include <iostream>
#include <string>

#include "a2l/a2lfile.h"
#include "a2l/extract.h"

#include <google/protobuf/util/json_util.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <file.a2l> [output.json]" << endl;
        return 1;
    }

    // 1. Parse the A2L file
    auto file = a2lfile::Loader::readA2lFile(argv[1]);
    if (!file) {
        cerr << "Failed to load: " << argv[1] << endl;
        return 1;
    }

    // 2. Extract into the protobuf contract
    a2l::A2lFile result = a2l::extract::extractFile(file.get());

    // 3. Configure JSON output (pretty-print with enum names)
    google::protobuf::util::JsonPrintOptions opts;
    opts.add_whitespace = true;

    // 4. Serialize to JSON
    string json;
    auto status = google::protobuf::util::MessageToJsonString(result, &json, opts);
    if (!status.ok()) {
        cerr << "JSON serialization failed: " << status.message() << endl;
        return 1;
    }

    // 5. Write to file or stdout
    if (argc >= 3) {
        ofstream out(argv[2]);
        if (!out.is_open()) {
            cerr << "Cannot open output file: " << argv[2] << endl;
            return 1;
        }
        out << json;
        out.close();
        cerr << "Wrote " << json.size() << " bytes to " << argv[2] << endl;
    } else {
        cout << json << endl;
    }

    return 0;
}
