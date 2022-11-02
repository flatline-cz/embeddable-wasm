//
// Created by tumap on 10/24/22.
//
#include <memory.h>
#include "instruction-decoder.h"
#include "instruction-trace.h"

WASM_INSTRUCTION(drop)
    // check stack
    if (ctx->value_stack.top == 0) {
        TRACE("Empty operand stack")
        return false;
    }

    // drop value
    ctx->value_stack.top--;

    return true;
}


WASM_INSTRUCTION(select)
    // check stack
    if (ctx->value_stack.top < 3) {
        TRACE("Empty operand stack")
        return false;
    }

    // check value
    if (ctx->value_stack.base[ctx->value_stack.top - 1].type != WASM_I32) {
        TRACE("Invalid operand on stack")
        return false;
    }

    // move element on stack
    if (ctx->value_stack.base[ctx->value_stack.top - 1].i32 == 0) {
        memcpy(ctx->value_stack.base + ctx->value_stack.top - 2,
               ctx->value_stack.base + ctx->value_stack.top - 1,
               sizeof(tWasm_value));
    }

    // update stack top
    ctx->value_stack.top-=2;

    return true;
}
