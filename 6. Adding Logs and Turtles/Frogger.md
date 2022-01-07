# Adding Logs and Turtles

The logic for the movement of turtles and logs is almost identical to that of the cars so I will gloss over the details and instead just point out the additions and differences.  In this section, we add the code that moves and renders the logs and turtles but will leave the code that makes the turtles submerge to a later step.

Firstly, the existing `ObstacleType` enumeration has been extended to add `FloatingLog` and `Turtles` to the bottom of the list.

```cpp
enum ObstacleType {
    Car,
    Truck,
    Van,
    FloatingLog,
    Turtles,
};
``` 

A second array, similar to that used by the cars, has been added to track the six logs and turtles.  Note that both arrays are of the same type – our structure `Obstacle`.

```cpp
Obstacle cars[NUMBER_OF_CARS];
Obstacle waterObstacles[NUMBER_OF_WATEROBJECTS];
```

The button press handling that moves the frog has also been updated.  You can see additional cases have been added to the up and down switches to handle the upper areas of the screen.

Two major changes to the code centre around the movement of the logs and turtles and the collision detection of these.  When a frog jumps on a log (or turtle), it moves with the log automatically until it reaches the side of the screen at which point the frog is pushed off the log and ultimately into the water.  Secondly, the first and third rows of logs move left to right whereas the second row of logs moves the opposite direction.  This minor difference actually results in a lot of code!

First let’s look at the function that moves the logs.  It is shown below and is similar to the original `moveCars()` function we looked at previously.  One major change is that it detects whether the log has moved off the left-hand **or right-hand** side of the screen and repositions the log back on the opposite side.  The original `moveCars()` only had to handle the left-hand side of the screen.

Depending on the direction the log is travelling, it calls either `launchWaterObstacles_Left()` or `launchWaterObstacles_Right()` which are variations of the original `launchCar()` function we looked at previously.  I recommend you review the two functions to understand the differences and compare them to the original `launchCar()` function.

```cpp
void moveWaterObstacles(Obstacle &object1, Obstacle &object2, int8_t increment) {

    object1.x = object1.x + increment;
    
    if (object1.x <- 20 && increment < 0) { 
        launchWaterObstacles_Left(object1, object2); 
    }

    if (object1.x > 128 && increment > 0) {
        launchWaterObstacles_Right(object1, object2); 
    }
    

    object2.x = object2.x + increment;
    
    if (object2.x <- 20 && increment < 0) { 
        launchWaterObstacles_Left(object2, object1); 
    }

    if (object2.x > 128 && increment > 0) {
        launchWaterObstacles_Right(object2, object1); 
    }

}
```

The code below shows the movement of the logs and the automatic movement of the player if they happen to be sitting on one.  

The function `moveWaterObstacles()`, like the equivalent `moveCars()` accepts two log or turtle references so that it can move both and reposition them relative to each other if needed.  It also accepts a third parameter which is the number of pixels to move the logs which can be a negative value – indicating a left movement – or a positive value – indicating a right movement.

After moving each pair of logs or turtles, a test is made to see if the frog is positioned in that same lane of the river using the frog’s `y` value.  If so, the frog is also moved in line with the logs.  However, if the frog is on the far left or far right of the screen then it is not moved causing it to be pushed along the log and (ultimately) into the water.

```cpp
// Update the water object positions ..

moveWaterObstacles(waterObstacles[0], waterObstacles[1], -1);

if (explodingFrogCounter == 0 && player.y == 7 && player.x > 0) {

    player.x--;

}

moveWaterObstacles(waterObstacles[2], waterObstacles[3], 1);

if (explodingFrogCounter == 0 && player.y == 13 && player.x < 124) {

    player.x++;
    
}

if (arduboy.frameCount % 2 == 0) {

    moveWaterObstacles(waterObstacles[4], waterObstacles[5], -1);

    if (explodingFrogCounter == 0 && player.y == 19 && player.x > 0) {

        player.x--;
        
    }

}
```

You will notice in the above code that the same `frameCount` and `modulus` trick is used on the lowest pair of logs to have them move slower than the other lanes.  As you can tell, this is a common trick to have things move at different speeds to each other!

Rather than detecting collisions with the water objects, we need to ensure that the frog is actually on top of them and not in the water.  You will notice that there is a new function, called `onWaterObstacle()`, that performs a similar function to the `carCrash()` function we used previously.  It uses the Arduboy library’s collide function to ensure that the frog is sitting on a log or turtles.

Previously when detecting collisions with the cars we were interested in any collision.  Now with the logs and turtles, we need to ensure that the frog is sitting on at least one of them.  The original collision detection shown in **Step 4**, has been modified to handle this.

An initial test of the players `y` position allows us to determine whether they are in the lower section of the screen `(y > 19)` or upper section.  The test for collisions with the cars remains unchanged.

The test to see if the player is on a log is a little different in that we must see if the player is on **any** log before assuming they are in the water.  This is achieved using a Boolean flag, named `onALog`, which is initially set to `false` but is set to `true` if any collision between the frog and a log is detected.  After evaluating all six log collisions, if the `onALog` variable is still false then we can assume the frog is in the water and the explosion graphic kicked off.

```cpp 
// Has the frog collided with a car or is drowning in the water?

if (explodingFrogCounter == 0) {

    if (player.y > 19) {

        for (uint8_t i = 0; i < 6; i++) {

            if (carCrash(cars[i])) {

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

            explodingFrogCounter = 7;

        }

    }

}
```

Hopefully, your knowledge of the car movement and collision detection allowed you to understand how we added the logs and turtles.  The addition of the left to right movement of the middle logs complicates it a little but not so much that you shouldn’t be able to follow it.

If you compile and upload the game now, you will see the logs moving in both directions and you can hop between them.  In the next section we will add some additional logic to make the turtles submerge randomly.
