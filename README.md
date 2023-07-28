# SnakeGame

## Introduction

Welcome to our GitHub repository for the SnakeGame project! We've utilized a Nucleo microcontroller, an 8x8 LED matrix with an integrated Max7219 LED display driver, and an omnidirectional joystick. With the Mbed API, we've crafted an engaging SnakeGame that you can control using the joystick, displayed on the LED matrix. Guide the Snake to consume fruits, avoiding collisions and increasing your score, while enjoying sound effects through the piezo speaker. Get ready for a fun and challenging gaming experience! 🐍🎮

## Components Needed

- Nucleo-F401RE Microcontroller
- Breadboard
- 8x8 LED matrix for displaying the game
- Analog joystick for user control
- Piezo speaker for game sounds

## Circuit Design

![image](https://github.com/BenTheurich/EmbeddedSnakeGame/assets/81211973/70f471eb-55f2-447b-a883-f0a14a4b8995)


## Implementation

For the Snake object, we used a linked list to track each body part. The Snake moves by adding a node to the head and removing the tail. When the Snake eats a fruit, it moves forward without removing the tail. We found that using a linked list was the best way to keep track of the Snake's position.

The Game object uses a 2D array to represent the board imposed on the 8x8 Matrix. Each row is represented by 0s for LEDs that should be turned off and 1s for LEDs that should be turned on. This allows us to create a binary number based on each row and update the LED matrix accordingly.

## Results

This is the final result of our project. In addition to what we originally planned to create, we were able to add nice-to-have features, such as animations, game sounds, and a decorative board for our components to attach to.

![image](https://github.com/BenTheurich/EmbeddedSnakeGame/assets/81211973/b3dbb8cf-3422-463b-bfcd-d18be172d914)

Link for video demonstration:
https://drive.google.com/file/d/1f-Nvek4yxz_EDnvyU3LeZlxIyDfSh_NV/view

## Usage

1. Connect the components as shown in the circuit diagram.
2. Compile and upload the code to the Nucleo-F401RE Microcontroller.
3. Once the matrix displays the starting pattern, press "select" on the joystick to start the game.
4. Use the joystick to control the direction of the Snake.
5. Guide the Snake to consume fruits and increase your score.
6. Avoid collisions with the Snake's body and the edge of the game board.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- The inspiration for this project came from a video of a pong game using similar components.
- We referred to various implementations of the SnakeGame using an 8x8 matrix for ideas and references.

## Team Members

- Cody McKinney
- Carter Rath
- Ben Theurich


