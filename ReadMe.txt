1. How to build the project

- Please set "FPSTraining" project as a starter project and build with x64 configuration / Debug or Release mode.
  Or you can execute with "FPSTraining.exe" file in x64\Debug(or Release) folder.

- GitHub Link:
https://github.com/kanious/Project03_SimpleFPS



2. User Input Option

 < For Player(Camera) >
 * WASD : Move Player (look/right direction).
 * Drag Mouse : Rotate screen or aim target.
 * Click Mouse : Shot target.

 < Etc >
 * F1 : Switch game mode (Shot <-> Hover).
 * ESC : Close the program.



3. Simple Description

- You can check the game information on the top left of the screen.
  Game Mode: Shot or Hover (I reduced the hover time to 2.5s from 5s. 5s was too long for me)
  Killed Targets: Total number of targets killed (from all mods)
  Fired/Missed/Accuracy rate: Number of shots/misses/accuracy rate

- When aiming at a target in hover mode, the remaining time until destruction is displayed at the top of the crosshair.
  You can check this in sample_gameplay.png file.

- A total of 10 targets exist in the arena.
  If caught by a player, they teleport to a random location.
  The target moves in a random direction every random amount of time.
  It pauses briefly when changing direction, then speeds up.
  Acceleration continues to accumulate until it changes direction again or reaches maximum speed.
  If the target hits a wall while moving, it stops and gets another random direction.

- The firework effect is implemented using 20 objects at a time.
  The Particle System pre-creates 60 objects and recycles them each time.


