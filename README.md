# tModManager

The goal of this application is to provide an easy to use interface for managing your Terraria mods as well as launching the game all from one location.

On top of this, I strongly believe that we as programmers have a tendency to abuse modern libraries to the point it degrades the performance of our applications in more cases than we'd like to admit.  With this in mind I intend to keep this software as light as possible.  Just the features that are legitamitely useful and no gimmicky stuff.

Sorry for the long delay on the progress of this.  My work picked up pretty heavy so I ran out of time to do anything.

### Planned Features
* Simple graphical user interface that's both easy to navigate as well as intuitive
* Resource viewing from within the GUI.  This is of course going to respect the mod authors preferences.  Mod authors worked hard to give us excellent product so undermining them would be shameful.
* Downloading/updating of mods from within the GUI.
* Possible auto-update/update-notification styled features.

### Build Requirements
| Library | Description |
| ------- | ----------- |
| zlib    | DEFLATE format decompression |
| openssl | SHA1 hash algorithm |
| cmake   | Build system |
| ?       | Graphical User Interface |

### Build Instructions
The steps follow the basic principles of cmake.

#### Linux
cd into the project directory
mkdir build && cd build
cmake ../ && make
In order to test run ./src/tmm

### Final Notes
There's really not much to see here at the moment, the GUI isn't implemented so anything you would want to work isn't going to.  For the moment I'm taking care of a lot of the backend stuff so once I start on the GUI it will be mostly just plugging things in where they belong.

