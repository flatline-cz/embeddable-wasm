//
// Created by tumap on 10/27/22.
//
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "wa-types.h"
#include "wa-utils.h"
#include "wa-memory.h"


static uint32_t signature_count;
typedef struct _tWasm_signature_item {
    struct _tWasm_signature_item *next;
    uint16_t index;
    tWasm_signature signature;
} tWasm_signature_item;
static tWasm_signature_item *signatures = NULL;
static tWasm_signature_item *signature_last = NULL;

static void _wasm_format_signature_dump(tWasm_signature *signature);

static inline bool map_value_type(uint8_t wasm_type, eWasm_value_type *dest);

bool _wasm_format_parse_section_TYPE(
        tWasm_context *ctx,
        uint8_t *src, unsigned section_length) {

    uint16_t consumed;

    // get number of signatures
    if (!_wasm_extract_u32(src, section_length,
                           &consumed, &signature_count)) {
        return false;
    }
    src += consumed;
    section_length -= consumed;

    unsigned i;
    // process each signature
    for (i = 0; i < signature_count; i++) {
        // check type constant
        if (!section_length || *src != 0x60) {
            return false;
        }
        src++;
        section_length--;
        // get number of parameters
        uint32_t param_count;
        if (!_wasm_extract_u32(src, section_length, &consumed, &param_count)) {
            return false;
        }
        src += consumed;
        section_length -= consumed;

        // allocate space for signature
        tWasm_signature_item *signature_item = wasm_memory_system_alloc(
                &ctx->memory, sizeof(tWasm_signature_item));
        if (signature_item == NULL) {
            return false;
        }
        signature_item->index = i;
        signature_item->next = NULL;
        if (signature_last != NULL)
            signature_last->next = signature_item;
        else
            signatures = signature_item;
        signature_last = signature_item;
        signature_item->signature.param_count = param_count;
        if (param_count != 0) {
            signature_item->signature.params = wasm_memory_system_alloc(
                    &ctx->memory, param_count * sizeof(eWasm_value_type));
            if (signature_item->signature.params == NULL) {
                return false;
            }
        }

        // process parameter types
        unsigned j;
        for (j = 0; j < signature_item->signature.param_count; j++) {
            if (!section_length)
                return false;
            if (!map_value_type(*src, signature_item->signature.params + j)) {
                return false;
            }
            src++;
            section_length--;
        }

        // get number of results
        uint32_t result_count;
        if (!_wasm_extract_u32(src, section_length, &consumed, &result_count)) {
            return false;
        }
        src += consumed;
        section_length -= consumed;

        if (result_count > 1) {
            return false;
        }
        if (result_count == 1) {
            if (section_length < 2 || *src != 0x60) {
                return false;
            }
            src++;
            section_length -= 2;
            if (!map_value_type(*src, &signature_item->signature.return_type)) {
                return false;
            }
            src++;
        } else {
            signature_item->signature.return_type = WASM_NULL;
        }

    }
    return true;
}

static tWasm_signature *_wasm_signature_find(uint16_t index) {
    tWasm_signature_item *item;
    for (item = signatures; item != NULL; item = item->next) {
        if (index == 0)
            return &item->signature;
        index--;
    }
    return NULL;
}


static inline bool map_value_type(uint8_t wasm_type, eWasm_value_type *dest) {
    switch (wasm_type) {
        case 0x7f:
            *dest = WASM_I32;
            break;
        default:
            return false;
    }
    return true;
}

static void _wasm_dump_value_type(eWasm_value_type type) {
    switch (type) {
        case WASM_NULL:
            printf("nil");
            break;
        case WASM_I32:
            printf("i32");
            break;
        default:
            printf("?");
            break;
    }
}

static void _wasm_format_signature_dump(tWasm_signature *signature) {
    printf("(");
    int i;
    for (i = 0; i < signature->param_count; i++) {
        if (i != 0)
            printf(",");
        _wasm_dump_value_type(signature->params[i]);
    }
    printf(") -> (");
    _wasm_dump_value_type(signature->return_type);
    printf(")");
}

static void _wasm_format_function_dump(tWasm_function *function) {
    printf("%s, signature", function->name.name);
    _wasm_format_signature_dump(function->signature);
    if (function->exported) {
        printf(", exported");
    }
    if (function->imported) {
        printf(", imported");
    } else {
        if (function->implementation.code == NULL)
            printf(", CODE MISSING");
        else {
            printf(", code found, locals (");
            int l;
            for (l = 0; l < function->implementation.locals_count; l++) {
                if (l != 0)
                    printf(",");
                _wasm_dump_value_type(function->implementation.locals[l]);
            }
            printf(")");
        }
    }
    printf("\n");
}

void _wasm_format_functions_dump(tWasm_context *ctx) {
    tWasm_function_list_item *f;
    for (f = ctx->functions.function_list; f != NULL; f = f->next)
        _wasm_format_function_dump(&f->function);
}


static tWasm_function *_wasm_add_function(tWasm_context *ctx, uint16_t type_index) {
    // add new record to the list
    tWasm_function_list_item *function_item = wasm_memory_system_alloc(
            &ctx->memory, sizeof(tWasm_function_list_item));
    if (function_item == NULL) {
        return NULL;
    }
    function_item->next = NULL;
    if (ctx->functions.function_list_last != NULL) {
        function_item->function.index = ctx->functions.function_list_last->function.index + 1;
        ctx->functions.function_list_last->next = function_item;
    } else {
        function_item->function.index = 0;
        ctx->functions.function_list = function_item;
    }
    ctx->functions.function_list_last = function_item;

    // set signature
    tWasm_signature *signature = _wasm_signature_find(type_index);
    if (signature == NULL) {
        return NULL;
    }
    function_item->function.signature = signature;

    return &function_item->function;
}

bool _wasm_format_parse_section_IMPORT_function(
        tWasm_context *ctx,
        const char *name, uint8_t name_length,
        uint32_t index) {

    // create function
    tWasm_function *function = _wasm_add_function(ctx, index);
    if (function == NULL) {
        return false;
    }

    // set name
    function->name.name = name;
    function->name.name_length = name_length;

    // imported
    function->imported = true;
    function->import.binding = NULL;

    return true;
}

bool _wasm_format_parse_section_FUNCTION(
        tWasm_context *ctx,
        const uint8_t *src, unsigned section_length) {
    uint16_t consumed;

    // get function count
    uint32_t count;
    if (!_wasm_extract_u32(src, section_length, &consumed, &count)) {
        return false;
    }
    src += consumed;
    section_length -= consumed;

    unsigned i;
    for (i = 0; i < count; i++) {
        // parse index
        uint32_t index;
        if (!_wasm_extract_u32(src, section_length, &consumed, &index)) {
            return false;
        }
        src += consumed;
        section_length -= consumed;

        // create a new function
        tWasm_function *function = _wasm_add_function(ctx, index);
        if (function == NULL) {
            return false;
        }

        // clear name
        char *name = wasm_memory_system_alloc(
                &ctx->memory, 7);
        if (name == NULL) {
            return false;
        }
        function->name.name = name;
        function->name.name_length = 6;
        sprintf(name, "_f#%03d",
                function->index);

    }

    return true;
}

bool _wasm_format_parse_section_EXPORT_function(
        tWasm_context *ctx,
        const char *name, uint8_t name_length,
        uint32_t index) {

    // find function
    tWasm_function *function = (tWasm_function *) _wasm_find_function(ctx, index);
    if (function == NULL) {
        return false;
    }

    // set name
    function->name.name = name;
    function->name.name_length = name_length;

    return true;
}

bool _wasm_format_parse_section_CODE(
        tWasm_context *ctx,
        const uint8_t *src, unsigned section_length) {

    uint16_t consumed;

    // extract number of code segments
    uint32_t number_of_code_segments;
    if (!_wasm_extract_u32(src, section_length, &consumed, &number_of_code_segments)) {
        return false;
    }
    src += consumed;
    section_length -= consumed;

    // parse each code segment
    uint16_t index;
    uint16_t count;
    for (index = 0, count = number_of_code_segments; index < count; index++) {
        // find function
        uint16_t idx;
        tWasm_function_list_item *node;
        for (idx = 0, node = ctx->functions.function_list; node != NULL; node = node->next) {
            if (node->function.imported)
                continue;
            if (idx == index)
                break;
            idx++;
        }
        if (node == NULL) {
            return false;
        }
        tWasm_function *function = &node->function;

        // get size
        uint32_t code_size;
        if (!_wasm_extract_u32(src, section_length, &consumed, &code_size)) {
            return false;
        }
        src += consumed;
        section_length -= consumed;
        const uint8_t *next_segment = src + code_size;
        uint16_t next_length = section_length - code_size;

        // get number of local types
        uint32_t number_of_locals;
        if (!_wasm_extract_u32(src, section_length, &consumed, &number_of_locals)) {
            return false;
        }
        src += consumed;
        section_length -= consumed;

        // count total locals count
        unsigned local_count = 0;
        const uint8_t *local_src = src;
        uint16_t local_length = section_length;

        unsigned i;
        for (i = 0; i < number_of_locals; i++) {
            uint32_t j;
            if (!_wasm_extract_u32(src, section_length, &consumed, &j)) {
                return false;
            }
            src += consumed;
            section_length -= consumed;
            if (section_length == 0) {
                return false;
            }
            src++;
            section_length--;
            local_count += j;
        }

        // allocate memory
        function->implementation.locals_count = local_count;
        function->implementation.locals = wasm_memory_system_alloc(
                &ctx->memory, local_count * sizeof(eWasm_value_type));
        if (function->implementation.locals == NULL) {
            return false;
        }

        // extract each local type
        src = local_src;
        section_length = local_length;

        unsigned l_idx = 0, l_cnt;
        for (i = 0; i < number_of_locals; i++) {
            uint32_t j;
            if (!_wasm_extract_u32(src, section_length, &consumed, &j)) {
                return false;
            }
            src += consumed;
            section_length -= consumed;
            if (section_length == 0) {
                return false;
            }
            uint8_t type = *src;
            src++;
            section_length--;

            for (l_cnt = 0; l_cnt < j; l_cnt++, l_idx++) {
                if (!map_value_type(type, function->implementation.locals + l_idx)) {
                    return false;
                }
            }
        }

        // set code pointers
        if (function->imported) {
            return false;
        }
        function->implementation.code = src;
        function->implementation.code_length = section_length - next_length;

        // next code segment
        src = next_segment;
        section_length = next_length;
    }

    return true;
}
