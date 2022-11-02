//
// Created by tumap on 10/24/22.
//
#include "instruction-decoder.h"
#include "instruction-trace.h"

WASM_INSTRUCTION(and_i32)
    tWasm_value_stack *stack = &ctx->current_frame->value_stack;

    // check stack size
    if (stack->top < 2) {
        TRACE("Operand stack is empty")
        return false;
    }

    // check operand types
    if (stack->base[stack->top - 1].type != WASM_I32 || stack->base[stack->top - 2].type != WASM_I32) {
        TRACE("Invalid operands on stack")
        return false;
    }

    // operation itself
    stack->base[stack->top - 2].i32 &= stack->base[stack->top - 1].i32;

    // remove one operand from stack
    stack->top--;

    return true;
}

WASM_INSTRUCTION(or_i32)
    tWasm_value_stack *stack = &ctx->current_frame->value_stack;

    // check stack size
    if (stack->top < 2) {
        TRACE("Operand stack is empty")
        return false;
    }

    // check operand types
    if (stack->base[stack->top - 1].type != WASM_I32 || stack->base[stack->top - 2].type != WASM_I32) {
        TRACE("Invalid operands on stack")
        return false;
    }

    // operation itself
    stack->base[stack->top - 2].i32 |= stack->base[stack->top - 1].i32;

    // remove one operand from stack
    stack->top--;

    return true;
}

WASM_INSTRUCTION(xor_i32)
    tWasm_value_stack *stack = &ctx->current_frame->value_stack;

    // check stack size
    if (stack->top < 2) {
        TRACE("Operand stack is empty")
        return false;
    }

    // check operand types
    if (stack->base[stack->top - 1].type != WASM_I32 || stack->base[stack->top - 2].type != WASM_I32) {
        TRACE("Invalid operands on stack")
        return false;
    }

    // operation itself
    stack->base[stack->top - 2].i32 ^= stack->base[stack->top - 1].i32;

    // remove one operand from stack
    stack->top--;

    return true;
}
