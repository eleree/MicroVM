#ifndef __MICRO_VM_VM_H__
#define __MICRO_VM_VM_H__

#include <stdint.h>
#include <string.h>

#include "porting\config.h"
#include "assert.h"
#include "memory.h"

typedef struct VMConfigArgs
{
	char * bootpath;
	char * jrepath;

	char * bootClass;
	uint64_t minHeap;
	uint64_t maxHeap;

	/* JNI invocation API hooks */
	int(*vfprintf)(FILE *stream, const char *fmt, va_list ap);
	void(*exit)(int status);
	void(*abort)(void);

}VMConfigArgs;

typedef struct VMInstance
{
	struct VMConfigArgs configArgs;
	struct ClassLoader * bootstrapLoader;
}VMInstance;

void * createVM(void);

int32_t startVM(void* vm, int32_t argc, char** argv);

void destroyVM(void* vm);

#endif