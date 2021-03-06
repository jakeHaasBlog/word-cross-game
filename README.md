# word-cross-game
A simple word fill game

User guide:
The objective of this game is to fit all of the words presented on the left side of the screen into the spaces provided on the game board. Words on the game board are read from left to right or top to bottom. There is guaranteed to be at least one correct solution to any generated puzzle. To play the game the user must hover the mouse pointer over a blank space and type a letter to fill in the selected space. The user can also hover the mouse over a tile and press backspace to clear that tile. When words are written on the board, they are automatically crossed off the words list on the left side of the screen. When all of the words have been crossed out, a winning animation will be played, this animation can be closed and reopened by clicking anywhere on the screen.

When the program is launched, a simple initial game board is presented to the user. The user can then play this game while the next boards are generated, three progress bars located at the bottom of the screen show the generation progress of each new board. Once a new board is fully generated the user can press the button directly to the right of the progress bar to begin a new game using the corrosponding board. The board buttons are labled 'easy', 'medium', and 'hard' based on the number of calculations required to generate the corrosponding board.


-- Programming Techniques Demonstrated --

Asynchronous programming using modern threads in c++11
- The reserve game boards are generated on slave threads in the background while the user plays the current board, this is done using the c++11 thread class provided in the standard library.

Smart pointers
- The active and background game boards are tracked using shared pointers provided by the standard template library to ensure safety from dangling pointers and memory leaks while enabling the active board to be easily swapped with the reserve board
- The slave threads are pointed to using unique pointers to ensure safety from memory leaks upon deletion and re-initialization.

Usage of the OpenGL API
- All graphical aspects of this program were accomplished using modern OpenGL with custom shaders and a variety of c-style buffer object handles.

Usage of static and dynamic libraries within Microsoft's Visial Studio 2019 IDE
- The GLFW library is linked statically and used to construct a cross platform window and input handles.
- The GLEW library is linked statically and used to gather the OpenGL extensions supported by the current computer system and provide prototypes to them.
- The YSE library is linked dynamically and is used for loading and playing sound effects.
- The STBImage library is linked statically to decode a variety of image files such as .png and .jpg files into a format to be used by OpenGL
