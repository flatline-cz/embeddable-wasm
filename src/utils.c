//
// Created by tumap on 10/21/22.
//
#include <stdlib.h>
#include "wa-utils.h"


bool _wasm_extract_u32(const uint8_t *data, uint16_t data_length,
                       uint16_t *consumed, uint32_t *decoded) {
    uint32_t value;
    uint8_t byte;
    uint8_t i;
    uint8_t shift;

    for (i = 0, shift = 0; i < 5 && data_length; i++, data_length--, shift += 7) {
        byte = data[i];
        if (shift)
            value |= ((uint16_t) (byte & 0x7f)) << shift;
        else
            value = (uint16_t) (byte & 0x7f);
        if ((byte & 0x80) == 0) {
            *consumed = i + 1;
            *decoded = value;
            return true;
        }
    }

    return false;
}


const tWasm_function *_wasm_find_function(const tWasm_context *const ctx, const uint16_t function_index) {
    tWasm_function_list_item *f;
    for (f = ctx->functions.function_list; f != NULL && f->function.index != function_index; f = f->next);
    return (f != NULL) ? &f->function : NULL;
}

