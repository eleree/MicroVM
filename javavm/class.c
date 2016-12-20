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
	for (uint16_t i = 1; i < c->constantPoolCount; i++)
	{
		uint8_t tag = readClassUint8(classFile);
		(c->constantPool + i)->cpType = tag;
		switch (tag)
		{
		case CONSTATNT_CLASS:
			(c->constantPool+i)->cpItem.bits = readClassUint16(classFile);
			break;
		case CONSTATNT_FIELDREF:
			(c->constantPool + i)->cpItem.unresolved.fieldRef.classIndex = readClassUint16(classFile);
			(c->constantPool + i)->cpItem.unresolved.fieldRef.nameAndTypeIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_METHODREF:
			(c->constantPool + i)->cpItem.unresolved.methodRef.classIndex = readClassUint16(classFile);
			(c->constantPool + i)->cpItem.unresolved.methodRef.nameAndTypeIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_INTERFACE_METHODREF:
			(c->constantPool + i)->cpItem.unresolved.interfaceMethodRef.classIndex = readClassUint16(classFile);
			(c->constantPool + i)->cpItem.unresolved.interfaceMethodRef.nameAndTypeIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_STRING:
			(c->constantPool + i)->cpItem.unresolved.stringIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_INTEGER:
			(c->constantPool + i)->cpItem.bits = readClassUint32(classFile);
			break;
		case CONSTATNT_FLOAT:
			(c->constantPool + i)->cpItem.bits = readClassUint32(classFile);
			break;
		case CONSTATNT_LONG:
			(c->constantPool + i)->cpItem.bits = ((uint64_t)readClassUint32(classFile)) << 32;
			(c->constantPool + i)->cpItem.bits |= readClassUint32(classFile);			
			break;
		case CONSTATNT_DOUBLE:
			(c->constantPool + i)->cpItem.bits = ((uint64_t)readClassUint32(classFile)) << 32;
			(c->constantPool + i)->cpItem.bits |= readClassUint32(classFile);
			break;
		case CONSTATNT_NAME_AND_TYPE:
			(c->constantPool + i)->cpItem.unresolved.nameAndType.nameIndxe = readClassUint16(classFile);
			(c->constantPool + i)->cpItem.unresolved.nameAndType.descriptorIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_UTF8:			
			do
			{
				uint16_t length = readClassUint16(classFile);
				(c->constantPool + i)->cpItem.mutf8String = vmCalloc(length+1, sizeof(char));
				for (uint16_t i = 0; i < length; i++)
				{
					(c->constantPool + i)->cpItem.mutf8String[i] = readClassUint8(classFile);
				}
			} while (0);			
			break;
		case CONSTATNT_METHOD_HANDLE:
			break;
		case CONSTATNT_METHOD_TYPE:
			break;
		case CONSTATNT_INVOKE_DYNAMIC:
			break;
		}

		if (tag == CONSTATNT_LONG || tag == CONSTATNT_DOUBLE)
			i++;
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