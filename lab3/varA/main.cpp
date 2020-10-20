#include <iostream>
#include <iomanip>
#include "Hex.h"
#include "my_input.h"

const char* kMsgs[] = {
        "0. Exit",
        "1. Change number",
        "2. Add number (+)",
        "3. Subtract number (-)",
        "4. Print number",
        "5. Compare with number",
        "6. Check if even",
        "7. Bit shift right(digits)",
        "8. Bit shift left(digits)",
        "9. Clear digits"
};
constexpr int kN = sizeof(kMsgs) / sizeof(kMsgs[0]);


int main() {
    int menu_case;
    bool end = false;
    hexmath::Hex* sample = nullptr;
    for (auto & kMsg : kMsgs) {
        std::cout << kMsg << std::endl;
    }
    while (!end) {
        my_input::getNum(menu_case);
        switch(menu_case)
        {
            case 0:
            {
                end = true;
                break;
            }
            case 1 :
            {
                delete sample;
                sample = new hexmath::Hex();
                try {
                    hexmath::input(std::cin, *sample);
                }
                catch (std::out_of_range& msg) {
                    std::cout << msg.what() << std::endl;
                }
                catch (std::invalid_argument& msg) {
                    std::cout << msg.what() << std::endl;
                }
                catch (...) {
                    std::cout << "Something went wrong. Exit..." << std::endl;
                    exit(-1);
                }
                break;
            }
            case 2:
                if (sample != nullptr) {
                    hexmath::Hex y;
                    try {
                        hexmath::input(std::cin, y);
                        *sample = Add(*sample, y);
                        print(std::cout, *sample) << std::endl;
                    }
                    catch (std::out_of_range& msg) {
                        std::cout << msg.what() << std::endl;
                    }
                    catch (std::invalid_argument& msg) {
                        std::cout << msg.what() << std::endl;
                    }
                    catch (...) {
                        std::cout << "Something went wrong. Exit..." << std::endl;
                        exit(-1);
                    }
                }
                else {
                    std::cout << "Sample not found." << std::endl;
                }
                break;
            case 3:
                if (sample != nullptr) {
                    hexmath::Hex y;
                    try {
                        hexmath::input(std::cin, y);
                        *sample = Sub(*sample, y);
                        hexmath::print(std::cout, *sample) << std::endl;
                    }
                    catch (std::out_of_range& msg) {
                        std::cout << msg.what() << std::endl;
                    }
                    catch (std::invalid_argument& msg) {
                        std::cout << msg.what() << std::endl;
                    }
                    catch (...) {
                        std::cout << "Something went wrong. Exit..." << std::endl;
                        exit(-1);
                    }
                }
                else {
                    std::cout << "Sample not found." << std::endl;
                }
                break;
            case 4:
            {
                if (sample != nullptr) {
                    hexmath::print(std::cout, *sample) << std::endl;
                }
                else {
                    std::cout << "Sample not found." << std::endl;
                }
                break;
            }
            case 5:
            {
                if (sample != nullptr) {
                    hexmath::Hex y;
                    try {
                        hexmath::input(std::cin, y);
                        if (hexmath::isGreater(*sample, y)) {
                            std::cout << "sample > y" << std::endl;
                        }
                        else if (hexmath::isLess(*sample, y)) {
                            std::cout << "sample < y" << std::endl;
                        }
                        else {
                            std::cout << "sample == y" << std::endl;
                        }
                    }
                    catch (std::out_of_range& msg) {
                        std::cout << msg.what() << std::endl;
                    }
                    catch (std::invalid_argument& msg) {
                        std::cout << msg.what() << std::endl;
                    }
                    catch (...) {
                        std::cout << "Something went wrong. Exit..." << std::endl;
                        exit(-1);
                    }
                }
                else {
                    std::cout << "Sample not found." << std::endl;
                }
                break;
            }
            case 6:
            {
                if (sample != nullptr) {
                    bool res = sample->isEven();
                    if (res) {
                        std::cout << "Even" << std::endl;
                    }
                    else {
                        std::cout << "Odd" << std::endl;
                    }
                }
                else {
                    std::cout << "Sample not found." << std::endl;
                }
                break;
            }
            case 7:
            {
                if (sample != nullptr) {
                    int bias = 0;
                    my_input::getNum(bias);
                    try {
                        sample->BitShiftRight(bias);
                        hexmath::print(std::cout, *sample);
                    }
                    catch (std::invalid_argument& msg) {
                        std::cout << msg.what() << std::endl;
                    }
                    catch (...) {
                        std::cout << "Something went wrong. Exit..." << std::endl;
                        exit(-1);
                    }
                }
                else {
                    std::cout << "Sample not found." << std::endl;
                }
                break;
            }
            case 8:
            {
                if (sample != nullptr) {
                    int bias = 0;
                    my_input::getNum(bias);
                    try {
                        sample->BitShiftLeft(bias);
                        hexmath::print(std::cout, *sample);
                    }
                    catch (std::invalid_argument& msg) {
                        std::cout << msg.what() << std::endl;
                    }
                    catch (...) {
                        std::cout << "Something went wrong. Exit..." << std::endl;
                        exit(-1);
                    }
                }
                else {
                    std::cout << "Sample not found." << std::endl;
                }
                break;
            }
            case 9:
            {
                if (sample != nullptr) {
                    sample->setNull();
                }
                else {
                    std::cout << "Sample not found." << std::endl;
                }
                break;
            }
            default:
            {
                std::cout << "Bad operation code. Retry." << std::endl;
                break;
            }
        }
    }
    return 0;
}