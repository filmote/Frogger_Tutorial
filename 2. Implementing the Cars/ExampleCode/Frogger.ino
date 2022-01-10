#include <Arduboy2.h>
#include "Images.h"

#define NUMBER_OF_CARS 6

Arduboy2 arduboy;

enum ObstacleType {
    Car,
    Truck,
    Van,
};

struct Obstacle {

    int16_t x;
    uint8_t y;
    ObstacleType type;           

};

Obstacle cars[NUMBER_OF_CARS];

void setup() {
  
    arduboy.begin();
    arduboy.setFrameRate(45);
    arduboy.initRandomSeed();

    resetGame();

}   

void loop() {

    if ( !arduboy.nextFrame() ) return;
    arduboy.clear();

    drawScreen();
    arduboy.display();

}

void resetGame() {

    cars[0] = { 130, 32, ObstacleType::Car };
    cars[1] = { 180, 32, ObstacleType::Truck };
    cars[2] = { 64, 41, ObstacleType::Truck };

}

void drawScreen() {

    // Draw road, verge, etc ..

    arduboy.drawLine(0, 26, 127, 26);
    arduboy.drawLine(0, 30, 127, 30);
    arduboy.drawLine(0, 57, 127, 57);


    // Draw road lines ..

    for (uint8_t x = 8; x < 128; x = x + 12) {

        arduboy.drawLine(x, 39, x + 4, 39);
        arduboy.drawLine(x, 48, x + 4, 48);

    }

    
    // Draw cars ..

    for (uint8_t i = 0; i < NUMBER_OF_CARS; i++) {
        drawCar(cars[i]);
    }

}

void drawCar(Obstacle car) {

    if (car.type == ObstacleType::Car) {

        Sprites::drawSelfMasked(car.x, car.y, Car_Small, 0);

    }

}

void moveCars(Obstacle &car, int8_t increment) {

    car.x = car.x + increment;
    if (car.x <- 20) { car.x = car.x + 148; }

}
