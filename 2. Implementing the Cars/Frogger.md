# Implementing the Cars

In this section, we will focus on adding the cars to the game play.  The code needed will track their locations, move them from right to left and then reposition them on the right-hand side of the screen so they drive endlessly.  Although this seems simple enough, we will quickly cover a number of programming concepts.  These same concepts will be covered again in future steps so do not be concerned if they don’t all make sense the first time.

As mentioned in the Getting Started section, above, the code is split into two files named `Frogger.ino` and `Images.h`.

Opening the `Images.h` file reveals the encoded graphic data for the three vehicle types used in the game – a small or compact car, a truck and a van.  This encoded data was created using the tools and process described here. 

```cpp
const uint8_t PROGMEM Car_Small[] = {
10, 6,
0x1e, 0x3f, 0x3f, 0x3f, 0x21, 0x3f, 0x3f, 0x21, 
0x3f, 0x1e, 
};

const uint8_t PROGMEM Car_Truck[] = {
17, 6,
0x3f, 0x21, 0x3f, 0x3f, 0x1e, 0x3f, 0x3f, 0x3f, 
0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 
0x3f, 
};

const uint8_t PROGMEM Car_Van[] = {
11, 6,
0x1e, 0x3f, 0x21, 0x3f, 0x2d, 0x2d, 0x3f, 0x2d, 
0x2d, 0x3f, 0x1e, 
};

```

Switching back to the `Frogger.ino` file, I have defined an `enum` called `ObstacleType` that defines these three car types.  In later steps when we add logs and turtles, we will extend the enumeration to include these extra items – hence I have called it `ObstacleType` rather than `VehicleType`.

We will use this enumeration later to categorise the vehicles and allow us to draw the correct graphic when it comes time to draw the screen.

```cpp
enum ObstacleType {
    Car,
    Truck,
    Van,
};
```



## Defining a Collection of Cars

In this version of Frogger, I have limited the number of cars to six or two per road lane.  In order to keep track of each car’s location and type, we could define a series of variables such as ..

```cpp
int16_t car1_x;
uint8_t car1_y;
ObstacleType car1_type;           

int16_t car2_x;
uint8_t car2_y;
ObstacleType car2_type;         
...
```

.. and so on for the six cars.  Although this is a valid approach, it is not very efficient as we cannot perform a single task on each of the cars as the code must use the correct variable name for each of the six cars.

Another approach is to make the variables an array like that shown below.  

```cpp
int16_t car_x[6];
uint8_t car_y[6];
ObstacleType car_type[6];           
```

With this approach, we could write a single function that uses an index to refer to the information about a single car that the function is operating over.  For example, the function below will draw the car nominated by the index value in its correct position.  

```cpp
void drawCar(uint8_t index) {

    Sprites::drawOverwrite(car_x[index], car_y[index] ..

}
```

An even better approach is to use a structure to group all the related variables about a single car together.  This approach allows us to then create an array of car obstacles which we can then refer to individually.

```cpp
struct Obstacle {

    int16_t x;
    uint8_t y;
    ObstacleType type;           

};

Obstacle cars[6];
```

Our example `drawCar()` function can be changed to accept a single car parameter (of structure type `Obstacle`) as shown below.  In addition to being much more readable that the example above, the use of structures allows us to do other tricks which we will see in future steps.

```cpp
void drawCar(Obstacle car) {

    Sprites::drawOverwrite(car.x, car.y ..

}
```


## Initial placement of the Vehicles

Before we start moving the cars and trucks around, we need to place them initially and define what type (car, truck or van) they are.  Thinking ahead, we will need to be able to reset the position of the cars, the score and number of lives left between games later so it is best to add this code to a function that we will be able to reuse.  I have called it `resetGame()` as shown below.

```cpp
void resetGame() {

    cars[0] = { 130, 32, ObstacleType::Car };
    cars[1] = { 180, 32, ObstacleType::Truck };
    cars[2] = { 64, 41, ObstacleType::Truck };
    ...

}
```


    cars[3] = { 110, 41, ObstacleType::Van };
    cars[4] = { 45, 50, ObstacleType::Car };
    cars[5] = { 70, 50, ObstacleType::Van };


As you can see, the cars are individually populated using a notation called Brace Initialization.  In this format, the parameters of the structure are defined in a comma separated list in the **same order they are declared in the structure itself**.  Thus, for the first car in the array (index 0), the `x` value is set to `130`, the `y` value to `32` and the obstacle type to ‘Car’.

> Your Turn: <br/>
> The `resetGame()` function has only specified the starting location for three of the six vehicles in game.  Add the remaining 3 vehicles at locations (110, 41), (45, 50) and (70, 50) and of types Van, Car and Van, respectively.


We can now add a call to `resetGame()` into the default `setup()` function to ensure the cars are initialised.

```cpp

void setup() {
  
    arduboy.begin();
    arduboy.setFrameRate(45);
    arduboy.initRandomSeed();

    resetGame();

}   
```


## Moving the Cars

Now that we have an array of cars, we can start moving them from right to left.  However, as we want the cars to move to the far right-hand side after they have travelled across the screen, we will use a function to move them rather than simply decrementing the car structure’s `x` value.

The function `moveCars()` achieves this by detecting when the vehicle is completely out of view on the left-hand side and, if so, adding a large amount to place it beyond the screen width to the right.  Our vehicles are range between `10` and `17` pixels in width therefore when rendered at a position of `-20`, they are not visible at all.

```cpp
void moveCars(Obstacle &car, int8_t increment) {

    car.x = car.x + increment;
    if (car.x <- 20) { car.x = car.x + 148; }

}
```

We can then move the cars using the `moveCars()` function in our main `loop()` by calling the function consecutively for each car in the array.

> Your Turn: <br/>
> In the main `loop()`, add some code that iterates through the array of cars.  For each car found, call the `moveCars()` function passing both the car and the increment to move the car each frame.  As we are moving from right to left, this should be a value of `-1`.


## Rendering the Cars Onscreen

After all that leg-work, we are ready to actually render the cars on the screen.  The `drawCar()` function, shown below, takes a car obstacle as a parameter.  It then tests to see what obstacle type it is before rendering the appropriate image.  The use of the `ObstacleType` enum makes it really clear what each `if` statement is testing.

The `drawSelfMasked()` function will render an image at the specified coordinates.  The third parameter is the name of the image itself (as defined in our `Images.h` file) and the fourth parameter is an index for images that contain more than one frame.  For now, you can ignore the fourth parameter and set it to a default value of zero.  We will use the index parameter in a later step when we implement the turtles.  If you are unfamiliar with the various draw methods of the Sprites class, it might be worth reviewing the xxxxx  page for more information.  

```cpp
void drawCar(Obstacle car) {

    if (car.type == ObstacleType::Car) {

        Sprites::drawSelfMasked(car.x, car.y, Car_Small, 0);

    }

    ...
}
```

> Your Turn: <br/>
> Add two additional if statements to handle the remaining two car types, Truck and Van.  Their image names are `Car_Truck` and `Car_Van` respectively.


Now that we have a simple function that renders a single car, we can wrap this up into an overall function that renders the road sides, traffic lanes and all six cars.

```cpp
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
```

If you compile the code and upload it to your device, you will see the cars moving from right to left continuously.  Pretty cool, huh?

Wow, that was a lot of work to do just to get some cars moving!  But the good news is that you have covered a lot of ground – used an `enum` to describe the obstacle types, used a simple `struct` to hold the information about each car and created an array of cars from that same structure.

But .. you may have noticed that the cars all move at the same speed and the gap between them is constant which would make avoiding them really simple.  In the next section, we will rework the car movements a little to spice the game up.
