# Cellular automaton

My take on implementing some cellular automatons:
- Wolfram codes
- Falling sand
- Game of life
- Langton's ant

## Build and run

```bash
# build and run
make
# build
make build
# run
make run

# run with binary
./build/automaton
./build/automaton --help-all

./build/automaton -e wolfram --rule 110 --cell-width 6
./build/automaton -e sand --pause 50 --animation
./build/automaton -e life --rule B3/S12345
./build/automaton -e ant --pause 3 --cell-width 5
```

## Debug

GTK's `g_debug()` is used for debugging. To enable debug logs use env variable:

```bash
# with binary
G_MESSAGES_DEBUG=all ./build/automaton
```
