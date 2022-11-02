

set(WASM_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/execution/interpreter.c
        ${CMAKE_CURRENT_LIST_DIR}/src/execution/instruction-decoder.c
        ${CMAKE_CURRENT_LIST_DIR}/src/execution/instruction-numeric.c
        ${CMAKE_CURRENT_LIST_DIR}/src/execution/instruction-variable.c
        ${CMAKE_CURRENT_LIST_DIR}/src/execution/instruction-control.c
        ${CMAKE_CURRENT_LIST_DIR}/src/execution/instruction-logic.c
        ${CMAKE_CURRENT_LIST_DIR}/src/execution/instruction-parametric.c
        ${CMAKE_CURRENT_LIST_DIR}/src/execution/instruction-arithmetic.c
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/wa-memory.c
        ${CMAKE_CURRENT_LIST_DIR}/src/utils.c
        ${CMAKE_CURRENT_LIST_DIR}/src/format/format.c
        ${CMAKE_CURRENT_LIST_DIR}/src/format/format-functions.c
        ${CMAKE_CURRENT_LIST_DIR}/src/binding/wa-binding.c
        )

set(WASM_INCLUDES
        ${CMAKE_CURRENT_LIST_DIR}/include/
        ${CMAKE_CURRENT_LIST_DIR}/src/
        )

