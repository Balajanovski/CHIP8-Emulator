# CHIP8-Emulator
A simple CHIP8 emulator created to learn more about emulation<br>
Modeled as close as possible to the Chip8 architecture, including using every single individual bit for the vga memory unlike other emulators which use a byte for each pixel.<br>
Criticism is appreciated.<br>
[![Screen_Shot_2017-12-27_at_9.19.32_AM.png](https://s10.postimg.org/wa05vpbpl/Screen_Shot_2017-12-27_at_9.19.32_AM.png)](https://postimg.org/image/wmrk1vtz9/)

## Dependencies
* SDL2
* CMake

## Usage
1. Clone the repository by typing `git clone https://github.com/Balajanovski/CHIP8-Emulator.git` into the command line
2. Enter the repository by typing `cd CHIP8-Emulator`
3. Create the Makefile by typing `cmake .`
4. Build the project by typing `make`
5. Run the project by typing `./CHIP8-Emulator [directory_of_ROM]` where you replace [directory_of_rom] with the directory of your CHIP8 ROM.
