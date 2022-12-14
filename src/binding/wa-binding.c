//
// Created by tumap on 10/25/22.
//
#include <string.h>
#include "wa-binding.h"
#include "../execution/instruction-trace.h"
#include "wa-utils.h"

#if WASM_EXEC_TRACE

#include <stdio.h>

#define TRACE_CALL(f) printf("Function '"); fwrite(f->name.name, 1, f->name.name_length, stdout); printf("' called\n");
#else
#define TRACE_CALL(f)
#endif


static bool check_type_stack2binding(eWasm_value_type from, eWasm_binding_type to);

static bool check_type_binding2stack(eWasm_binding_type from, eWasm_value_type to);

static tWasm_function *find_function_by_name(tWasm_context *ctx,
                                             tWasm_function_call *call_ctx,
                                             bool imported) {

    // scan all functions
    tWasm_function_list_item *item;
    for (item = ctx->functions.function_list; item; item = item->next) {
        // compare type
        if (item->function.imported != imported)
            continue;

        // compare name
        if (strncmp(call_ctx->function.name, item->function.name.name, item->function.name.name_length) != 0)
            continue;

        // compare parameters
        if (call_ctx->param_count != item->function.signature->param_count)
            continue;
        unsigned i;
        for (i = 0; i < call_ctx->param_count; i++) {
            if (!check_type_binding2stack(call_ctx->params[i].type, item->function.signature->params[i]))
                break;
        }
        if (i != call_ctx->param_count)
            continue;

        // compare return type
        if (!check_type_stack2binding(item->function.signature->return_type, call_ctx->return_value.type))
            continue;

        return &item->function;
    }

    return NULL;

}

bool wasm_binding_bind_function(
        tWasm_context *ctx,
        tWasm_binding *binding) {

    // find function
    tWasm_function *function = find_function_by_name(ctx, &binding->call_ctx, true);
    if (function == NULL) {
        // function not imported
        return true;
    }

    if (function->import.binding != NULL) {
        TRACE("Function already bound (name=%s)", binding->call_ctx.function.name)
        return false;
    }

    function->import.binding = binding;

    TRACE("Function bound (name=%s)", binding->call_ctx.function.name)

    return true;
}


bool wasm_binding_call_function(tWasm_context *ctx, tWasm_function_call *call_ctx) {
    // interpreter idle?
    if (ctx->current_frame != NULL) {
        TRACE("Can't call function, execution in progress")
        return false;
    }

    // find function
    const tWasm_function *function = _wasm_find_function(ctx, call_ctx->function.index);
    if (function == NULL) {
        TRACE("Function not found (idx=%d)", call_ctx->function.index);
        return false;
    }
//    TRACE_CALL(function)

    // initialize frame
    ctx->current_frame = &ctx->frame_stack.base->frame;
    ctx->frame_stack.top++;
    ctx->frame_stack.size--;
    ctx->current_frame->caller_frame = NULL;
    ctx->current_frame->locals_count = function->implementation.locals_count;
    ctx->current_frame->value_stack.base = ctx->value_stack.base;
    uint16_t size = ctx->value_stack.size - ctx->value_stack.top;
    uint16_t count = function->implementation.locals_count;
    ctx->current_frame->value_stack.top = count;
    size -= count;
    ctx->current_frame->value_stack.size = size;
    ctx->current_frame->function = function;
    ctx->current_frame->instruction_counter = 0;

    // TODO: copy parameters
    int i;
    for (i = 0; i < call_ctx->param_count; i++) {
        switch (call_ctx->params[i].type) {
            case WASM_BINDING_TYPE_NONE:
                ctx->current_frame->value_stack.base[i].type = WASM_NULL;
                break;
            case WASM_BINDING_TYPE_U32:
                ctx->current_frame->value_stack.base[i].type = WASM_I32;
                ctx->current_frame->value_stack.base[i].i32 = (int32_t) *(call_ctx->params[i].u32.value);
                break;
            default:
                return false;
        }
    }

    return true;

}

static bool check_type_stack2binding(eWasm_value_type from, eWasm_binding_type to) {
    switch (from) {
        case WASM_NULL:
            return to == WASM_BINDING_TYPE_NONE;
        case WASM_I32:
            return to == WASM_BINDING_TYPE_U32 || to == WASM_BINDING_TYPE_BYTE_ARRAY;
        default:
            break;
    }
    return false;
}

static bool check_type_binding2stack(eWasm_binding_type from, eWasm_value_type to) {
    switch (from) {
        case WASM_BINDING_TYPE_NONE:
            return to == WASM_BINDING_TYPE_NONE;
        case WASM_BINDING_TYPE_U32:
        case WASM_BINDING_TYPE_BYTE_ARRAY:
            return to == WASM_I32;
        default:
            break;
    }
    return false;
}
