//
// Created by tumap on 10/21/22.
//
#include "instruction-decoder.h"
#include "instruction-trace.h"
#include "wa-utils.h"


WASM_INSTRUCTION(nop)
    // do nothing
    return true;
}

WASM_INSTRUCTION(unreachable)
    // invalid code
    return true;
}

WASM_INSTRUCTION(end)
    // reset context
    ctx->frame_stack.top = 0;
    ctx->value_stack.top = 0;
    ctx->current_frame = NULL;

    return true;
}

WASM_INSTRUCTION(return)
    // check value stack
    if (ctx->current_frame->value_stack.top < ctx->current_frame->locals_count) {
        TRACE("Value stack empty")
        return false;
    }

    // drop frame & locals
    ctx->frame_stack.top--;
    ctx->current_frame = ctx->current_frame->caller_frame;

    // sanity checks
    if (ctx->current_frame == NULL) {
        if (ctx->frame_stack.top != 0) {
            TRACE("Frame stack is not empty")
            return false;
        }
        if (ctx->value_stack.top != 0) {
            TRACE("Operand stack is not empty")
            return false;
        }
    }

    TRACE("Leaving function")

    return true;
}

WASM_INSTRUCTION(call)
    // get function index
    uint32_t index;
    if (!wasm_instruction_get_u32(ctx, &index)) {
        TRACE("Unable to extract instruction parameter")
        return false;
    }

    // find function
    const tWasm_function *function = _wasm_find_function(ctx, index);
    if (!function) {
        TRACE("Function not found (id=%d)", index)
        return false;
    }
    TRACE("Calling function (name=%s)", function->name.name)

    // imported function
    if (function->imported) {
        if (!function->import.binding) {
            TRACE("Function not bound (name=%s)", function->name.name)
            return false;
        }

        // transfer parameters
        tWasm_frame *cf = ctx->current_frame;
        tWasm_function_call *call_ctx = &function->import.binding->call_ctx;
        int i;
        for (i = (int)call_ctx->param_count-1; i >= 0; i--) {
            cf->value_stack.top--;
            cf->value_stack.size++;
            tWasm_value *from = cf->value_stack.base + cf->value_stack.top;
            tWasm_binding_value *to = call_ctx->params + i;
            switch (from->type) {
                case WASM_NULL:
                    break;
                case WASM_I32:
                    switch (to->type) {
                        case WASM_BINDING_TYPE_NONE:
                            break;
                        case WASM_BINDING_TYPE_U32:
                            *(to->u32.value) = from->i32;
                            break;
                        default:
                            return false;
                    }
                    break;
                default:
                    return false;
            }
        }

        // call function
        if (!function->import.binding->routine(call_ctx)) {
            return false;
        }

        // TODO: transfer result

        return true;
    }


    // TODO: call function


    return true;
}



