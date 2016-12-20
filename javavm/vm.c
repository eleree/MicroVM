#include <vm.h>

void * createVM(void)
{
	JavaVM * vm = vmCalloc(1, sizeof(JavaVM));

	vm->configArgs.jrepath = "D:/jre";
	vm->configArgs.bootpath = "D:/jre";
	vm->configArgs.bootClass = "HelloWorld";
	if (vm == NULL)
		return NULL;

	return vm;
}

int32_t initVM(JavaVM * vm)
{
	return 0;
}

int32_t startVM(void* vm, int32_t argc, char** argv)
{
	printf("Starting Java VM...\n");

	if (initVM(vm) < 0)
		return -1;



	return 0;
}

void destroyVM(void* vm)
{
	if (vm != NULL)
		vmFree(vm);
}