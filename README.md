# Card Roulette
 
A console-based card game using my engine. Showcasing Unicode in the Windows console and generating Unicode cards from imported data. WIP
 
## Prerequisites
 
- VS 2022
- Written for C++23

## Generating the Solution
 
Run `GenerateProject.bat` from the root of the repo. This will generate the project files.
 
## Building
 
Open `CardRoulette.sln` in Visual Studio, select a configuration (`Debug`, `Development`, or `Shipping`), and build.

- `Debug` - No optimization (/MTd, /ZI, _DEBUG)
- `Development` - Partial optimization (/MT, /Zi, NDEBUG)
- `Shipping` - Full optimization (/GL, /MT, /Zi, NDEBUG)
 
## Cleaning
 
Run `CleanProject.bat` to remove all generated project and build output files.
