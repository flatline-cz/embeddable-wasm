//
// Created by tumap on 10/21/22.
//
#include "instruction-decoder.h"
#include "instruction-trace.h"

WASM_INSTRUCTION(const_i32)
    tWasm_frame *cf = ctx->current_frame;

    // decode value
    uint32_t value;
    if(!wasm_instruction_get_u32(ctx, &value)) {
        TRACE("Unable to extract instruction parameter")
        return false;
    }

    // get new stack top
    if(!cf->value_stack.size) {
        return false;
    }
    cf->value_stack.size--;
    tWasm_value *top=cf->value_stack.base+cf->value_stack.top++;

    // put value to the operand stack
    top->type = WASM_I32;
    top->i32 = (int32_t) value;

    return true;
}

WASM_INSTRUCTION(const_i64)
    // TODO: implement
    return false;
}

WASM_INSTRUCTION(const_f32)
    // TODO: implement
    return false;
}

WASM_INSTRUCTION(const_f64)
    // TODO: implement
    return false;
}
