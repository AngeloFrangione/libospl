# How to contribute to OSPL
------------------------------------------------------
First of all, thanks for being interested in contributing !  
Let's help us to make this photo library the best open source solution !

## Main guidelines

* You should not go too fast  
  We prepared milestones with progressive goals, and we prefer that everybody focuses on the current milestone.
* Please follow the [NORM.md](https://github.com/AngeloFrangione/libospl/blob/master/NORM.md)
  We are following a code norm that you must follow in order to get your contribution accepted.  
  Our norm is not very restrictive and aims to have a code that is coherent and readable to other contributors.
* Your commit messages should be clear.  
  We ask you to make clear and exhaustive commit messages.  
  You should choose a clear title and then have a line for each change you made.
  Each line should be prefixed with the type of change you made like this : `[ADD]`, `[FIX]`, ...
* We use git flow. Take a look at [this](https://medium.com/@muneebsajjad/git-flow-explained-quick-and-simple-7a753313572f) article about git-flow
* For build instructions see [BUILD.md](https://github.com/AngeloFrangione/libospl/blob/master/BUILD.md)
* We have a documentation that explains most of the functions behaviors [here](https://angelofrangione.github.io/libospl/reference)
### Subprojects
There are multiple modules in the subproject folder, for now, there is the thumbnailer module, and the interface between sqlite (stockage) module. Theses two meant to be upgradable without modifying the main code of the library. For example, the thumbnailer has a generic thumbnail creation function that determines the image format and create automaticly the thumbnail. So adding a supported format shouldn't affect libospl

## DB structure
Images are worth a million words so here is a diagram for the .db file.
You can also look [here](https://dbdiagram.io/d/5ec97ad039d18f5553ffa8cf) for a maintained version 
There is also a .sql file at the [root directory](https://github.com/AngeloFrangione/libospl/blob/master/database.sql)
![database structure](https://raw.githubusercontent.com/AngeloFrangione/libospl/master/database_diagram.png)

## Milestones

You can look at the current milestones [here](https://github.com/AngeloFrangione/libospl/milestones)

## Big ideas
* For all awesome ideas look at [IDEAS.md](https://github.com/AngeloFrangione/libospl/blob/master/IDEAS.md) 

## External libraries

We plan to use following external libraries for making all this working:

- [cwalk](https://github.com/likle/cwalk/tree/master) for managing paths on windows and unix
- [sqlite](https://www.sqlite.org/index.html) for storing data
- [libexif](https://libexif.github.io/) for extracting exif informations
- [epeg](https://github.com/mattes/epeg) for fast jpeg thumbnail creation (wich uses libjpeg and libexif)
- [pngscale](https://github.com/dcoetzee/pngscale) for png thumbnail creation
- [libimobiledevice](https://www.libimobiledevice.org/) for importing photos from iDevices
- [libraw](https://www.libraw.org/) for reading raw photos from cameras
