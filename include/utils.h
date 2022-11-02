//
// Created by tumap on 10/21/22.
//

#ifndef WASM_TEST_UTILS_H
#define WASM_TEST_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include "wa-types.h"

bool _wasm_extract_u32(const uint8_t *data, uint16_t data_length,
                       uint16_t *consumed, uint32_t *decoded);

const tWasm_function *_wasm_find_function(const tWasm_context *ctx, uint16_t function_index);

#endif //WASM_TEST_UTILS_H
