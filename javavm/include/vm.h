#ifndef __MICRO_VM_VM_H__
#define __MICRO_VM_VM_H__

#include <stdint.h>
#include <string.h>

#include "porting\config.h"
#include "assert.h"
#include "memory.h"

void * createVM(void);

int32_t startVM(void* vm, int32_t argc, char** argv);

void destroyVM(void* vm);

#endif