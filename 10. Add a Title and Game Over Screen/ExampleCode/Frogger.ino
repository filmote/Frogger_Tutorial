#include <Arduboy2.h>
#include "Images.h"

#define NUMBER_OF_CARS 6
#define NUMBER_OF_WATEROBSTACLES 6

Arduboy2 arduboy;

enum GameMode {
    TitleScreen,
    Play,
    GameOver,
};

enum ObstacleType {
    Car,
    Truck,
    Van,
    FloatingLog,
    Turtles,
};

struct Frog {

    uint8_t x = 62;
    uint8_t y = 59;
    uint8_t lives = 3;

    void reset() {

        x = 62;
        y = 59;

    }
};

struct Obstacle {

    int16_t x;
    uint8_t y;
    ObstacleType type;           
    uint8_t delay;

    void decreaseDelay() {

        if (delay > 0) {
            delay = delay - 1;
        }

    }

};

GameMode gameMode = GameMode::TitleScreen;
Frog player;

uint8_t explodingFrogCounter = 0;
bool home[5] = { false, false, false, false, false };
uint16_t score = 0;
uint8_t titleCount = 0;
uint8_t homeFullCounter = 0;

Obstacle cars[NUMBER_OF_CARS];
Obstacle waterObstacles[NUMBER_OF_WATEROBSTACLES];

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

    switch (gameMode) {

        case GameMode::TitleScreen:
            title_Loop();
            break;

        case GameMode::Play:
            game_Loop();
            break;

        case GameMode::GameOver:
            gameOver_Loop();
            break;
            
    }


    arduboy.display();

}

void title_Loop() {

    Sprites::drawOverwrite(0, 0, Title, 0);

    if (arduboy.justPressed(A_BUTTON)) {

        gameMode = GameMode::Play;
        resetGame();

    }

}

void gameOver_Loop() {

    drawScreen();

    Sprites::drawExternalMask(39, 23, GameOver_Img, GameOver_Mask, 0, 0);

    if (arduboy.justPressed(A_BUTTON)) {

        gameMode = GameMode::TitleScreen;
        titleCount = 0;
        resetGame();

    }

}

void game_Loop() {


    // Update delays ..

    for (uint8_t i = 0; i < NUMBER_OF_WATEROBSTACLES; i++) {
        
        waterObstacles[i].decreaseDelay();

        if (waterObstacles[i].delay == 0) {
            waterObstacles[i].delay = random(50, 100);
        }

    }


    // Update exploding frog counter (if active) ..

    if (arduboy.frameCount % 4 == 0 && explodingFrogCounter > 0) {

        explodingFrogCounter--;

        if (explodingFrogCounter == 0) {

            player.reset();   

            if (player.lives == 0) {

                gameMode = GameMode::GameOver;

            }

        }

    }


    // Update exploding frog counter (if active) ..

    if (arduboy.frameCount % 4 == 0 && homeFullCounter > 0) {

        homeFullCounter--;
        score = score + 15;

        if (homeFullCounter == 0) {

            player.reset();   

            for (uint8_t i = 0; i < 5; i++) {
                home[i] = false;
            }

        }

    }


    // Control the frog's movements ..

    if (explodingFrogCounter == 0 && homeFullCounter == 0) {
            
        if (arduboy.justPressed(UP_BUTTON)) {

            switch (player.y) {
                
                case 59:        player.y = 50; score = score + 2; break;
                case 50:        player.y = 41; score = score + 2; break;
                case 41:        player.y = 32; score = score + 2; break;
                case 32:        player.y = 26; score = score + 2; break;
                case 26:        player.y = 19; score = score + 2; break;
                case 19:        player.y = 13; score = score + 2; break;
                case 13:        player.y = 7;  score = score + 2; break;

            }

        }

        else if (arduboy.pressed(UP_BUTTON) && player.y == 7) {         

            if (player.x >= 11 && player.x <= 13 && home[0] == false) {
                home[0] = true;     
                score = score + 50;   
                player.reset();         
            }

            else if (player.x >= 36 && player.x <= 38 && home[1] == false) {
                home[1] = true;                    
                score = score + 50;               
                player.reset();             
            }

            else if (player.x >= 61 && player.x <= 63 && home[2] == false) {
                home[2] = true;                    
                score = score + 50;               
                player.reset();             
            }

            else if (player.x >= 86 && player.x <= 88 && home[3] == false) {
                home[3] = true;                    
                score = score + 50;               
                player.reset();            
            }

            else if (player.x >= 111 && player.x <= 113 && home[4] == false) {
                home[4] = true;                    
                score = score + 50;               
                player.reset();           
            }


            // Have all the home places been filled?

            bool allFull = true;

            for (uint8_t i = 0; i < 5; i++) {
                
                if (!home[i]) {

                    allFull = false;
                    break;

                }

            }

            if (allFull) {

                homeFullCounter = 20;

            }

        }

        if (arduboy.justPressed(DOWN_BUTTON)) {

            switch (player.y) {
                
                case 7:         player.y = 13; break;
                case 13:        player.y = 19; break;
                case 19:        player.y = 26; break;
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

       

    // Update the water Obstacle positions ..
    
    moveWaterObstacles(waterObstacles[0], waterObstacles[1], -1);

    if (explodingFrogCounter == 0 && homeFullCounter == 0 && player.y == 7 && player.x > 0) {

        player.x--;

    }


    moveWaterObstacles(waterObstacles[2], waterObstacles[3], 1);

    if (explodingFrogCounter == 0 && homeFullCounter == 0  && player.y == 13 && player.x < 124) {

        player.x++;
        
    }


    if (arduboy.frameCount % 2 == 0) {

        moveWaterObstacles(waterObstacles[4], waterObstacles[5], -1);

        if (explodingFrogCounter == 0 && homeFullCounter == 0 && player.y == 19 && player.x > 0) {

            player.x--;
            
        }

    }


    // Has the frog collided with a car or is drowning in the water?

    if (explodingFrogCounter == 0 && homeFullCounter == 0) {

        if (player.y > 19) {

            for (uint8_t i = 0; i < 6; i++) {

                if (carCrash(cars[i])) {

                    player.lives--;
                    explodingFrogCounter = 7;

                }

            }

        }
        else {

            bool isOnALog = false;

            for (uint8_t i = 0; i < 6; i++) {

                if (onWaterObstacle(waterObstacles[i])) {
                    isOnALog = true;
                }
            
            }

            if (!isOnALog) {

                player.lives--;
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

}

void resetGame() {

    cars[0] = { 130, 32, ObstacleType::Car, 0 };
    cars[1] = { 180, 32, ObstacleType::Truck, 0 };
    cars[2] = { 64, 41, ObstacleType::Truck, 0 };
    cars[3] = { 110, 41, ObstacleType::Van, 0 };
    cars[4] = { 45, 50, ObstacleType::Car, 0 };
    cars[5] = { 70, 50, ObstacleType::Van, 0 };

    waterObstacles[0] = { 75, 7, ObstacleType::FloatingLog, 0 };
    waterObstacles[1] = { 25, 7, ObstacleType::FloatingLog, 0 };
    waterObstacles[2] = { 61, 13, ObstacleType::Turtles, 0 };
    waterObstacles[3] = { 5, 13, ObstacleType::FloatingLog, 0 };
    waterObstacles[4] = { 32, 19, ObstacleType::FloatingLog, 0 };
    waterObstacles[5] = { 70, 19, ObstacleType::Turtles, 0 };

    score = 0;
    player.lives = 3;
    player.reset();
    explodingFrogCounter = 0;

    for (uint8_t i = 0; i < 5; i++) {
        home[i] = false;
    }

}

void drawScreen() {
  
    Sprites::drawOverwrite(0, 0, Top, 0);

    for (uint8_t i = 0; i < 5; i++) {

        if (home[i] && (homeFullCounter == 0 || ((homeFullCounter - 1) % 5) == i)) {

            Sprites::drawSelfMasked(12 + (i * 25), 0, FrogImage, 0);

        }

    }

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


    // Draw water Obstacles ..

    drawWaterObstacle(waterObstacles[0], false);
    drawWaterObstacle(waterObstacles[1], false);
    drawWaterObstacle(waterObstacles[2], true);
    drawWaterObstacle(waterObstacles[3], true);
    drawWaterObstacle(waterObstacles[4], false);
    drawWaterObstacle(waterObstacles[5], false);


    // Render the score if the player is not close to it ..

    if (player.y < 59 || player.x > 47) {

        Sprites::drawOverwrite(0, 59, ScoreImg, 0);

        uint16_t scoreTmp = score;
        Sprites::drawOverwrite(25, 59, Numbers, scoreTmp / 10000);      scoreTmp = scoreTmp - ((scoreTmp / 10000) * 10000);
        Sprites::drawOverwrite(29, 59, Numbers, scoreTmp / 1000);       scoreTmp = scoreTmp - ((scoreTmp / 1000) * 1000);
        Sprites::drawOverwrite(33, 59, Numbers, scoreTmp / 100);        scoreTmp = scoreTmp - ((scoreTmp / 100) * 100);
        Sprites::drawOverwrite(37, 59, Numbers, scoreTmp / 10);         scoreTmp = scoreTmp - ((scoreTmp / 10) * 10);
        Sprites::drawOverwrite(41, 59, Numbers, scoreTmp);

    }


    // Render the number of lives left if the player is not close to it ..

    if (player.y < 59 || player.x < 104) {

        for (uint8_t i = 0; i < player.lives; i++) {

            Sprites::drawOverwrite(124 - (i * 6), 59, FrogImage, 0);

        }

    }

}

void drawWaterObstacle(Obstacle waterObstacles, bool right) {

    if (waterObstacles.type == ObstacleType::FloatingLog) {

        Sprites::drawSelfMasked(waterObstacles.x, waterObstacles.y, Log, 0);

    }
    else {

        // Render the turtle based on the delay value.

        switch (waterObstacles.delay) {

            case 0 ... 4:       // Completely above water.
            case 40 ... 150:
                Sprites::drawSelfMasked(waterObstacles.x, waterObstacles.y, Turtle_1, right);
                break;
            
            case 5 ... 9:       // Partially above water.
            case 35 ... 39:
                Sprites::drawSelfMasked(waterObstacles.x, waterObstacles.y, Turtle_2, right);
                break;
            
            case 30 ... 34:     // Almost submerged.
            case 10 ... 14:
                Sprites::drawSelfMasked(waterObstacles.x, waterObstacles.y, Turtle_3, right);
                break;
            
            case 15 ... 29:     // Underwater!  Render nothing.
                break;

        }

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

        case ObstacleType::Truck:
            carRect.width = 17;
            break;

        case ObstacleType::Van:
            carRect.width = 11;
            break;

        default: break;
            
    }

    return arduboy.collide(frogRect, carRect);

}


bool onWaterObstacle(Obstacle obstacle) {

    if (obstacle.type == ObstacleType::Turtles && obstacle.delay >= 15 && obstacle.delay <= 29) return false;

    Rect frogRect = { player.x, player.y, 4, 5 };
    Rect objRect = { obstacle.x, obstacle.y, 0, 5 };

    switch (obstacle.type) {

        case ObstacleType::FloatingLog:
            objRect.width = 25;
            break;

        case ObstacleType::Turtles:
            objRect.width = 22;
            break;
            
    }

    return arduboy.collide(frogRect, objRect);

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

void moveWaterObstacles(Obstacle &obstacle1, Obstacle &obstacle2, int8_t increment) {

    obstacle1.x = obstacle1.x + increment;
    
    if (obstacle1.x <- 20 && increment < 0) { 
        launchWaterObstacles_Left(obstacle1, obstacle2); 
    }

    if (obstacle1.x > 128 && increment > 0) {
        launchWaterObstacles_Right(obstacle1, obstacle2); 
    }
    

    obstacle2.x = obstacle2.x + increment;
    
    if (obstacle2.x <- 20 && increment < 0) { 
        launchWaterObstacles_Left(obstacle2, obstacle1); 
    }

    if (obstacle2.x > 128 && increment > 0) {
        launchWaterObstacles_Right(obstacle2, obstacle1); 
    }

}

void launchWaterObstacles_Left(Obstacle &obstacle1, Obstacle &obstacle2) {

    if (obstacle2.x > 110) {

        obstacle1.x = random(obstacle2.x + 35, obstacle2.x + 70);

    }
    else {

        obstacle1.x = random(130, 180);

    }

    obstacle1.type = static_cast<ObstacleType>( random(ObstacleType::FloatingLog, ObstacleType::Turtles + 1) );
    obstacle1.delay = random(70, 150);

}

void launchWaterObstacles_Right(Obstacle &obstacle1, Obstacle &obstacle2) {

    if (obstacle2.x <0) {

        obstacle1.x = random(obstacle2.x - 70, obstacle2.x - 25);

    }
    else {

        obstacle1.x = random(-70, -25);

    }

    obstacle1.type = static_cast<ObstacleType>( random(ObstacleType::FloatingLog, ObstacleType::Turtles + 1) );
    obstacle1.delay = random(70, 150);

}