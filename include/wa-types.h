//
// Created by tumap on 9/22/22.
//

#ifndef WASM_WASM_H
#define WASM_WASM_H

#include <stdbool.h>
#include <stdint.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef uint16_t tWasm_code_pointer;
typedef uint32_t tWasm_mem_size;

// *******************************************
// **  Operand stack                        **
// *******************************************

typedef enum enumWASM_ValueType {
    WASM_NULL, WASM_FUNC_ADDR, WASM_EXTERN_FUNC_ADDR,
    WASM_I32, WASM_I64, WASM_F32, WASM_F64
} eWasm_value_type;

typedef struct tagWASM_Value {
    eWasm_value_type type;
    union {
        int32_t i32;
        int64_t i64;
        float f32;
        double f64;
    };
} tWasm_value;

typedef struct tagWasm_value_stack {
    tWasm_value *base;
    uint16_t size;
    uint16_t top;
} tWasm_value_stack;

// *******************************************
// **  Calling stack                        **
// *******************************************

struct tagWasm_function;

typedef enum enumWasm_stack_item_type {
    WASM_LOCAL, WASM_FRAME
} eWasm_stack_item_type;

typedef struct tagWasm_frame {
    struct tagWasm_frame *caller_frame;
    uint16_t locals_count;
    tWasm_value_stack value_stack;
    const struct tagWasm_function *function;
    tWasm_code_pointer instruction_counter;
} tWasm_frame;

typedef struct tagWasm_stack_item {
    eWasm_stack_item_type type;
    union {
        tWasm_value local;
        tWasm_frame frame;
    };
} tWasm_stack_item;

typedef struct tagWasm_stack {
    tWasm_stack_item *base;
    uint16_t size;
    uint16_t top;
} tWasm_stack;

// *******************************************
// **  Memory management context            **
// *******************************************

typedef struct tagWasm_memory {
    // entire memory block
    uint8_t *memory;
    tWasm_mem_size total_size;

    // system memory block
    tWasm_mem_size system_size;
    tWasm_mem_size system_used;

    // heap memory
    bool heap_set;
    tWasm_mem_size heap_base;
    tWasm_mem_size heap_size;
    tWasm_mem_size heap_used;
} tWasm_memory;

// *******************************************
// **  Program context                      **
// *******************************************

typedef struct tagWasm_signature {
    eWasm_value_type return_type;
    unsigned param_count;
    eWasm_value_type* params;
} tWasm_signature;

typedef struct tagWasm_entity_name {
    const char *name;
    uint8_t name_length;
} tWasm_entity_name;

typedef enum enumWasm_binding_type {
    WASM_BINDING_TYPE_NONE, WASM_BINDING_TYPE_U32, WASM_BINDING_TYPE_BYTE_ARRAY
} eWasm_binding_type;

typedef struct tagWasm_binding_value {
    eWasm_binding_type type;
    union {
        struct {
            uint32_t *value;
        } u32;
        struct {
            int length;
            char *value;
        } byte_array;
    };
} tWasm_binding_value;

typedef struct tagWasm_function_call {
    const char *name;
    unsigned param_count;
    tWasm_binding_value *params;
    tWasm_binding_value return_value;
} tWasm_function_call;

typedef bool (*rWasm_function_binding)(tWasm_function_call *call_ctx);

typedef struct tagWasm_binding {
    tWasm_function_call call_ctx;
    rWasm_function_binding routine;
} tWasm_binding;

typedef struct tagWasm_function {
    tWasm_entity_name name;
    uint16_t index;
    bool imported;
    bool exported;
    tWasm_signature* signature;
    union {
        struct {
            // code
            const uint8_t *code;
            uint16_t code_length;
            // locals
            uint16_t locals_count;
            eWasm_value_type *locals;
        } implementation;
        struct {
            tWasm_binding *binding;
        } import;
    };
} tWasm_function;

typedef struct tagWasm_function_list_item {
    tWasm_function function;
    struct tagWasm_function_list_item *next;
} tWasm_function_list_item;

typedef struct tagWasm_function_list {
    tWasm_function_list_item *function_list;
    tWasm_function_list_item *function_list_last;
} tWasm_function_list;

// *******************************************
// **  Runtime context                      **
// *******************************************

typedef struct tagWasm_context {
    // ***********************************
    // ** Program context               **
    // ***********************************

    // source data
    const uint8_t *source;

    // list of functions
    tWasm_function_list functions;

    // memory management
    tWasm_memory memory;

    // ***********************************
    // ** Current execution context     **
    // ***********************************

    // operand stack
    tWasm_value_stack value_stack;

    // function frames
    tWasm_stack frame_stack;

    // current frame
    tWasm_frame *current_frame;


} tWasm_context;


#endif //WASM_WASM_H
