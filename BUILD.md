# BUILD.md
---------------------------------------------------
## macOS 
###### Dependencies
Before continuing, assure that these packages are installed (via brew or macports):

- libjpeg
- libsqlite3
- libmagic
- libexif

###### Building
the following command will build ospl:   
`meson . build && cd build && ninja`

## Linux
###### Dependencies
Before continuing, assure that these packages are installed:
- libjpeg-dev
- libsqlite3-dev
- libmagic-dev
- libexif-dev

###### Building
the following command will build ospl:   
`meson . build && cd build && ninja`

## Windows
Comming later