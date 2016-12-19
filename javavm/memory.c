#include "porting\config.h"
#include <stdint.h>
#include <string.h>
#include <malloc.h>

static uint64_t memCount = 0;
static uint64_t maxMemCount = 0;

void * vmCalloc(size_t count, size_t size)
{
	void *mem = calloc(count, size);
	memCount += _msize(mem);

	if (memCount > maxMemCount)
		maxMemCount = memCount;

	return mem;
}

void * vmMalloc(size_t size)
{
	void *mem = malloc(size);
	memCount += _msize(mem);

	if (memCount > maxMemCount)
		maxMemCount = memCount;

	return mem;
}

void vmFree(void * mem)
{
	if (mem == NULL)
		return;
	memCount -= _msize(mem);
	free(mem);
}

uint64_t vmMemoryUsed(void)
{
	return memCount;
}

uint64_t vmMaxMemoryUsed(void)
{
	return maxMemCount;
}