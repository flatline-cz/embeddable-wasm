//
// Created by tumap on 11/9/22.
//

#ifndef DASHBOARD_FORMAT_TRACE_H
#define DASHBOARD_FORMAT_TRACE_H

#ifdef TRACE_WASM_FORMAT
#include <stdio.h>
#define TRACE(...) printf("WASM Format:  "); printf(__VA_ARGS__); printf("\n");
#define FAILURE(...) printf("WASM Format:  "); printf(__VA_ARGS__); printf(" (%s, line %d)\n", __FUNCTION__, __LINE__);
#else
#define TRACE(...)
#define FAILURE(...)
#endif

#endif //DASHBOARD_FORMAT_TRACE_H
