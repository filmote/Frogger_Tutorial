# Adding Turtle Submerge Logic

In this section, we will add the logic to make the turtle submerge and come back to the surface randomly and alter the existing logic that checks to see if you are on a log or turtle to include the turtle’s position above or below water.

We will implement the submerge logic using a counter that we will decrement on each `frameCount` in a similar way we animated the explosion graphics.  Each turtle will require its own counter so we can extend the `Obstacle` struct to include an extra variable, `delay`, and a little function that will decrease it until it reaches zero.

```cpp
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
```

When launching the water obstacle, we can now assign a random number to the new `delay` variable to control the length of time the turtle is above and below water.  As you will see in the rendering logic to follow, the turtle is partially or completely below the water when its counter is in the range of `5` to `39` and it is fully above water for ranges higher than that.

Therefore, a random delay in the range of `70` to `150` provides enough time for the turtle to be above the water before it submerges.  You can play with this range and see how it affects the game if you like.

```cpp
void launchWaterObstacles_Left(Obstacle &obstacle1, Obstacle &obstacle2) {

    ...
    obstacle1.delay = random(70, 150);

}
```

When rendering the turtles, we simple use the delay to determine whether the turtle is fully above, partially below or completely below the water.  

You will notice that the switch statement below uses two separate tricks – each case statement specifies a range of values and multiple case statements are used before rendering the images themselves.  This is one of the neat features of the switch statement!  Looking at the first part of the switch statement, the two clauses are saying ‘if the delay value is between `0` and `4` or the delay value is between `40` and `150` then render the image.’

```cpp
void drawWaterObstacle(Obstacle waterObstacles, bool right) {

    if (waterObstacles.type == ObstacleType::FloatingLog) {

        ... 
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
```

You may have noticed that the case statements are not in ‘numerical’ order.  This is not a requirement of the switch statement and I have taken advantage of this fact to streamline the code.  

If you were to restructure the switch statement, you could alter it to look like that below.  However, you would need to add the rendering code to each case statement resulting in a small increase in code.  It may not sound like much but when you are writing large games using devices with minimal memory, even these small memory savings can help when you reach the limits!

```cpp
switch (waterObstacles.delay) {

    case 0 ... 4:       // Completely above water.
        Sprites::drawSelfMasked(x, y, Turtle_1, right);
        break;

    case 5 ... 9:       // Partially above water.
    case 10 ... 14:     // Almost submerged. 
    case 15 ... 29:     // Underwater!  Render nothing.
    case 30 ... 34:     // Almost submerged.
    case 35 ... 39:     // Partially above water.
    case 40 ... 150:    // Completely above water.

}
```

Another little trick you may have noticed is the parameter named right in the `drawWaterObstacle()` function declaration.  When calling the function, passing a `false` will render the turtles facing left and passing a `true` will render them facing right.

But how?

Each of the turtle images includes two frames showing the turtles facing left (frame 0) and right (frame 1).  You may also have noticed that the call to `Sprites::drawSelfMasked()` uses the parameter right to indicate which image index to use.

But again, how?

In C++ (and many other languages), a Boolean value can be used interchangeably with a numeric number.  A `false` value equates to `0` and a `true` to `1`.  Neat, huh?

Finally, we can change the logic that determines whether the player is sitting on a turtle or not to exclude the point where the turtles are completely submerged.  Rather than change the existing `switch` code, I have simply made a check to see if the obstacle is a turtle and its underwater – if so then we know immediately that the return value must be `false`.  

```cpp
bool onWaterObstacle(Obstacle obstacle) {

    if (obstacle.type == ObstacleType::Turtles && obstacle.delay >= 15 && 
        obstacle.delay <= 29) return false;

    ...

}
```

In the next section, we will add the logic to allow you to finally make it home.  To jazz it up a touch, we will add a visual sparkle that will play once all home locations are full so the player realises what a feat they have just achieved!
