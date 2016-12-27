#include <vm.h>
#include "class.h"

extern VMInstance * vmInstance;

void resolveClassRef(SymbolicRef * symRef)
{
	Class * d = symRef->fromClass;
	Class * c = loadClass(d->classLoader, symRef->className);

	vmAssert(c != NULL);

	symRef->inClass = c;
}

Class * resolveClass(SymbolicRef * symRef)
{
	if (symRef->inClass == NULL)
	{
		resolveClassRef(symRef);
	}
	return symRef->inClass;
}

MethodBlock * resolveMethod(MethodRef * methodRef)
{
	return NULL;
}