# tModManager

The goal of this application is to provide an easy to use interface for managing your Terraria mods as well as launching the game all from one location.

### Planned Features
* Simple graphical user interface that's both easy to navigate as well as intuitive
* Resource viewing from within the GUI.  This is of course going to respect the mod authors preferences.  Mod authors worked hard to give us excellent product so undermining them would be shameful.
* Downloading/updating of mods from within the GUI.
* Possible auto-update/update-notification styled features.

### Build Requirements
| Library | Description |
| ------- | ----------- |
| crypto++ | DEFLATE and SHA1 algorithms |
| QT       | Cross-platform library for user interface and various other functions |
| cmake    | Build system |

### Build Instructions
The steps follow the basic principles of cmake.

#### Linux
cd into the project directory

mkdir build && cd build

cmake ../ && make

In order to test run ./src/tmm
