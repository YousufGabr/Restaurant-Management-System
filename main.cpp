// main.cpp
#include "Restaurant/Restaurant.h"
#include <iostream>
#include <ctime>

int main() {
    // 1. Perform any needed initializations [cite: 360]
    Restaurant myRestaurant;

    // Seed the random number generator once in main
    srand(static_cast<unsigned>(time(0)));

    std::cout << "Starting Phase 1.2 Random Simulation..." << std::endl;

    // 2. Run the simulation
    myRestaurant.RunPhase1Simulator();

    std::cout << "Simulation Ended." << std::endl;
    system("pause");
    return 0;
}