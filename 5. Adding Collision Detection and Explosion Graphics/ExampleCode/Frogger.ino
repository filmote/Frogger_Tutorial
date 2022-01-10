#include <Arduboy2.h>
#include "Images.h"

#define NUMBER_OF_CARS 6

Arduboy2 arduboy;

enum ObstacleType {
    Car,
    Truck,
    Van,
};

struct Frog {

    uint8_t x = 62;
    uint8_t y = 59;

    void reset() {

        x = 62;
        y = 59;

    }
};

struct Obstacle {

    int16_t x;
    uint8_t y;
    ObstacleType type;           

};

Frog player;
Obstacle cars[NUMBER_OF_CARS];
uint8_t explodingFrogCounter = 0;

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


    // Update exploding frog counter (if active) ..

    if (arduboy.frameCount % 4 == 0 && explodingFrogCounter > 0) {

        explodingFrogCounter--;

        if (explodingFrogCounter == 0) {

            player.reset();   

        }

    }


    // Control the frog ..

    if (explodingFrogCounter == 0) {
            
        if (arduboy.justPressed(UP_BUTTON)) {

            switch (player.y) {
                
                case 59:        player.y = 50; break;
                case 50:        player.y = 41; break;
                case 41:        player.y = 32; break;
                case 32:        player.y = 26; break;

            }

        }

        if (arduboy.justPressed(DOWN_BUTTON)) {

            switch (player.y) {
                
                case 26:        player.y = 32; break;
                case 32:        player.y = 41; break;
                case 41:        player.y = 50; break;
                case 50:        player.y = 59; break;

            }

        }

        if (arduboy.justPressed(RIGHT_BUTTON) && player.x < 120) {

            player.x = player.x + 5;

        }

        if (arduboy.justPressed(LEFT_BUTTON) && player.x > 5) {

            player.x = player.x - 5;

        }

    }


    // Update the cars positions ..

    if (arduboy.frameCount % 2 == 0) {

        moveCars(cars[0], cars[1], -1);
        moveCars(cars[4], cars[5], -1);

    }
    else {

        moveCars(cars[0], cars[1], -2);

    }

    moveCars(cars[2], cars[3], -1);


    // Has the frog collided with a car?

    if (explodingFrogCounter == 0) {

        for (uint8_t i = 0; i < 6; i++) {

            if (carCrash(cars[i])) {

                explodingFrogCounter = 7;

            }

        }

    }

    drawScreen();


    // Draw frog .. even if exploding!

    if (explodingFrogCounter > 0) {
        Sprites::drawSelfMasked(player.x - 10, player.y - 8, Frog_Exploding, explodingFrogCounter - 1);
    }
    else {
        Sprites::drawErase(player.x - 1, player.y - 1, FrogMask, 0);
        Sprites::drawSelfMasked(player.x, player.y, FrogImage, 0);
    }

    arduboy.display();

}

void resetGame() {

    cars[0] = { 130, 32, ObstacleType::Car };
    cars[1] = { 180, 32, ObstacleType::Truck };
    cars[2] = { 64, 41, ObstacleType::Truck };
    cars[3] = { 110, 41, ObstacleType::Van };
    cars[4] = { 45, 50, ObstacleType::Car };
    cars[5] = { 70, 50, ObstacleType::Van };

    player.reset();
    explodingFrogCounter = 0;

}

void drawScreen() {
  
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

bool carCrash(Obstacle car) {

    Rect frogRect = { player.x, player.y, 4, 5 };
    Rect carRect = { car.x, car.y, 0, 6 };

    switch (car.type) {

        case ObstacleType::Car:
            carRect.width = 10;
            break;

        default: break;
            
    }

    return arduboy.collide(frogRect, carRect);

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

    car1.type = static_cast<ObstacleType>(random(ObstacleType::Car, ObstacleType::Van + 1));

}
