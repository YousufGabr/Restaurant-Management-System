// main.cpp
#include "Restaurant/Restaurant.h"
#include "UI/UI.h"
#include <iostream>
#include <ctime>

int main() {

    UI ui;
    ui.welcome_msg();

    srand(static_cast<unsigned>(time(0)));

    Restaurant myRestaurant;
    myRestaurant.RunSimulator();

    return 0;
}