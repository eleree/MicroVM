#include "class.h"
#include "classfile.h"


int32_t readAndCheckVersion(ClassFile * classFile)
{
	uint16_t minorVersion = readClassUint16(classFile);
	uint16_t majorVersion = readClassUint16(classFile);
	switch (majorVersion)
	{
	case 45:
		return 0;
		break;
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
		if (minorVersion == 0)
			return 0;
		break;
	default:
		break;
	}
	return 1;
}

int32_t readAndCheckMagic(ClassFile * classFile)
{
	uint32_t magicNumber = readClassUint32(classFile);
	if (magicNumber != 0xCAFEBABE)
	{
		printf("java.lang.ClassFormatError: magic!");
		return 1;
	}
	return 0;
}

void readConstantPool(ClassFile * classFile, Class * c)
{
	c->constantPoolCount = readClassUint16(classFile);
	c->constantPool = vmCalloc(sizeof(ConstantPool), c->constantPoolCount);
	for (uint16_t i = 0; i < c->constantPoolCount; i++)
	{
		uint8_t tag = readClassUint8(classFile);
		(c->constantPool + i)->cpType = tag;
		switch (tag)
		{
		case CONSTATNT_CLASS:
			(c->constantPool+i)->cpItem.bits = readClassUint16(classFile);
			break;
		case CONSTATNT_FIELDREF:
			break;
		case CONSTATNT_METHODREF:
			break;
		case CONSTATNT_INTERFACE_METHODREF:
			break;
		case CONSTATNT_STRING:
			break;
		case CONSTATNT_INTEGER:
			break;
		case CONSTATNT_FLOAT:
			break;
		case CONSTATNT_LONG:
			break;
		case CONSTATNT_DOUBLE:
			break;
		case CONSTATNT_NAME_AND_TYPE:
			break;
		case CONSTATNT_UTF8:
			break;
		case CONSTATNT_METHOD_HANDLE:
			break;
		case CONSTATNT_METHOD_TYPE:
			break;
		case CONSTATNT_INVOKE_DYNAMIC:
			break;
		}


	}
}

Class * parseClassFile(ClassFile * classFile)
{
	Class * c = NULL;

	readAndCheckMagic(classFile);

	if (readAndCheckVersion(classFile) < 0)
		return NULL;

	c = vmCalloc(1, sizeof(Class));
	vmAssert(c != NULL);

	readConstantPool(classFile, c);

	c->accessFlags = readClassUint16(classFile);
	uint16_t thisClass = readClassUint16(classFile);
	uint16_t superClass = readClassUint16(classFile);
	return c;
}