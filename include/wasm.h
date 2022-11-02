//
// Created by tumap on 9/23/22.
//

#ifndef WASM_WASM_H
#define WASM_WASM_H

#include <stdbool.h>
#include <stdint.h>


void wasm_initialize(void* memory, uint32_t length);

bool wasm_load(const void* buffer, uint16_t length);



#endif //WASM_WASM_H
