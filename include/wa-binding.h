//
// Created by tumap on 10/25/22.
//

#ifndef WASM_TEST_WA_BINDING_H
#define WASM_TEST_WA_BINDING_H

#include <stdbool.h>
#include <stdint.h>
#include "wa-types.h"

bool wasm_binding_bind_function(
        tWasm_context *ctx,
        tWasm_binding* binding);

bool wasm_binding_call_function(
        tWasm_context *ctx,
        tWasm_function_call *call_ctx);

#endif //WASM_TEST_WA_BINDING_H
