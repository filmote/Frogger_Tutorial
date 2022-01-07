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
    arduboy.pollButtons();


    // Update the cars positions ..

    if (arduboy.frameCount % 2 == 0) {

        moveCars(cars[0], cars[1], -1);
        moveCars(cars[4], cars[5], -1);

    }
    else {

        moveCars(cars[0], cars[1], -2);

    }

    moveCars(cars[2], cars[3], -1);
    drawScreen();

    arduboy.display();

}

void resetGame() {

    cars[0] = { 130, 32, ObstacleType::Car };
    cars[1] = { 180, 32, ObstacleType::Truck };
    cars[2] = { 64, 41, ObstacleType::Truck };
    cars[3] = { 110, 41, ObstacleType::Van };
    cars[4] = { 45, 50, ObstacleType::Car };
    cars[5] = { 70, 50, ObstacleType::Van };

}

void drawScreen() {


    // Draw road, verge, etc ..
  
    arduboy.drawLine(0, 26, 127, 26);
    arduboy.drawLine(0, 30, 127, 30);
    arduboy.drawLine(0, 57, 127, 57);

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

    if (car.type == ObstacleType::Truck) {

        Sprites::drawSelfMasked(car.x, car.y, Car_Truck, 0);

    }

    if (car.type == ObstacleType::Van) {

        Sprites::drawSelfMasked(car.x, car.y, Car_Van, 0);

    }

}

void moveCars(Obstacle &car1, Obstacle &car2, int8_t increment) {

    car1.x = car1.x + increment;
    if (car1.x <- 20) { launchCar(car1, car2); }

    car2.x = car2.x + increment;
    if (car2.x <- 20) { launchCar(car2, car1); }

}

void launchCar(Obstacle &car1, Obstacle &car2) {

    if (car2.x > 110) {

        car1.x = random(car2.x + 20, car2.x + 70);

    }
    else {

        car1.x = random(130, 180);

    }

}
