# Math Game Effects

A collection of classic retro visual effects implemented in pure C using SDL3.  
All effects are generated with simple mathematics — no shaders, no textures, just code.

Inspired by old-school games and demoscene visuals from the DOS / Amiga era.

Included effects:

- Fire
- Tunnel
- Starfield
- Plasma
- Metaballs
- Water ripple
- Rain
- Orbit
- Bounce
- Mode 7
- RotoZoom
- Value / Perlin-like Noise  
- And more…

Each effect lives in its own folder and builds into a standalone executable.

## Requirements

- SDL3 development libraries
- A C compiler (clang or gcc)

On macOS:

```bash
brew install sdl3
```

On Linux (Debian/Ubuntu):

```bash
sudo apt install libsdl3-dev
```

## Build

Compile all effects at once:

```bash
make
```

This will enter each subfolder and build the corresponding demo.

## Run

Each effect produces a `main` binary inside its folder:

```bash
./rain/main
./fire_math/main
./tunnel_effect/main
./star_field/main
./plasma/main
# etc...
```

![Jan-15-2026 10-52-09](https://github.com/user-attachments/assets/1d2ce83e-df63-4097-8bcc-cdbbd6ca4c42)


Press **ESC** to exit any demo.
