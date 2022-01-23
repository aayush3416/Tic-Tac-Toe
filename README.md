# Introduction
This project was created and by my group for my Digital Computation class. This game will consists of a physical Robot that we built playing against a user.
We have programmed the EV3 Mindstorms Robot to play a game of Tic-Tac-Toe. The EV3 faces off against a human player on the physical board using red and green tiles instead of x’s and o’s. The board, with some tiles in place, is shown below.

![board_layout](https://user-images.githubusercontent.com/87783633/150659338-4142e1be-146c-412e-90fa-c1e2486e2501.jpg)

# How it Works?
The game is broken down into steps as follows. First, the robot prompts the user for the difficulty of the game, which determines how much time the player gets to play each turn. Easy mode will give the user 20 seconds to place a tile on the board. Medium and hard give the player less and less time to play their turn so they are forced to make quick decisions. Now that the game is set up, the player presses the center button to begin their turn. After the player places a tile, the robot scans the board to determine whether the game has ended or not, and to determine which slots on the board are empty. The robot uses this data to decide which empty tile to drop a tile into using the dispenser. This process repeats until either the robot or player has won the game, the game has been tied or the player has been caught cheating. Once the game has been concluded, the robot will display the game outcome and prompt the user to press the center button to terminate the program.

