# Overview

In this tutorial we will build a version of the classic arcade game **Frogger**.  This game is complex enough that we can cover a lot of programming and gaming concepts yet simple enough that we will not lose interest along the way!

In case you haven’t played the game in the last 20 years, allow me to recap the game play for you.  You are a frog (of course!) and must cross the three lanes of traffic – avoiding the cars and trucks - and then the cross the treacherous river by jumping between logs to the safe locations on the far side of the river.  You can also catch a ride on the turtles that swim in the river but beware as they are likely to submerge at any stage.

The mechanics in this game are quite simple and in this tutorial we will implement them progressively allowing you to review and test each logical step before proceeding to the next.  


## Getting Started

Before starting this tutorial, it is recommended that … xxxxx

In order to fully understand the code, it would be helpful if you have read a little about `enums` and `structures`.  The tutorial will gloss over the details of these so please if something doesn’t make sense then refer back to these definitions.

The tutorial will be broken into nine steps:

* implementing the cars
* mixing up the car’s movements
* adding the player’s frog
* adding collision detection and explosion graphics
* adding logs and turtles
* adding turtle submerge logic
* adding home logic
* adding score logic
* adding a title and game over screen

Frogger has two distinct modes of play – crossing the road then crossing the river.  In this section, we will implement the road and cars first as it is easier of the two modes and will allow us to build on the code to implement the river section later. 

Review the code that has been loaded with this tutorial.  As you will see, there are only two files – `Frogger.ino` which contains the actual code and `Images.h` which includes the converted graphics referenced in the code.

You should be familiar with the `.ino` file from the tutorials already completed.  It is the main program file and generally contains all of your custom code.  However, when writing programs you will need to use existing libraries to perform basic task such as rendering graphics and reading button presses.
 
The top line of the `Frogger.ino` file shows how we include the common Arduboy2 library and any other libraries needed.  We can also refer to our own libraries or files in the same way – the two lines below show how we refer to the Arduboy standard library and our graphics file.  Before you ask, libraries shared across multiple programs – such as the Arduboy2 library – are delimited by angle braces whereas files unique to the program (and in the same directory or a subdirectory as the program) are delimited by quotation marks.

```cpp
#include <Arduboy2.h>
#include "Images.h"
```

As we progress through the stages of development, we will be simply adding additional code and graphics to these two files.  In a larger game, these files can be split apart into separate files to make it easy to organise and locate code however in a simple game like Frogger it is (almost!) manageable in two files.
