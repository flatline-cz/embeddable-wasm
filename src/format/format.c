//
// Created by tumap on 9/23/22.
//

#include "format.h"
#include "mem-manager.h"
#include "wa-types.h"
#include "utils.h"
#include <stdio.h>
#include <memory.h>

#ifdef WASM_FORMAT_TRACE
#define TRACE(...) printf("WASM Format:  "); printf(__VA_ARGS__); printf("\n");
#else
#define TRACE(...)
#endif


bool _wasm_format_parse_section_TYPE(
        tWasm_context *ctx,
        const uint8_t *src, unsigned section_length);

bool _wasm_format_parse_section_FUNCTION(
        tWasm_context *ctx,
        const uint8_t *src, unsigned section_length);

bool _wasm_format_parse_section_CODE(
        tWasm_context *ctx,
        const uint8_t *src, unsigned section_length);

bool _wasm_format_parse_section_IMPORT_function(
        tWasm_context *ctx,
        const char *name, uint8_t name_length,
        uint32_t index);

bool _wasm_format_parse_section_EXPORT_function(
        tWasm_context *ctx,
        const char *name, uint8_t name_length,
        uint32_t index);

void _wasm_format_functions_dump(tWasm_context *ctx);

static bool parse_export_section(tWasm_context *ctx, const uint8_t *source, uint16_t length);

static bool parse_import_section(tWasm_context *ctx, const uint8_t *source, uint16_t length);


bool wasm_format_load(tWasm_context *ctx, const uint8_t *src, uint16_t length) {
    ctx->source = src;
    ctx->functions.function_list = NULL;
    ctx->functions.function_list_last = NULL;

    // check magic number & version
    if (length < 8) {
        TRACE("Invalid length(%d)", length)
        return false;
    }
    if (src[0] != 0x00 || src[1] != 0x61 || src[2] != 0x73 || src[3] != 0x6d ||
        src[4] != 0x01 || src[5] != 0x00 || src[6] != 0x00 || src[7] != 0x00) {
        TRACE("Invalid header")
        return false;
    }
    src += 8;
    length -= 8;

    // parse each section
    while (length != 0) {
        // extract section type
        if (length < 1) {
            TRACE("Invalid structure (unable to extract section type)")
            return false;
        }
        uint8_t section_type = *(src++);
        length--;

        // extract section length
        uint32_t section_length;
        uint16_t consumed;
        if (!_wasm_extract_u32(src, length, &consumed, &section_length)) {
            TRACE("Invalid structure (unable to extract section length)")
            return false;
        }
        src += consumed;
        length -= consumed;

        // validate section length
        if (section_length > length) {
            TRACE("Invalid structure (inconsistent section length)")
            return false;
        }

        // process section
        switch (section_type) {
            case 0x00: // CUSTOM SECTION
                break;

            case 0x01: // TYPE SECTION
                if (!_wasm_format_parse_section_TYPE(ctx, src, section_length))
                    return false;
                break;

            case 0x02: // IMPORT SECTION
                if (!parse_import_section(ctx, src, section_length))
                    return false;
                break;

            case 0x03: // FUNCTION SECTION
                if (!_wasm_format_parse_section_FUNCTION(ctx, src, section_length))
                    return false;
                break;

            case 0x05: // MEMORY SECTION
                break;

            case 0x06: // GLOBAL SECTION
            {
                TRACE("GLOBAL section (length %d)", section_length)
                break;
            }

            case 0x07: // EXPORT SECTION
                if (!parse_export_section(ctx, src, section_length))
                    return false;
                break;

            case 0x0A: // CODE SECTION
                if (!_wasm_format_parse_section_CODE(ctx, src, section_length))
                    return false;
                break;

            default: {
//                TRACE("Unknown section (type %d, length %d)", section_type, section_length);
//                return false;
            }
        }

        // skip section
        src += section_length;
        length -= section_length;

    }

    _wasm_format_functions_dump(ctx);

    return true;
}


static void parse_entity_name(tWasm_context *ctx, tWasm_entity_name *name, const uint8_t *source, uint16_t length) {
    name->name_length = length;
    char *text = wasm_memory_system_alloc(&ctx->memory, length + 1);
    memcpy(text, source, length);
    text[length] = 0;
    name->name = text;
}

static bool parse_import_section(tWasm_context *const ctx, const uint8_t *source, uint16_t length) {
    uint16_t consumed;

    // extract number of imports
    uint32_t number_of_imports;
    if (!_wasm_extract_u32(source, length, &consumed, &number_of_imports)) {
        TRACE("Invalid structure (unable to extract number of imported entities)")
        return false;
    }
    source += consumed;
    length -= consumed;

    // parse each import
    while (number_of_imports-- > 0) {
        // get module name length
        uint32_t module_name_length;
        if (!_wasm_extract_u32(source, length, &consumed, &module_name_length)) {
            TRACE("Invalid structure (unable to extract imported entity module name length)")
            return false;
        }
        source += consumed;
        length -= consumed;
        source += module_name_length;
        length -= module_name_length;

        // get name length
        uint32_t name_length;
        if (!_wasm_extract_u32(source, length, &consumed, &name_length)) {
            TRACE("Invalid structure (unable to extract imported entity name length)")
            return false;
        }
        source += consumed;
        length -= consumed;
        const uint8_t *name_start = source;
        source += name_length;
        length -= name_length;

        // get type
        if (length < 2) {
            TRACE("Invalid structure (unable to extract imported entity type)")
            return false;
        }
        uint8_t type = *source;
        source++;
        length--;

        // get index
        uint32_t index;
        if (!_wasm_extract_u32(source, length, &consumed, &index)) {
            TRACE("Invalid structure (unable to extract imported entity index)")
            return false;
        }
        source += consumed;
        length -= consumed;

        // is function?
        if (type == 0x00) {

            if (!_wasm_format_parse_section_IMPORT_function(
                    ctx, (const char *) name_start, name_length, index)) {
                return false;
            }
            continue;
        }

        // TODO: decode type
        TRACE("NOT-SUPPORTED: import type %d", type)
    }

    return true;
}

static bool parse_export_section(tWasm_context *const ctx, const uint8_t *source, uint16_t length) {
    uint16_t consumed;

    // extract number of exports
    uint32_t number_of_exports;
    if (!_wasm_extract_u32(source, length, &consumed, &number_of_exports)) {
        TRACE("Invalid structure (unable to extract number of exported entities)")
        return false;
    }
    source += consumed;
    length -= consumed;

    // parse each export
    while (number_of_exports-- > 0) {

        // get name length
        uint32_t name_length;
        if (!_wasm_extract_u32(source, length, &consumed, &name_length)) {
            TRACE("Invalid structure (unable to extract exported entity name length)")
            return false;
        }
        source += consumed;
        length -= consumed;
        const uint8_t *name_start = source;
        source += name_length;
        length -= name_length;

        // get type
        if (length < 2) {
            TRACE("Invalid structure (unable to extract exported entity type)")
            return false;
        }
        uint8_t type = *source;
        source++;
        length--;

        // get index
        uint32_t index;
        if (!_wasm_extract_u32(source, length, &consumed, &index)) {
            TRACE("Invalid structure (unable to extract exported entity index)")
            return false;
        }
        source += consumed;
        length -= consumed;

        // is function?
        if (type == 0x00) {
            if (!_wasm_format_parse_section_EXPORT_function(
                    ctx, (const char *) name_start, name_length, index)) {
                return false;
            }
            continue;
        }

        // TODO: decode type
//        TRACE("NOT-SUPPORTED: export type %d", type)
    }
    return true;
}
