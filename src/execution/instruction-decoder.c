//
// Created by tumap on 10/21/22.
//

//#include <stdlib.h>
#include "instruction-decoder.h"
#include "instruction-trace.h"
#include "wa-utils.h"

WASM_INSTRUCTION_DECL(const_i32)

WASM_INSTRUCTION_DECL(const_i64)

WASM_INSTRUCTION_DECL(const_f32)

WASM_INSTRUCTION_DECL(const_f64)

WASM_INSTRUCTION_DECL(local_get)

WASM_INSTRUCTION_DECL(local_set)

WASM_INSTRUCTION_DECL(local_tee)

WASM_INSTRUCTION_DECL(global_get)

WASM_INSTRUCTION_DECL(global_set)

WASM_INSTRUCTION_DECL(unreachable)

WASM_INSTRUCTION_DECL(nop)

WASM_INSTRUCTION_DECL(end)

WASM_INSTRUCTION_DECL(return)

WASM_INSTRUCTION_DECL(call)

WASM_INSTRUCTION_DECL(clz)

WASM_INSTRUCTION_DECL(ctz)

WASM_INSTRUCTION_DECL(popcnt)

WASM_INSTRUCTION_DECL(add_i32)

WASM_INSTRUCTION_DECL(sub_i32)

WASM_INSTRUCTION_DECL(mul_i32)

WASM_INSTRUCTION_DECL(div_s_i32)

WASM_INSTRUCTION_DECL(div_u_i32)

WASM_INSTRUCTION_DECL(rem_s_i32)

WASM_INSTRUCTION_DECL(rem_u_i32)

WASM_INSTRUCTION_DECL(and_i32)

WASM_INSTRUCTION_DECL(or_i32)

WASM_INSTRUCTION_DECL(xor_i32)

WASM_INSTRUCTION_DECL(drop)

WASM_INSTRUCTION_DECL(select)

const rWasm_instruction_executor wasm_instruction_decoding_table[256] = {
        WASM_INSTRUCTION_REF(unreachable),
        WASM_INSTRUCTION_REF(nop),
        NULL, // TODO: block
        NULL, // TODO: loop
        NULL, // TODO: if
        NULL, // TODO: else
        NULL, NULL,
        NULL, NULL, NULL,
        WASM_INSTRUCTION_REF(end),
        NULL, // TODO: br
        NULL, // TODO: br_if
        NULL, // TODO: br_table
        WASM_INSTRUCTION_REF(return),

        WASM_INSTRUCTION_REF(call),
        NULL, // TODO: call_indirect
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL,
        WASM_INSTRUCTION_REF(drop),
        WASM_INSTRUCTION_REF(select),
        NULL, // TODO: select(t)
        NULL, NULL, NULL,

        WASM_INSTRUCTION_REF(local_get),
        WASM_INSTRUCTION_REF(local_set),
        WASM_INSTRUCTION_REF(local_tee),
        WASM_INSTRUCTION_REF(global_get),
        WASM_INSTRUCTION_REF(global_set),

        NULL, // TODO: table_get
        NULL, // TODO: table_set
        NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL,



        WASM_INSTRUCTION_REF(const_i32),
        WASM_INSTRUCTION_REF(const_i64),
        WASM_INSTRUCTION_REF(const_f32),
        WASM_INSTRUCTION_REF(const_f64),

        NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        WASM_INSTRUCTION_REF(clz),

        WASM_INSTRUCTION_REF(ctz),
        WASM_INSTRUCTION_REF(popcnt),
        WASM_INSTRUCTION_REF(add_i32),
        WASM_INSTRUCTION_REF(sub_i32),
        WASM_INSTRUCTION_REF(mul_i32),

        WASM_INSTRUCTION_REF(div_s_i32),
        WASM_INSTRUCTION_REF(div_u_i32),
        WASM_INSTRUCTION_REF(rem_s_i32),
        WASM_INSTRUCTION_REF(rem_u_i32),
        WASM_INSTRUCTION_REF(and_i32),
        WASM_INSTRUCTION_REF(or_i32),
        WASM_INSTRUCTION_REF(xor_i32),
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,

        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL,

};

bool wasm_instruction_get_u32(tWasm_context *ctx, uint32_t *value) {
    uint16_t consumed;
    if (!_wasm_extract_u32(
            ctx->current_frame->function->implementation.code + ctx->current_frame->instruction_counter,
            ctx->current_frame->function->implementation.code_length - ctx->current_frame->instruction_counter,
            &consumed, value)) {
        return false;
    }
    ctx->current_frame->instruction_counter += consumed;
    return true;
}
