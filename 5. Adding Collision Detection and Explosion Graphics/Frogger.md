# Adding Collision Detection and Explosion Graphics

In this section, we will add collision detection between the frog and cars using an inbuilt function of the Arduboy library.  Once a collision is detected, we will then render a simple animation of the frog exploding using a single, multi-framed image.

First let’s look at detecting the collision between the frog and a vehicle.  The Arduboy library provides two structures – a point that has simple x / y coordinates and a rectangle that is described using the upper-left coordinates, width and height – and two collision functions that can detect whether a point or rectangle have collided with another rectangle.
In our game, we need only use the function that detects the collision between the rectangle described by the player and the rectangles that describe each vehicle.

The code below shows a simple function that will detect whether the frog has collided with the car passed as a parameter to the function.  The first two lines of code create rectangles using the inbuilt Arduboy `Rect` structure and populate these with the coordinates, width and height of the player and car respectively.  

You may have noticed that the width of the car rectangle is set to zero.  As the vehicle are actually different lengths, the width is set to the appropriate width of the vehicle being tested based on its type – car, truck or van – within the following switch statement.

```cpp
bool carCrash(Obstacle car) {

    Rect frogRect = { player.x, player.y, 4, 5 };
    Rect carRect = { car.x, car.y, 0, 6 };

    switch (car.type) {

        case ObstacleType::Car:
            carRect.width = 10;
            break;

        case ObstacleType::Truck:
            ...

        default: break;
            
    }

    return arduboy.collide(frogRect, carRect);

}
```

> Your Turn <br/>
> In the `carCrash()` function above, add to additional `case` statements to handle the `Truck` and `Van`.  These two vehciles are 17 and 11 pixels in length, respectively.


Finally, any collision is detected between the two rectangles using the Arduboy libraries’ `collide()` function and the result returned as a true or false to the calling code. We can now detect for collisions between the frog and any car, simply be iterating through the car collection in the main `loop()`, as shown below:

```cpp
        for (uint8_t i = 0; i < 6; i++) {

            if (vehicleCrash(cars[i])) {

                // Show the exploding graphics!

            }

        }
```

So how do we display a little graphic when the collision happens?

Animated graphics that occur over a number of frames take a little effort to setup and control while allowing background tasks – like the continual movement of the cars – to continue.  We do this by introducing a new variable, named `explodingFrogCounter`, and use it to track whether the animation is still being played or not.

If you review the `Images.h` file, you will see a new graphic has been created for the explosion images.  This differs from the previous images we were using in that actually has seven distinct images of the animation in one single image.  Each image can be rendered individually using the various `Spites::draw..()` routines by specifying the index of the image as the last parameter of the call.  We will see how to do that in a moment.

Our explosion animation has seven frames.  When a collision is detected, we will set the variable `explodingFrogCounter` to `7` to indicate that it should render the last image from the animation.  We then decrease the variable over the following game frames until it reaches zero at which point the animation is over and the frog can be repositioned at the bottom of the screen and the game can continue.

The detection code above can be changed to set the `explodingFrogCounter` to `7` as shown below:

```cpp
        for (uint8_t i = 0; i < 6; i++) {

            if (vehicleCrash(cars[i])) {

                explodingFrogCounter = 7;

            }

        }
```

Now, we need to change how we render the frog itself.  If the `explodingFrogCounter` is not equal to zero then the frog is exploding and we render the explosion using the `Frog_Exploding` image and specifying the image to render using the `explodingFrogCounter` variable.  You will notice that in order for it to work correctly, we must subtract one from the variable as the `explodingFrogCounter` will have a value between 1 and 7 whereas the indices on the image range from 0 to 6.

```cpp
void loop() {

    ...

    drawScreen();

    // Draw frog .. even if exploding!

    if (explodingFrogCounter > 0) {
        Sprites::drawSelfMasked(player.x - 10, player.y - 8, 
                 Frog_Exploding, explodingFrogCounter - 1);
    }
    else {
        Sprites::drawErase(player.x - 1, player.y - 1, FrogMask, 0);
        Sprites::drawSelfMasked(player.x, player.y, FrogImage, 0);
    }

    arduboy.display();

}
```

If the `explodingFrogCounter` equals zero then we just render the frog normally.

Once a collision has been detected, we need to decrement the `explodingFrogCounter` variable over a short period of time.  However, if we decremented this variable once per frame the graphics would be so quick that it would just look like a blur.

We can use the sane `frameCount` and `modulus` technique that we used previously when launching the vehicles to slow this down over a greater number of frames as shown below.  The code is place at the top of the main `loop()` to ensure it is executed whenever necessary.

```cpp
void loop() {

    ...

    // Update exploding frog counter (if active) ..

    if (arduboy.frameCount % 4 == 0 && explodingFrogCounter > 0) {

        explodingFrogCounter--;

        if (explodingFrogCounter == 0) {

            player.reset();   

        }

    }
```

The test `if (arduboy.frameCount %4 == 0 && explodingFrogCounter > 0)` ensures that the `frameCount` is an exact multiple of four and the `explodingFrogCounter` counter is greater than zero and thus needs to be reduced. 

After reducing the `explodingFrogCounter` variable, a second check is made to see if the variable is now equal to zero – indicating that the animation is over – and game play can continue.  If this is the case, the frog is reset and it repositioned to the middle of the lower verge.

Elsewhere in the code, you will see that checks are made to see if the `explodingFrogCounter` variable is equal to zero before reacting to player buttons or checking for collisions.  In your code, search for `explodingFrogCounter == 0` to see some examples.  These tests prevent the player from moving the frog as it explodes (as shown below) or detecting multiple collisions as the animation is displayed.

```cpp
   // Control the frog ..

    if (explodingFrogCounter == 0) {
            
        if (arduboy.justPressed(UP_BUTTON)) {

            switch (player.y) {
                
                case 59:        player.y = 50; break;
                ...
```

> Your Turn: <br/>
> Review the code and find all uses of the variable `explodingFrogCounter`.  Can you see where it is decremented at the start of the `loop()`, where it is used to prevent the player moving the frog in the middle of the explosion and where it is used when rendering the frog?

Now we are getting somewhere!

If you compile and upload the code to your console, you will see the collision detection and explosions in action.  It is really starting to look like a game!
