//
// Created by tumap on 9/23/22.
//

#ifndef WASM_TEST_FORMAT_H
#define WASM_TEST_FORMAT_H

#include <stdbool.h>
#include <stdint.h>
#include "wa-types.h"

bool wasm_format_load(tWasm_context* ctx, const uint8_t* buffer, uint16_t length);


#endif //WASM_TEST_FORMAT_H
