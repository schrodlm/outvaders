![outvader logo](gfx/enemy3_1.png)

# Outvaders

Project done as a part of the interview proccess at Warhorse Studios. I was given a broken [Space Invaders](https://en.wikipedia.org/wiki/Space_Invaders) game with unfinished library in C++ using `Windows`, `Direct3D` and `FMOD` libraries. My job was to make the game playable, document the proccess and to create overall clean application from the starting point I was given.

## Added features
- Collision detection
- New Sprites and textures
- Utility function for the library
- Entities
- Highscores
- Menus
- And a lot  more

## Documentation 

This project includes Doxygen config file, to create documentation navigate to projects folder and run `doxygen Doxyfile` this will generate HTML documentation, then navigate to `index.html` and open it

## What do I want to implement next

If I will find a time throughtout my next semester I would like to add sound, two players mode and finish clearing out memory leaks and bad practices.

## Debugging and some bad practices
I left CRT debugger libraries and code in source code so you can clearly see some memory leaks I didn't manage to sort out, they don't affect the game in any way, but clearly leaving them in is a very bad practice, another bad practice I didn't have time to sort out are enum return values, in some handling function I am returning simply an integer, which works great, but sadly isn't that readable. I wanted to add enums, but again, time...


## Getting Started

### Prerequisites
- Visual Studio 2008 or higher

### Installation

## Running the app
1. Clone the repository: git clone https://github.com/schrodlm/outvaders.git
2. Navigate to the server directory: `cd outvaders`
3. Open `outvaders.sln` Visual Studio solution
4. Build the application withing the Visual Studio

## Built With
- [Visual Studio](https://visualstudio.microsoft.com/) - IDE FOR Windows software development
- [Direct3D](https://learn.microsoft.com/en-us/windows/win32/direct3d) - Direct3D is a graphics application programming interface

## Authors
- [Matěj Schrödl](https://github.com/schrodlm)

