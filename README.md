# Sanket
# Ronan's Adventure Game

**A text-based adventure game implemented in C++ using graphics.h and WinAPI for sound.**

---

## Overview

In this game, you follow the story of Ronan, Canon, and Camilla through various branching paths where your choices shape the outcome. Battle enemies, choose your weapons and magic, and decide the fate of the world!

---

## Features

- Text-based adventure with typing animation
- Choice-based branching storyline with multiple endings
- Simple 2D graphics display with images per scene
- Background music and sound effects using WinAPI (`PlaySoundA`)
- Keyboard navigation for menus and choices

---

## Controls

- **Arrow keys (Left/Right/Up/Down)** to navigate choices or menu
- **Enter key** to confirm selections
- **Space key** during text typing to skip animation and show full text instantly

---

## File Structure

- **`*.txt`** files: Scene descriptions (one line each)
- **`*.jpeg`** files: Scene background images
- **`*.wav`** files: Scene audio effects or background music
- **`Game.exe`**: Compiled executable (included here)
- **`source.cpp`**: Source code file (this repository)

Make sure all media files are in the same directory as the executable to run correctly.

---

## How to Compile

Requires:
- A C++ compiler (e.g., `g++` from MinGW)
- `graphics.h` library (e.g., WinBGIm)
- Windows OS (uses Windows-specific APIs like `PlaySoundA`)
- task.json file with all `graphics.h` library linker file (included in the repository)

Compile and build with following shortcut keys:
Ctrl + Shift + b

Start with the following command:
```bash
Start Game.exe
