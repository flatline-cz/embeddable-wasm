//
// Created by tumap on 10/24/22.
//
#include "interpreter.h"
#include "mem-manager.h"
#include "instruction-decoder.h"
#include "instruction-trace.h"
#include "utils.h"
#include <stdio.h>

#ifndef WASM_VALUE_STACK_SIZE
#define WASM_VALUE_STACK_SIZE        256
#endif
#ifndef WASM_FRAME_STACK_SIZE
#define WASM_FRAME_STACK_SIZE         64
#endif


bool wasm_interpreter_initialize(tWasm_context *const ctx) {
    // allocate stacks
    ctx->value_stack.size = WASM_VALUE_STACK_SIZE;
    ctx->value_stack.top = 0;
    ctx->value_stack.base = wasm_memory_system_alloc(
            &ctx->memory,
            WASM_VALUE_STACK_SIZE * sizeof(tWasm_value));
    if (!ctx->value_stack.base)
        return false;
    ctx->frame_stack.size = WASM_FRAME_STACK_SIZE;
    ctx->frame_stack.top = 0;
    ctx->frame_stack.base = wasm_memory_system_alloc(
            &ctx->memory,
            WASM_FRAME_STACK_SIZE * sizeof(tWasm_stack_item));
    if (!ctx->frame_stack.base)
        return false;

    // initialize heap
    if (!wasm_memory_initialize_heap(&ctx->memory))
        return false;

    // no active frame
    ctx->current_frame = NULL;

    return true;
}

bool wasm_execution_in_progress(const tWasm_context *const ctx) {
    return ctx->current_frame != NULL;
}

bool wasm_interpreter_do_work(tWasm_context *const ctx) {
    if (ctx->current_frame == NULL)
        return true;

    // get instruction code
    if (ctx->current_frame->instruction_counter >= ctx->current_frame->function->implementation.code_length) {
        TRACE("Out of code")
        return false;
    }
    uint8_t code = ctx->current_frame->function->implementation.code[ctx->current_frame->instruction_counter++];

    // decode instruction
    rWasm_instruction_executor instruction = wasm_instruction_decoding_table[code];
    if (instruction == NULL) {
        TRACE("Unsupported instruction 0x%02X", code)
        return false;
    }

    // execute instruction
    if (!instruction(ctx)) {
        TRACE("Trap encountered, execution terminated (pc=0x%04X, code=0x%02X)",
              ctx->current_frame->instruction_counter, code)
        return false;
    }

    return true;
}



//bool wasm_interpreter_call_function(tWasm_context *const ctx, const uint16_t function_index) {
//    // interpreter idle?
//    if (ctx->current_frame != NULL) {
//        TRACE("Can't call function, execution in progress")
//        return false;
//    }
//
//    // find function
//    const tWasm_function *function = _wasm_find_function(ctx, function_index);
//    if (!function) {
//        TRACE("Function (id=%d) not found", function_index)
//        return false;
//    }
//
//    // initialize frame
//    ctx->current_frame = &ctx->frame_stack.base->frame;
//    ctx->frame_stack.top++;
//    ctx->current_frame->caller_frame = NULL;
//    ctx->current_frame->locals_count = 0;  // FIXME
//    ctx->current_frame->value_stack.base = ctx->value_stack.base;
//    ctx->current_frame->value_stack.size = ctx->value_stack.size - ctx->value_stack.top;
//    ctx->current_frame->value_stack.top = 0;
//    ctx->current_frame->function = function;
//    ctx->current_frame->instruction_counter = 0;
//
//    return true;
//}
