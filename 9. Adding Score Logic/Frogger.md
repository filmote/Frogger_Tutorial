# Adding Score Logic

What is a game without a score?

In this section, we will add some simple logic to add scores as the player progresses up the screen and reaches the home positions.  A bonus will also be added as the player fills all the home positions.

Search for the variable score in the code and note where it is declared, where the value is increased and where the score is reset.  By now, this should all be self-explanatory.

You will also discover a block of code that renders the score to the screen.  Due to the lack of real-estate on the screen to use the Arduboy libraries’ built-in `print()` function, I have used another trick – an image with multiple frames that represent the digits zero to nine in a small 3x5 pixel font.

In true video-game fashion, I wanted my scores to have leading zeroes so I have employed a little trick to determine what each digit of the score should be.  As you can see below, each digit of the score is progressively evaluated by dividing it (initially) by 10,000 and rendering the result.  Any value greater than 10,000 is then removed from the score and the process repeated for 1,000 then 100 and so on.

```cpp
if (player.y < 59 || player.x > 47) {

    Sprites::drawOverwrite(0, 59, ScoreImg, 0);

    uint16_t scoreTmp = score;
    Sprites::drawOverwrite(25, 59, Numbers, scoreTmp / 10000);      scoreTmp = scoreTmp - ((scoreTmp / 10000) * 10000);
    Sprites::drawOverwrite(29, 59, Numbers, scoreTmp / 1000);       scoreTmp = scoreTmp - ((scoreTmp / 1000) * 1000);
    Sprites::drawOverwrite(33, 59, Numbers, scoreTmp / 100);        scoreTmp = scoreTmp - ((scoreTmp / 100) * 100);
    ...

}
```

> Your Turn: <br/>
> Add the two remaining lines that handle the tens and units column of the score.

What does the test for the player’s location have to do with anything?

[spoiler]
It allows the score to be hidden when the player is at the bottom of the screen and moves to the far left.  Without this, the frog image would be rendered on top of the score which is a bit ugly!
[/spoiler]

In the next section, we will wrap the code up with a game over banner and a title screen.
