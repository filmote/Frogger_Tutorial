# Add a Title and Game Over screen

A typical design pattern for a game is to have a variable that describes what mode the program is in.  For a basic game (like ours!) this will include a title screen, playing the game itself and a game over stage.  For more complex games, this might include modes for setting options, checking inventory, saving scores and so on.

The game mode can be defined in a new `enum` as shown below:

```cpp
enum GameMode {
    TitleScreen,
    Play,
    GameOver,
};

GameMode gameMode = GameMode::TitleScreen;
```

Adding the title and game over sequence requires some drastic surgery to the existing main `loop()`.  The code that was originally in it must be moved to a separate function – in our case `game_Loop()` – and a new switch added that controls what mode is being shown at any time.  The result of this radical surgery is shown below:

```cpp
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
    ...

}

void gameOver_Loop() {

    drawScreen();
    Sprites::drawExternalMask(39, 23, GameOver_Img, GameOver_Mask, 0, 0);
    ...

}

void game_Loop() {

    ... original code!
```

In each mode, the pressing of a button or some other event will force the `gameState` to update to another mode.  For example, when a player presses the ‘A’ button from the title screen, the mode is updated to `GameMode::Play` which has the effect of starting the game.

```cpp
void title_Loop() {

    Sprites::drawOverwrite(0, 0, Title, 0);

    ...

    if (arduboy.justPressed(A_BUTTON)) {

        gameMode = GameMode::Play;
        resetGame();

    }

}
```

Searching for `gameMode` in the code will reveal other points where it is updated.  

Congratulations!  You have built a version of **Frogger** and delved into a lot of sophisticated programming topics. 
