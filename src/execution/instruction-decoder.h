//
// Created by tumap on 10/21/22.
//

#ifndef WASM_TEST_INSTRUCTION_DECODER_H
#define WASM_TEST_INSTRUCTION_DECODER_H

#include <stdbool.h>
#include "wa-types.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef bool (*rWasm_instruction_executor)(tWasm_context* ctx);

extern const rWasm_instruction_executor wasm_instruction_decoding_table[256];

bool wasm_instruction_get_u32(tWasm_context* ctx, uint32_t* value);

#endif //WASM_TEST_INSTRUCTION_DECODER_H
