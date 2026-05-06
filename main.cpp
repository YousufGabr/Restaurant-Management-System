// main.cpp
#include "Restaurant/Restaurant.h"
#include <iostream>
#include <ctime>

int main() {
    
    Restaurant myRestaurant;

   
    srand(static_cast<unsigned>(time(0)));


    // ANSI escape codes for colors
    std::cout << "\033[33m"; // Yellow text
    std::cout << "*******************************************" << std::endl;
    std::cout << "*                                         *" << std::endl;
    std::cout << "*       WELCOME TO Our Restaurant         *" << std::endl;
    std::cout << "*                                         *" << std::endl;
    std::cout << "*******************************************" << std::endl;
    std::cout << "\033[0m"; // Reset to default color

    std::cout << "\n\033[32m  Serving delicious code since 2026!\033[0m\n" << std::endl;
    myRestaurant.RunSimulator();

    return 0;
}