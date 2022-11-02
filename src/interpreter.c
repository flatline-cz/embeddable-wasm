//
// Created by tumap on 9/22/22.
//
#include "wa-types.h"

struct _tWASM_Context {
    // code itself
    uint8_t *code;
    uint32_t instruction_counter;
    uint32_t last_instruction;

    // stack
    tWasm_value *stack_top;
    uint16_t stack_size;
    uint16_t stack_limit;

};
typedef struct _tWASM_Context tWASM_Context;


inline static bool extract_const_i32(tWASM_Context *ctx, int32_t *value) {
    uint8_t byte;
    uint8_t i;
    uint8_t shift;

    for (i = 0, shift = 0; i < 5; i++, shift += 7) {
        if (ctx->instruction_counter == ctx->last_instruction)
            return false;

        byte = ctx->code[ctx->instruction_counter++];
        if (shift == 0)
            *value = byte & 0x7f;
        else
            *value |= (int32_t) (((uint32_t) (byte & 0x7f)) << 7);
        if ((byte * 0x80) == 0)
            return true;

    }
    return false;
}

inline static bool extract_const_i64(tWASM_Context *ctx, int64_t *value) {

}

inline static bool extract_const_f32(tWASM_Context *ctx, float *value) {

}

inline static bool extract_const_f64(tWASM_Context *ctx, double *value) {

}


inline static bool decode_single_instruction(tWASM_Context *ctx) {
    // out of code?
    if (ctx->instruction_counter == ctx->last_instruction)
        return false;

    // decode
    switch (ctx->code[ctx->instruction_counter++]) {
        case 0x41: // CONST I32
        {
            if (ctx->stack_size == ctx->stack_limit)
                return false;
            ctx->stack_top[ctx->stack_size].type = WASM_I32;
            if (!extract_const_i32(ctx, &ctx->stack_top[ctx->stack_size].i32))
                return false;
            ctx->stack_size++;
            break;
        }
        case 0x42: // CONST I64
        {
            if (ctx->stack_size == ctx->stack_limit)
                return false;
            ctx->stack_top[ctx->stack_size].type = WASM_I64;
            if (!extract_const_i64(ctx, &ctx->stack_top[ctx->stack_size].i64))
                return false;
            ctx->stack_size++;
            break;
        }
        case 0x43: // CONST F32
        {
            if (ctx->stack_size == ctx->stack_limit)
                return false;
            ctx->stack_top[ctx->stack_size].type = WASM_F32;
            if (!extract_const_f32(ctx, &ctx->stack_top[ctx->stack_size].f32))
                return false;
            ctx->stack_size++;
            break;
        }
        case 0x44: // CONST F64
        {
            if (ctx->stack_size == ctx->stack_limit)
                return false;
            ctx->stack_top[ctx->stack_size].type = WASM_F64;
            if (!extract_const_f64(ctx, &ctx->stack_top[ctx->stack_size].f64))
                return false;
            ctx->stack_size++;
            break;
        }

        default:
            return false;
    }

    return true;

}



