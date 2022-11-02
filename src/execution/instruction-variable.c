//
// Created by tumap on 10/21/22.
//
#include <string.h>
#include "instruction-decoder.h"
#include "instruction-trace.h"

WASM_INSTRUCTION(local_get)
    tWasm_frame *cf = ctx->current_frame;

    // extract local index
    uint32_t index;
    if (!wasm_instruction_get_u32(ctx, &index)) {
        TRACE("Unable to extract instruction parameter")
        return false;
    }

    // push value to the stack
    if (cf->value_stack.top == cf->value_stack.size) {
        TRACE("Operand stack is full")
        return false;
    }
    tWasm_value *top = cf->value_stack.base + cf->value_stack.top++;

    // copy value
    memcpy(top, cf->value_stack.base + index, sizeof(tWasm_value));

    return true;
}

WASM_INSTRUCTION(local_set)
    tWasm_frame *cf = ctx->current_frame;

    // extract local index
    uint32_t index;
    if (!wasm_instruction_get_u32(ctx, &index)) {
        TRACE("Unable to extract instruction parameter")
        return false;
    }

    // get value from stack
    if (cf->value_stack.top == 0) {
        // unable to pop value from stack
        TRACE("Operand stack is empty")
        return false;
    }
    tWasm_value *value = cf->value_stack.base + (--cf->value_stack.top);

    // copy value
    memcpy(cf->value_stack.base + index, value, sizeof(tWasm_value));

    return true;
}

WASM_INSTRUCTION(local_tee)
    // TODO: implement
    return false;
}

WASM_INSTRUCTION(global_get)
    // TODO: implement
    return false;
}

WASM_INSTRUCTION(global_set)
    // TODO: implement
    return false;
}
