# Cellular automaton

My take on implementing some cellular automatons:
- Game of life
- Falling sand
- Wolfram codes

## Build and run

```bash
# build and run
make

# build with make
make build

# build with cmake
cmake -S . -B build
cmake --build build -- -j

# run with make
make run

# run with binary
./build/automaton
./build/automaton --help-all

# examples
./build/automaton -e wolfram --wf-code 110 --cell-width 6
./build/automaton -e sand --delay 50 --animation
./build/automaton -e life
```

## Debug

GTK's `g_debug()` is used for debugging. To enable debug logs use env variable:

```bash
# with binary
G_MESSAGES_DEBUG=all ./build/automaton
```
