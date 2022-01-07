# Mixing up the Car Movements

In order to vary the speed of the car movements and to randomly change the gap between them, we need to rework some of the code we just completed.  What?

Previously the cars in a single lane of the road could be considered individually – one the car had moved off the side of the screen, we could simply place it on the right-hand side knowing that it would never clash or overlap the other car in the same lane.

Now, when we move a car to the right-hand side of the screen we need to know where the other car in the same lane is so we can randomly place the car behind the other one yet make sure they do not overlap.  The `launchCar()` function below facilitates that by accepting two parameters – the car to be launched (car) and the other car in the same lane (car2) – and then calculating a random placement based on the second car’s existing location.

The first portion of the if statement checks the other cars position and if it is on the far-right of the screen or even beyond the right boundary of the screen, then it calculates a new starting point for the car being re-launched that is between `20` and `70` pixels **further right than the other car**.

If the other car is somewhere in the middle of the screen, the remainder of the if statement will assign an `x` value to the car being re-launched somewhere between `130` and `180` pixels right ensuring it starts off-screen but not too far behind the other vehicle.

```cpp
void launchCar(Obstacle &car1, Obstacle &car2) {

    if (car2.x > 110) {
        car1.x = random(car2.x + 20, car2.x + 70);
    }
    else {
        car1.x = random(130, 180);
    }

}
```

One other little detail that should be pointed out in this function is the use of the ampersand (&) in front of the `car1` and `car2` parameters in the function declaration.  When passing values in C++, the values are copied by default and any changes to their values within the function would not affect the original variable.  The copy of the variable’s information is discarded on completion of the function.

Adding the ampersand in front of the variable names within a function tells the C++ compiler that you want to pass a reference to the original variable rather than a copy of its value.  By passing a reference, you can then modify the variable and have those values be reflected outside of the function in the original variable.  In our case, we want to change the value of car1’s `x` value therefore we need a reference to it.  

But I hear you ask “why have an ampersand on car2 if we are not changing it?”.  Good question as the function would work exactly the same way with or without the ampersand.  Using a reference for an object or structure, rather than a copy, actually saves memory at runtime as the compiler does not need to copy the data but can just reuse the same memory locations.  On a device with minimal memory, this can become really important.  For now, remove the ampersand from the car2 variable and test the code to satisfy yourself that it works either way.

Now that we have a function to relaunch a car based on the location of its lane sharing pair, we need to change the existing logic to move them in pairs.  Again, a simple function can assist .. voila!

```cpp
void moveCars(Obstacle &car1, Obstacle &car2, int8_t increment) {

    car1.x = car1.x + increment;
    if (car1.x <- 20) { launchCar(car1, car2); }

    car2.x = car2.x + increment;
    if (car2.x <- 20) { launchCar(car2, car1); }

}
```

In addition to the references to the cars being moved, this parameter accepts a third parameter which indicates how many pixels to move the car – more on that soon.  What you will notice in the code above is that we move the first car and test to see if it needs relaunching before moving the second car and then testing to see if it too needs relaunching.  The two calls to the `launchCar()` function specifies the car that has just been moved followed by the other car – after moving car1 the parameters to `launchCar()` are car1 followed by car2 whereas after moving car2 the `launchCar()` parameters are reversed.

Finally, we can use a timing trick to make the cars move at different speed.  In the beginner’s tutorial, you would have seen how the Arduboy library regulates the speed of a game using a specified frame rate.  Under the covers, the library keeps the number of frames it has completed and exposes this in a variable called `frameCount`.

We can use the `frameCount` variable to alter the speed of the different cars.  Although the `frameCount` value simply grows and grows (until in eventually wraps around to zero again), we can use the modulus function to determine if it is an odd or even number as shown below and move the cars by different amounts.

In the function below, the pairs of cars with index 2 and 3 are moved one pixel every frame of the game to produce a ‘medium’ speed.  The cars with an index of 4 and 5 are moved one pixel only when the `frameCount` is even (ie when the value mod two returns a value of zero) producing a ‘slow’ speed which is half that of cars 2 and 3.  Finally, cars 0 and 1 are moved 1 pixel every even frame and 2 pixels every odd frame producing a fast speed that is one-and-a-half times faster than cars 2 and 3.

```cpp
void loop() {

    ...

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
```

So, compile the code and see the results!  As expected, the cars move at different speeds and the gaps between them vary randomly.  It’s starting to look like a game already.

It was worth labouring over the details in this section as we will use the same logic when we complete the log and turtle movements later.

In the next section we will look at adding the frog and implementing some simple movements.
