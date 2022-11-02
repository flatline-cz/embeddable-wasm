# embedable-wasm
Embedable WebAssembly interpreter

Main features:
1. Limited RAM usage (less than 64KB)
1. Ability to call exported WASM functions from embedder
1. Ability to import embedder functions for script usage
1. Single threaded
1. Plain C, no runtime required
1. Execution is called in a loop, one WASM instruction per one call
