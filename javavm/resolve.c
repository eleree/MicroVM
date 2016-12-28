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

bool isFieldVolatile(FieldBlock * field)
{
	return 0 != (field->classMember.accessFlags & ACC_VOLATILE);
}

bool isFieldTransient(FieldBlock * field)
{
	return 0 != (field->classMember.accessFlags & ACC_TRANSIENT);
}

bool isFieldEnum(FieldBlock * field)
{
	return 0 != (field->classMember.accessFlags & ACC_ENUM);
}

bool isFieldLongOrDouble(FieldBlock * field)
{
	if (field->classMember.descriptor[0] == 'J' || field->classMember.descriptor[0] == 'D')
		return true;

	return false;
}

bool isFieldPublic(FieldBlock * field)
{
	if (field == NULL)
		return false;

	return	0 != (field->classMember.accessFlags & ACC_PUBLIC);
}

bool isFieldPrivate(FieldBlock * field)
{
	if (field == NULL)
		return false;

	return	0 != (field->classMember.accessFlags & ACC_PRIVATE);
}

bool isFieldProtected(FieldBlock * field)
{
	if (field == NULL)
		return false;

	return	0 != (field->classMember.accessFlags & ACC_PROTECTED);
}

bool isFieldStatic(FieldBlock * field)
{
	if (field == NULL)
		return false;

	return	0 != (field->classMember.accessFlags & ACC_STATIC);
}

bool isFieldFinal(FieldBlock * field)
{
	if (field == NULL)
		return false;

	return	0 != (field->classMember.accessFlags & ACC_FINAL);
}

bool isFieldSynthetic(FieldBlock * field)
{
	if (field == NULL)
		return false;

	return	0 != (field->classMember.accessFlags & ACC_SYNTHETIC);
}


