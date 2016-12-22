#include <vm.h>
#include "class.h"
#include "frame.h"
#include "thread.h"
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

extern void interpret(Class * c, MethodBlock * method, Thread * thread, int argc, char ** argv);

int32_t startVM(void* vm, int32_t argc, char** argv)
{
	Class * mainClass = NULL;
	Thread * mainThread = newThread();

	VMInstance * vmInstance = (VMInstance*)vm;
	printf("Starting Java VM...\n");

	if (initVM(vm) < 0)
		return -1;

	
	mainClass = loadClass(vmInstance, vmInstance->configArgs.bootClass);

	interpret(mainClass, getClassMainMethod(mainClass), mainThread, argc, argv);

	return 0;
}

void destroyVM(void* vm)
{
	if (vm != NULL)
		vmFree(vm);
}