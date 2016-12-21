#ifndef __MICRO_VM_UTF8_H__
#define __MICRO_VM_UTF8_H__ 

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

int32_t stringReplaceDot(const char * sourceStr, char * targetStr, int32_t maxLen);

int32_t stringReplaceSplash(const char * sourceStr, char * targetStr, int32_t maxLen);

int32_t stringHasSuffix(const char * sourceStr, const char * suffixStr);

#endif 