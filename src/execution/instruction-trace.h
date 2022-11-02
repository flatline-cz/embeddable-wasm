//
// Created by tumap on 10/24/22.
//

#ifndef WASM_TEST_INSTRUCTION_TRACE_H
#define WASM_TEST_INSTRUCTION_TRACE_H

#ifdef WASM_EXEC_TRACE
#include <stdio.h>
#define TRACE(...) printf("WASM Exec:  "); printf(__VA_ARGS__); printf("\n");
#else
#define TRACE(...)
#endif

#if defined(WASM_EXEC_TRACE) && WASM_EXEC_TRACE == 2
#define TRACE_INSTRUCTION(name) printf("WASM Exec:  <%s>\n", #name );
#else
#define TRACE_INSTRUCTION
#endif

#define WASM_INSTRUCTION(name) bool _wasm_instruction_ ## name (tWasm_context *const ctx) { \
TRACE_INSTRUCTION(name)

#define WASM_INSTRUCTION_DECL(name) bool _wasm_instruction_ ## name (tWasm_context* ctx);

#define WASM_INSTRUCTION_REF(name) _wasm_instruction_ ## name
#endif //WASM_TEST_INSTRUCTION_TRACE_H
