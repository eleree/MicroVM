#include <vm.h>
#include "class.h"

void * createVM(void)
{
	VMInstance * vm = vmCalloc(1, sizeof(VMInstance));
	vm->bootstrapLoader = vmCalloc(1, sizeof(ClassLoader));
	vm->configArgs.jrepath = "D:/jre";
	vm->configArgs.bootpath = "D:/jre";
	vm->configArgs.bootClass = "HelloWorld";
	if (vm == NULL)
		return NULL;

	return vm;
}

int32_t initVM(VMInstance * vm)
{
	return 0;
}

int32_t startVM(void* vm, int32_t argc, char** argv)
{
	Class * mainClass = NULL;
	VMInstance * vmInstance = (VMInstance*)vm;
	printf("Starting Java VM...\n");

	if (initVM(vm) < 0)
		return -1;

	mainClass = loadClass(vmInstance, vmInstance->configArgs.bootClass);

	return 0;
}

void destroyVM(void* vm)
{
	if (vm != NULL)
		vmFree(vm);
}