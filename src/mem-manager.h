//
// Created by tumap on 9/23/22.
//

#ifndef WASM_MEMORY_H
#define WASM_MEMORY_H

#include <stdint.h>
#include "wa-types.h"

void wasm_memory_initialize(tWasm_memory *ctx, uint8_t *memory_block, tWasm_mem_size size);

void* wasm_memory_system_alloc(tWasm_memory* ctx, tWasm_mem_size size);

bool wasm_memory_initialize_heap(tWasm_memory* ctx);



#endif //WASM_MEMORY_H
