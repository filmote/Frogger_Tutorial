# Adding Home Logic

In this section, we will add the logic to allow the player to jump into a home position at the top of the screen.  When a player achieves this, the frog will then be repositioned at the bottom of the screen to allow them to try again.  As promised in the previous section, once all positions are full we will display a little sequence to acknowledge the feat.

Before we start, we need to define an array of Boolean flags to allow us to record which home locations are occupied or not.  You will see in our game, there are five home locations and hence our array needs five elements.  I have also added some simple logic in the `resetGame()` function to reset these home locations between games.

```cpp
bool home[5] = { false, false, false, false, false };

void resetGame() {

    ...

    for (uint8_t i = 0; i < 5; i++) {
        home[i] = false;
    }

}
```

When playing the game, it was really unfair to make the player hit the up button **exactly** when the frog was aligned with the home location so I have allowed the player to simply hold the button down until they are properly aligned.

This was done by modifying the existing code that detects the button presses made by the player.  Immediately after the original code that handles the pressing of the Up button, I have added a second test for `arduboy.pressed(UP_BUTTON))` which will detect whether they are holding the button down.  

If so, a check is made to ensure the player is in the correct position to enter the home location and that the location is currently empty before moving the player into that position.  The `home[]` array is then updated to `true` to indicate the position is full before resetting the frog location to allow the player to start again.

```cpp
void loop() {

    ...

    // Control the frog's movements ..

    if (explodingFrogCounter == 0 && homeFullCounter == 0) {
            
        if (arduboy.justPressed(UP_BUTTON)) {

            switch (player.y) {
                
                case 59:        player.y = 50; break;
                case 50:        player.y = 41; break;
                ...

            }

        }

        else if (arduboy.pressed(UP_BUTTON) && player.y == 7) {         

            if (player.x >= 11 && player.x <= 13 && home[0] == false) {
                home[0] = true;     
                player.reset();         
            }

            else if (player.x >= 36 && player.x <= 38 && home[1] == false) {
                home[1] = true;                    
                player.reset();             
            }

            ...
```

> Your Turn: <br/>
> Add home logic for the reamining positions.  These positions have `x` values of `63`, `87` and `112`.  As shown in the first two positions, we have accepted `x` values +/- ` to make it a little easier for the player.

Immediately after a frog makes it home, a check of the `home[]` array is made to see if all positions are complete.  If so, a new variable, `homeFullCounter`, is set to a non-zero value to indicate that an animation should occur.

```cpp
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
```

The logic for this is very similar to the code we used to display the explosion animation and I encourage you to review the code where you see the `homeFullCounter` variable and compare it to the original explosion code.

One last little detail - when rendering the screen now, we need to populate any home locations based on the values stores in the `home[]` array.  This code is shown below and seems straight forward except for that convoluted if statement.  What is this doing?  I will let you ponder that ..

[spoiler]
When there is no animation to display, the `homeFullCounter` variable will be equal to zero so the second half of the condition will always be true and the if statement can be reduce down to `if (home[i] && true)` which will render a frog in the home location if it is populated.

If there is an animation happening, every value in `home[]` will equal `true` and the value of `homeFullCounter` will decrease from 20 to 0.  The modulus function will evaluate the `homeFullCounter` value and return a new value between `0` and `4` which will flash the frog from right to left in the home positions.  You will see this when you run the code!
[/spoiler]
 
```cpp
void drawScreen() {
  
    Sprites::drawOverwrite(0, 0, Top, 0);

    for (uint8_t i = 0; i < 5; i++) {

        if (home[i] && (homeFullCounter == 0 || ((homeFullCounter - 1) % 5) == i)) {

            Sprites::drawSelfMasked(12 + (i * 25), 0, FrogImage, 0);

        }

    }
```

Really, we are almost there!  Some addition code for scores, a title screen and a game over sequence and we are done.
