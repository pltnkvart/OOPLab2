#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <limits>

static const char *enum_string[] = {"EMPTY", "PIECES", "BULK", "LIQUID", "GAS", "DANGEROUS"};

std::string convertToenum(int val);

template<class T>
T getNum(T min = std::numeric_limits<T>::lowest(), T max = std::numeric_limits<T>::max()) {
    T a;
    while (true) {
        std::cin >> a;
        if (std::cin.eof())
            throw std::runtime_error("Failed to read number: EOF");
        if (std::cin.bad())
            throw std::runtime_error("Error");
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "You are wrong";
        } else {
            if(a < min || a > max){
                std::cout << "Out of range";
            } else return a;
        }
        std::cout << ", repeat please: ";
    }
}

#endif //INPUT_H
