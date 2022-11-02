//
// Created by tumap on 9/23/22.
//
#include "mem-manager.h"

#if defined(WASM_MEMORY_TRACE) && WASM_MEMORY_TRACE > 1
#include <stdio.h>
#define TRACE(...) printf("WASM Memory: "); printf(__VA_ARGS__); printf("\n");
#else
#define TRACE(...)
#endif
#if defined(WASM_MEMORY_TRACE) && WASM_MEMORY_TRACE > 0
#include <stdio.h>
#define TRACE_MEMORY(...) printf("WASM Memory: "); printf(__VA_ARGS__); printf("\n");
#else
#define TRACE_MEMORY(...)
#endif


#ifndef WASM_MEMORY_SYSTEM_KB
#define SYSTEM_SIZE     (8*1024)
#else
#define SYSTEM_SIZE     ((WASM_MEMORY_SYSTEM_KB)*1024)
#endif

void wasm_memory_initialize(tWasm_memory *ctx, uint8_t *memory, tWasm_mem_size length) {
    ctx->memory=memory;
    ctx->total_size=length;
    ctx->system_size=SYSTEM_SIZE;
    ctx->system_used=0;
    ctx->heap_set=false;
}

void *wasm_memory_system_alloc(tWasm_memory* ctx, tWasm_mem_size size) {
    if(ctx->heap_set) {
        TRACE("Can't allocate system memory block, heap already created")
        return NULL;
    }
    uint8_t *ret = ctx->memory + ctx->system_size;
    if(ctx->system_size+size>ctx->total_size) {
        TRACE("Can't allocate system memory block, out of memory")
        return NULL;
    }
    ctx->system_size += size;
    TRACE("Allocated %d bytes as system memory", size)
    return ret;
}

bool wasm_memory_initialize_heap(tWasm_memory* ctx) {
    if(ctx->heap_set) {
        TRACE("Can't initialize heap, already initialized")
        return false;
    }
    ctx->heap_base=ctx->system_size;
    ctx->heap_size=ctx->total_size-ctx->system_size;
    ctx->heap_used=0;
    ctx->heap_set=true;
    TRACE_MEMORY("System size is %d bytes", ctx->system_size)
    TRACE_MEMORY("Heap size is %d bytes", ctx->heap_size)

    return true;
}
