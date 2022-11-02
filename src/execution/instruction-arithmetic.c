//
// Created by tumap on 10/28/22.
//
#include "instruction-decoder.h"
#include "instruction-trace.h"

WASM_INSTRUCTION(clz)
// TODO: implement
    return false;
}

WASM_INSTRUCTION(ctz)
// TODO: implement
    return false;
}

WASM_INSTRUCTION(popcnt)
// TODO: implement
    return false;
}

WASM_INSTRUCTION(add_i32)
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
    stack->base[stack->top - 2].i32 += stack->base[stack->top - 1].i32;

    // remove one operand from stack
    stack->top--;

    return true;
}

WASM_INSTRUCTION(sub_i32)
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
    stack->base[stack->top - 2].i32 -= stack->base[stack->top - 1].i32;

    // remove one operand from stack
    stack->top--;

    return true;
}

WASM_INSTRUCTION(mul_i32)
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
    stack->base[stack->top - 2].i32 *= stack->base[stack->top - 1].i32;

    // remove one operand from stack
    stack->top--;

    return true;
}

WASM_INSTRUCTION(div_s_i32)
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
    stack->base[stack->top - 2].i32 /= stack->base[stack->top - 1].i32;

    // remove one operand from stack
    stack->top--;

    return true;
}

WASM_INSTRUCTION(div_u_i32)
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
    stack->base[stack->top - 2].i32 = (int32_t) (
            ((uint32_t) stack->base[stack->top - 2].i32) /
            ((uint32_t) stack->base[stack->top - 1].i32)
            );

    // remove one operand from stack
    stack->top--;

    return true;
}

WASM_INSTRUCTION(rem_s_i32)
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
    stack->base[stack->top - 2].i32 %= stack->base[stack->top - 2].i32;

    // remove one operand from stack
    stack->top--;

    return true;
}

WASM_INSTRUCTION(rem_u_i32)
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
    stack->base[stack->top - 2].i32 = (int32_t) (
            ((uint32_t) stack->base[stack->top - 2].i32) %
            ((uint32_t) stack->base[stack->top - 1].i32)
    );

    // remove one operand from stack
    stack->top--;

    return true;
}
