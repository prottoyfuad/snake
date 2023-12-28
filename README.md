## SNAKE

### INSTRUCTIONS

Execute `./play` from terminal (preferably in fullscreen) to start playing the game!
_(Assuming you have_ `gcc` _installed!)_
Use the arrow keys to control the movement of the snake.
Collect the small foods `*` and large foods `$` to score points.
If the snake hits any boundary wall `#` or it's own body cells `o`, it's game over!

You can change the difficulty level of the game by editing the source `asn.c`.
By default, the level is set to 8.
It's okay to set any level in range `[1, 1'000'000]`,
but anything over 20, is almost impossible to play!

The snake lives within a square with side length n.
By default, the the length of side is set to 20 units.
You can change the value of n to adjust the size.
It's oke to set it to anything more than 18 to to play indefinitely.
_(i.e. the snake won't get too large that it won't fit inside the square!)_
But also don't make it too large, that the square won't fit in your screen!

**ENJOY!**
