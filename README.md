# Elastic Collision Simulator

This project requires the **Raylib** library to run. Follow the instructions below to install the dependencies, compile, and execute the project.

## Required Dependencies

- **Raylib**: Main library for rendering and window management.

## Installation

### Arch Linux

To install Raylib on Arch Linux, run the following command:

```bash
sudo pacman -S raylib
```

## Compilation

To compile the code, run the following command:

```bash
make
```

## Execution

To run the simulator, you must provide the width, height, and number of particles as parameters:

```bash
./colision [width] [height] [num particles]
```

**Usage example:**
```bash
./colision 800 600 10
```
