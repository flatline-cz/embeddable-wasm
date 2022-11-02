//
// Created by tumap on 10/21/22.
//

#ifndef WASM_TEST_INTERPRETER_H
#define WASM_TEST_INTERPRETER_H

#include "wa-types.h"

bool wasm_execution_in_progress(const tWasm_context *ctx);

bool wasm_interpreter_do_work(tWasm_context *ctx);

bool wasm_interpreter_call_function(tWasm_context *ctx, uint16_t function_index);

bool wasm_interpreter_initialize(tWasm_context *ctx);


#endif //WASM_TEST_INTERPRETER_H
