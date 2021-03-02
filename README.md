# Cellular automaton

## Build

```bash
cmake -S . -B build
cmake --build build -- -j
./build/automaton
```

## Debug

GTK's `g_debug()` is used for debugging. To enable debug logs use env variable:

```bash
G_MESSAGES_DEBUG=all ./build/automaton
```
