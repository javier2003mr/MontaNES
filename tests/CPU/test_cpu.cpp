#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include "../../include/json.hpp"
#include "../../include/CPU.h" // Your provided header file

using json = nlohmann::json;

// Helper to format memory address for error messages
std::string to_hex(int w, int n) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::setw(w) << std::setfill('0') << n;
    return ss.str();
}

int main() {
    CPU cpu;
    cpu.initializeOpcodeTable(); // Initialize the CPU's internal tables

    // Loop through all possible opcode files (0x00 to 0xFF)
    for (int opcode = 0x00; opcode <= 0xFF; ++opcode) {

        // Construct filename: "../tests/XX.json"
        std::stringstream ss;
        ss << "tests/CPU/v1/" << std::hex << std::setw(2) << std::setfill('0') << opcode << ".json";
        std::string filename = ss.str();

        // Check if file exists before trying to open (skip missing opcodes)
        if (!std::filesystem::exists(filename)) {
            std::cout << "El fichero " << filename << " no existe" << std::endl;
            continue;
        }

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open " << filename << std::endl;
            continue;
        }

        std::cout << "Running tests from " << filename << "..." << std::endl;

        json test_suite;
        try {
            file >> test_suite;
        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error in " << filename << ": " << e.what() << std::endl;
            continue;
        }

        // Iterate through each test case in the file
        for (const auto& test : test_suite) {
            // 1. Initialize State
            auto initial = test["initial"];
            
            // Set Registers
            cpu.setPC(initial["pc"].get<unsigned short>());
            cpu.setSP(initial["s"].get<unsigned short>());
            cpu.setA(initial["a"].get<unsigned char>());
            cpu.setX(initial["x"].get<unsigned char>());
            cpu.setY(initial["y"].get<unsigned char>());
            cpu.setP(initial["p"].get<unsigned char>());

            // Set RAM
            // The JSON "ram" is an array of [address, value] pairs
            for (const auto& mem : initial["ram"]) {
                
                cpu.setMemoryDir(mem[0].get<unsigned short>(), mem[1].get<unsigned char>());
                
            }

            // 2. Execute Test
            // We run the CPU for the exact number of cycles specified in the test
            // This assumes emulationCycle() advances the CPU by one clock cycle/step
            int cycles_to_run = test["cycles"].size();
            int opcode_cycles;

            int i = 0;
            while (i < cycles_to_run){
                opcode_cycles = cpu.emulationCycle();
                i += opcode_cycles;
            }
            //for (int i = 0; i < cycles_to_run; ++i) {
            //    cpu.emulationCycle();
            //}

            // 3. Compare Results
            auto final_state = test["final"];
            bool passed = true;
            std::string error_msg;

            // Check Registers
            if (cpu.getPC() != final_state["pc"].get<unsigned short>()) {
                passed = false;
                error_msg += " PC mismatch (" + to_hex(4, cpu.getPC()) + " vs " + to_hex(4, final_state["pc"]) + ")";
            }
            // Note: Masking SP with 0xFF ensures we compare the 8-bit value if getSP returns full pointer
            if ((cpu.getSP() & 0xFF) != final_state["s"].get<unsigned short>()) {
                passed = false;
                error_msg += " SP mismatch";
            }
            if (cpu.getA() != final_state["a"].get<unsigned char>()) {
                passed = false;
                error_msg += " A mismatch";
            }
            if (cpu.getX() != final_state["x"].get<unsigned char>()) {
                passed = false;
                error_msg += " X mismatch";
            }
            if (cpu.getY() != final_state["y"].get<unsigned char>()) {
                passed = false;
                error_msg += " Y mismatch";
            }
            if (cpu.getP() != final_state["p"].get<unsigned char>()) {
                passed = false;
                error_msg += " P mismatch";
            }

            // Check RAM (Only verify memory locations specified in the final state)
            for (const auto& mem : final_state["ram"]) {
                unsigned short addr = mem[0].get<unsigned short>();
                unsigned char expected_val = mem[1].get<unsigned char>();
                unsigned char actual_val = cpu.getMemoryDir(addr);

                if (actual_val != expected_val) {
                    passed = false;
                    error_msg += " RAM mismatch at " + to_hex(4, addr) + 
                                 " (Act: " + to_hex(2, actual_val) + 
                                 " Exp: " + to_hex(2, expected_val) + ")";
                    break; // Stop checking RAM on first failure
                }
            }

            if (!passed) {
                std::cout << "FAILED: " << test["name"] << error_msg << std::endl;
                // return 1; // Uncomment to stop immediately on fail
            }
        }
    }

    std::cout << "All tests completed." << std::endl;
    return 0;
}