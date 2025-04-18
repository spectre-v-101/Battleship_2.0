# Battleship_2.0
Full repo containing source code of Battleship_2.0 game.
READ ME
Terminal Battleship Game with Smart AI & Special Torpedo
> A feature-rich Battleship game written entirely in C, with animated ASCII radar effects,
intelligent AI opponent, and a powerful special torpedo attack system. Built to run directly in the
terminal with no external libraries.
> Author:
- Ved Chattopadhyay
---
Files to Download:
Extract all the files to a folder.
After extracting, open that folder and click on the source code to run on VS code or directly click
on the executable file to run on Terminal.
Make sure you have all the following files in the **same folder** before compiling:
- `<filename>.c` — Main source code file
containing the full implementation.
- ‘nano.wav’ - WAV file for playing in game music
---
## How to Compile and Run (on VS code)
### Using GCC (Linux/macOS/WSL):
Open the source code file on vs code and click on the run code option .
After this 1 line of error will pop up in the vs code terminal stating:
‘ undefined reference to PlaySoundA@12'’ – this is happening because the compiler doesn't
know that we want to play music
IGNORE this error and in the next line write the following command to manually run the code:
Run the following 2  commands in vs code terminal.
gcc `<filename>`.c -o `<filename>`.exe -lwinmm
./`<filename>`.exe
### To run on System Terminal:
Locate the executable file and just click on it to run the program on the terminal.
### How to Play:
After opening press any key to proceed to the main menu , and press any key again to start
playing.
