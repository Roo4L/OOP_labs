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

void ChangeSample(hexmath::Hex*& p);
void AddNum(hexmath::Hex* p);
void SubNum(hexmath::Hex* p);
void PrintNum(hexmath::Hex* p);
void CmpNum(hexmath::Hex* p);
void isEvenNum(hexmath::Hex* p);
void BitShiftRightNum(hexmath::Hex* p);
void BitShiftLeftNum(hexmath::Hex* p);
void ClearDigits(hexmath::Hex* p);

void (*fcase[])(hexmath::Hex* p) = {
    nullptr,
    nullptr,
    AddNum,
    SubNum,
    PrintNum,
    CmpNum,
    isEvenNum,
    BitShiftRightNum,
    BitShiftLeftNum,
    ClearDigits
};

int main() {
    int menu_case;
    bool end = false;
    hexmath::Hex* sample = nullptr;
    for (auto & kMsg : kMsgs) {
        std::cout << kMsg << std::endl;
    }
    while (!end) {
        my_input::getNum(menu_case);
        if (menu_case > 0 && menu_case < kN) {
            fcase[menu_case](sample);
        }
        else if (menu_case == 1) {
            ChangeSample(sample);
        }
        else if (menu_case == 0){
            end = true;
        }
        else {
            std::cout << "Bad operation code. Retry." << std::endl;
        }
    }
    return 0;
}

void ChangeSample(hexmath::Hex*& p) {
    delete p;
    p = new hexmath::Hex();
    try {
        std::cin >> (*p);
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
void AddNum(hexmath::Hex* p) {
    if (p != nullptr) {
        hexmath::Hex y;
        try {
            std::cin >> y;
            *p = *p + y;
            std::cout << *p << std::endl;
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
}
void SubNum(hexmath::Hex* p) {
    if (p != nullptr) {
        hexmath::Hex y;
        try {
            std::cin >> y;
            *p = *p - y;
            std::cout << *p << std::endl;
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
}
void PrintNum(hexmath::Hex* p) {
    if (p != nullptr) {
        std::cout << *p << std::endl;
    }
    else {
        std::cout << "Sample not found." << std::endl;
    }
}
void CmpNum(hexmath::Hex* p) {
    if (p != nullptr) {
        hexmath::Hex y;
        try {
            std::cin >> y;
            if (*p > y) {
                std::cout << "sample > y" << std::endl;
            }
            else if (*p < y) {
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
}
void isEvenNum(hexmath::Hex* p) {
    if (p != nullptr) {
        bool res = p->isEven();
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
}
void BitShiftRightNum(hexmath::Hex* p) {
    if (p != nullptr) {
        int bias = 0;
        my_input::getNum(bias);
        try {
            *p >>= bias;
            std::cout << *p << std::endl;
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
}
void BitShiftLeftNum(hexmath::Hex* p) {
    if (p != nullptr) {
        int bias = 0;
        my_input::getNum(bias);
        try {
            *p <<= bias;
            std::cout << *p;
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
}
void ClearDigits(hexmath::Hex* p) {
    if (p != nullptr) {
        p->setNull();
    }
    else {
        std::cout << "Sample not found." << std::endl;
    }
}