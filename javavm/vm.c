#include <vm.h>
#include "class.h"
#include "frame.h"
#include "thread.h"

VMInstance * vmInstance = NULL;
void * createVM(void)
{
	VMInstance * vm = vmCalloc(1, sizeof(VMInstance));
	vm->bootstrapLoader = vmCalloc(1, sizeof(ClassLoader));
	vm->configArgs.jrepath = "D:/jre";
	vm->configArgs.bootpath = "D:/jre";
	vm->configArgs.bootClass = "FibonacciTest";
	vm->bootstrapLoader->classpath = "D:/jre";
	vm->bootstrapLoader->zippath = "D:/jre/rt.jar";
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

	
	mainClass = loadClass(vmInstance->bootstrapLoader, vmInstance->configArgs.bootClass);

	MethodBlock * method = getClassMainMethod(mainClass);

	Frame * frame = newFrame(mainThread, method,method->maxLocals, method->maxStack);
	pushThreadFrame(mainThread, frame);

	interpret(mainClass, method, mainThread, argc, argv);

	vmFree(frame);
	return 0;
}

void destroyVM(void* vm)
{
	if (vm != NULL)
		vmFree(vm);
}