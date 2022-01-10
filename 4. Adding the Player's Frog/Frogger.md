# Adding the Frog

In this section we will add the player (frog) and move it around the lower half of the screen in response to the player pressing the console buttons.  In the spirit of keeping the sections (relatively!) short, we will leave the detection of collisions for the next section.

Near the top of the `Frogger.ino` file, you will see that we have added a struct, named Frog, to capture the location of the player’s frog as it moves around the screen.  

```cpp
struct Frog {

    uint8_t x;
    uint8_t y;

    void reset() {

        x = 62;
        y = 59;

    }
};
```

This structure is similar to that used for the cars in that it contains an x and y value but the obstacle type has been omitted as – funnily enough – a frog is always a frog.  However, you may notice that the structure has a function imbedded in it that will reset the location of the frog to the middle of the lower road verge.  This is one of the added capabilities of structures that I hinted at in the section **Step 1: Implementing the Cars** earlier.  

Just as we did before when defining the array of cars, we can define a variable of type `Frog` called `player`.  We can also add a line to the bottom of the existing `resetGame()` function to reset the player at the start of the game.  

```cpp
Frog player;
Obstacle cars[NUMBER_OF_CARS];

void resetGame() {

    cars[0] = { 130, 32, ObstacleType::Car };
    cars[1] = { 180, 32, ObstacleType::Truck };
    ...

    player.reset();

}
```

Now we can look at capturing the button presses and translate those into frog movements.

The Arduboy library provides a number of functions to detect whether a button has just been pressed or pressed and held down, named `justPressed()` and `pressed()` respectively.  There are other functions to detect whether buttons have been released, are not being held and so forth.  Further information can be found [here] (https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/index.html).

To use the `justPressed()` function, a call must be made to `arduboy.pollButtons()` at the top of the main `loop()` to allow the library to determine what button have changed state in this frame compared to the previous state:

```cpp
void loop() {

    if ( !arduboy.nextFrame() ) return;
    arduboy.clear();
    arduboy.pollButtons();
    ...
```

Now we can detect the pressing of buttons and act accordingly.  The sample code below detects the pressing of the Up button and reduces the player’s `y` value (remember the top left-hand corner of the screen has the coordinates of 0, 0) making them move from the verge, through the three road lanes to the centre median strip.

```cpp
   // Control the frog's movements ..

    if (arduboy.justPressed(UP_BUTTON)) {

        switch (player.y) {
            
            case 59:        player.y = 50; break;
            case 50:        player.y = 41; break;
            case 41:        player.y = 32; break;
            case 32:        player.y = 26; break;

        }

    }
```

The code uses a `switch` – which is similar to a bunch of if statements.  In the example above, the player’s existing `y` value is tested and the matching case is executed.  This code could be written like that below but IMHO it’s not as readable and now is as good a time as any to introduce it – you will see it in use in a more complex form later in the tutorial when we implement the submerging turtles!

```cpp
   if (arduboy.justPressed(UP_BUTTON)) {

        if (player.y == 59)         { player.y = 50; }
        else if (player.y == 50)    { player.y = 41; }
        else if (player.y == 41)    { player.y = 32; }
        else if (player.y == 32)    { player.y = 26; }

    }        
```

Implementing the down functionality is almost identical with the `y` value being increased to produce a downward motion.  


> Your Turn: <br/>
> Using the logic that handles the up button as a guide, add code immediately below it to handle the down button.  As the player is moving down the screen, the `y` value of the `player` struct must increase.


The right movement is handled by the code shown below.  In addition to checking which button was pressed, we also check the frog’s existing `x` position to ensure that the player is not already on the far right of the screen.  The last thing we want is for the frog to move out of view!

```cpp
    if (arduboy.justPressed(RIGHT_BUTTON) && player.x < 120) {

        player.x = player.x + 5;

    }
```

> Your Turn: <br/>
> Add the logic for the left movement.  To avoid the playewr moving off the left hand side, ensure that the code prevents the `x` value from going below `0`.

If all is OK, we move the player 5 pixels in either direction.

At this point, you can compile the game and upload it to your device.  As you will find, you can move the frog around the lower half of the screen but there is nothing stopping you from running across the vehicles!  Oh dear ..

In the next section, we will add collision detection and a cute little explosion graphic when you are hit be a vehicle.
