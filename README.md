# Project 4: Rise of the AI 

NYU Tandon School of Engineering
CS-UY 3113 Fall 2025
Project 4: Rise of the AI
Due: 11:59pm, Saturday, Nov 8th 2025
Submission instructions
You must use delta time, a version of our Entity, Map, and Scene classes, and fixed time steps in this assignment. 

You should submit the link to your Github repo on Brightspace. 

Note that any commits done after the deadline will be ignored. 

Do not use any OpenGL functionality that we have not learned in class. 

All of your .cpp files should contain a header comment block as follows:

The main.cpp file you submit should contain a header comment block as follows:

/**
* Author: [Your name here]
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

No late submissions will be accepted.

You must use new assets (art, music, and sound) for this project!
You can not use anything from the lectures (i.e. no walk.png, gosth.png, tileset.png, and witches) and none of the music or sound effects (i.e. "Dirt Jump.waw") etc. 

You must find new assets.

There are some great sources for free assets:

More by the same artist 

itch.io has tons of awesome free graphics. 

Lots of different kinds of free assets. 

Royalty free music.

Requirement 1: Menu Screen (10%)
Show the name of your game and the text "Press enter to start" (or any variation thereof). 

The keycode for enter is: KEY_ENTER 

This can be a solid color background with text on it. 

The menu must be a different Scene object. Do not just show/hide text.

Requirement 2: 3 Levels (40%)
Your game needs to have 3 levels. 

They do not need to be long or complicated. 

They must scroll! (no single screen games) 

If you do not have 3 scrolling levels, your grade for the entire project will be 0. 

This is a platformer game, which means it needs to have platforms.

Requirement 3: 3 Lives (20%)
The player gets 3 lives for the entire game (not per level).

If the player runs out of lives, show a “You Lose” text. 

If the player gets to the end of your game, show text a “You Win” text. 

In either case, the game must go back to the main menu scene and reset the lives.

Requirement 4: AI (20%)
At least 3 types of moving AI (i.e. wanderer, follower, flyer, etc.) 

If the player touches the AI, the player will 1 life and restart the current level.

Each of your levels must have at least 1 AI.

Requirement 5: Audio (10%)
You must have at least 1 looping background music. 

There should be at least 3 sound effects (such as for jumping, when the player or AI dies, etc.)

Tips
Try checking collisions from the player's perspective. In other words, check if the player collided with the enemy, not if the enemy collided with the player. You will need to add code to Entity.h and Entity.cpp to make this work. 

Fully program one level first. Once you have that, making the subsequent levels should be simpler.

Extra Credit
Make a fourth level that contains a boss fight. The mechanics of this boss fight are completely up to you, but they must naturally be different from how you defeat enemies in the previous levels.
