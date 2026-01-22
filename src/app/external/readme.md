# External Libraries

This folder contains the external libraries used in this project.
These specific versions have been tested and confirmed to work with this project.

## SDL (Simple DirectMedia Layer)

**Version:** 2.30.0  
**Git Repository:** [https://github.com/libsdl-org/SDL.git](https://github.com/libsdl-org/SDL.git)  
**Branch:** SDL2

## SDL_ttf

**Version:** 2.23.0  
**Git Repository:** [https://github.com/libsdl-org/SDL_ttf.git](https://github.com/libsdl-org/SDL_ttf.git)  
**Branch:** SDL2

## Usage

If submodules are missing, git clone can be used to get these repositories.

### For SDL:
```sh
git clone -b SDL2 https://github.com/libsdl-org/SDL.git
```
### For SDL_TTF:
```sh
git clone -b SDL2 --recurse-submodules https://github.com/libsdl-org/SDL_ttf.git
```

## Submodules

Project contains submodules that were originally added using the following commands:

### For SDL:
```sh
git submodule add -b SDL2 https://github.com/libsdl-org/SDL.git external/SDL
```
### For SDL_ttf:
```sh
git submodule add -b SDL2 https://github.com/libsdl-org/SDL_ttf.git external/SDL_ttf
```
### Initialize and Update Submodules:
```sh
git submodule update --init --recursive
```