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

void prepareClassConstantPool(ClassFile * classFile, Class * c)
{
	c->constantPoolCount = readClassUint16(classFile);
	c->constantPool = vmCalloc(sizeof(ConstantPool), c->constantPoolCount);
	for (uint16_t i = 1; i < c->constantPoolCount; i++)
	{
		uint8_t tag = readClassUint8(classFile);
		ConstantPool * cp = (c->constantPool + i);
		cp->cpType = tag;
		printf("Index:%d, Tag:%d\n", i, tag);
		switch (tag)
		{
		case CONSTATNT_CLASS:
			cp->cpItem.bits = readClassUint16(classFile);
			break;
		case CONSTATNT_FIELDREF:
			cp->cpItem.unresolved.fieldRef.classIndex = readClassUint16(classFile);
			cp->cpItem.unresolved.fieldRef.nameAndTypeIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_METHODREF:
			cp->cpItem.unresolved.methodRef.classIndex = readClassUint16(classFile);
			cp->cpItem.unresolved.methodRef.nameAndTypeIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_INTERFACE_METHODREF:
			cp->cpItem.unresolved.interfaceMethodRef.classIndex = readClassUint16(classFile);
			cp->cpItem.unresolved.interfaceMethodRef.nameAndTypeIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_STRING:
			cp->cpItem.stringIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_INTEGER:
			cp->cpItem.bits = readClassUint32(classFile);
			break;
		case CONSTATNT_FLOAT:
			cp->cpItem.bits = readClassUint32(classFile);
			break;
		case CONSTATNT_LONG:
			cp->cpItem.bits = ((uint64_t)readClassUint32(classFile)) << 32;
			cp->cpItem.bits |= readClassUint32(classFile);			
			break;
		case CONSTATNT_DOUBLE:
			cp->cpItem.bits = ((uint64_t)readClassUint32(classFile)) << 32;
			cp->cpItem.bits |= readClassUint32(classFile);
			break;
		case CONSTATNT_NAME_AND_TYPE:
			cp->cpItem.unresolved.nameAndType.nameIndxe = readClassUint16(classFile);
			cp->cpItem.unresolved.nameAndType.descriptorIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_UTF8:			
			do
			{
				uint16_t length = readClassUint16(classFile);
				cp->cpItem.mutf8String = vmCalloc(length+1, sizeof(char));
				for (uint16_t b = 0; b < length; b++)
				{
					cp->cpItem.mutf8String[b] = readClassUint8(classFile);
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

void resolveClassConstantPool(ClassFile * classFile, Class * c)
{
	for (uint16_t i = 0; i < c->constantPoolCount; i++)
	{
		ConstantPool * cp = c->constantPool + i;
		uint8_t tag = cp->cpType;
		switch (tag)
		{
		case CONSTATNT_CLASS:
			//cp->cpItem.bits = readClassUint16(classFile);
			break;
		case CONSTATNT_FIELDREF:
			//cp->cpItem.unresolved.fieldRef.classIndex = readClassUint16(classFile);
			//cp->cpItem.unresolved.fieldRef.nameAndTypeIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_METHODREF:
			//cp->cpItem.unresolved.methodRef.classIndex = readClassUint16(classFile);
			//cp->cpItem.unresolved.methodRef.nameAndTypeIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_INTERFACE_METHODREF:
			//cp->cpItem.unresolved.interfaceMethodRef.classIndex = readClassUint16(classFile);
			//cp->cpItem.unresolved.interfaceMethodRef.nameAndTypeIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_STRING:
			//cp->cpItem.unresolved.stringIndex = readClassUint16(classFile);
			break;
		case CONSTATNT_INTEGER:
			//cp->cpItem.bits = readClassUint32(classFile);
			break;
		case CONSTATNT_FLOAT:
			//cp->cpItem.bits = readClassUint32(classFile);
			break;
		case CONSTATNT_LONG:
			//cp->cpItem.bits = ((uint64_t)readClassUint32(classFile)) << 32;
			//cp->cpItem.bits |= readClassUint32(classFile);
			break;
		case CONSTATNT_DOUBLE:
			//cp->cpItem.bits = ((uint64_t)readClassUint32(classFile)) << 32;
			//cp->cpItem.bits |= readClassUint32(classFile);
			break;
		case CONSTATNT_NAME_AND_TYPE:
			//cp->cpItem.unresolved.nameAndType.nameIndxe = readClassUint16(classFile);
			//cp->cpItem.unresolved.nameAndType.descriptorIndex = readClassUint16(classFile);
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

		if (tag == CONSTATNT_LONG || tag == CONSTATNT_DOUBLE)
			i++;
	}
}

const char * getConstantPoolMUTF8(Class * c, uint16_t index)
{
	if (c->constantPool[index].cpType == CONSTATNT_STRING)
	{
		return getConstantPoolMUTF8(c, c->constantPool[index].cpItem.stringIndex);
	}
	vmAssert(c->constantPool[index].cpType == CONSTATNT_UTF8);

	return c->constantPool[index].cpItem.mutf8String;
}

void resovleClassFileFields(ClassFile * classFile, Class * c, uint16_t fieldBlockCount)
{
	if (fieldBlockCount == 0)
		return;

	c->fieldBlock = vmCalloc(fieldBlockCount, sizeof(FieldBlock));
	for (uint16_t i = 0; i < fieldBlockCount; i++)
	{
		uint16_t nameIndxe = 0;
		uint16_t descriptorIndex = 0;
		FieldBlock * field = c->fieldBlock + i;
		field->classMember.accessFlags = readClassUint16(classFile);
		nameIndxe = readClassUint16(classFile);
		descriptorIndex = readClassUint16(classFile);
		field->classMember.name = getConstantPoolMUTF8(c, nameIndxe);;
		field->classMember.descriptor = getConstantPoolMUTF8(c, descriptorIndex);
		uint16_t fieldAttrCount = readClassUint16(classFile);
		for (uint16_t j = 0; j < fieldAttrCount; j++)
		{
			uint16_t attrNameIndex = readClassUint16(classFile);
			if (strcmp(getConstantPoolMUTF8(c,attrNameIndex), "ConstantValue") == 0)
			{
				uint32_t attrLen = readClassUint32(classFile);
				field->constantValue.index = readClassUint16(classFile);
				printf("ConstantValue Index:%d\n", field->constantValue.index);
				//skipClassBytes(classFile, attrLen);
			}else{
				uint32_t attrLen = readClassUint32(classFile);
				skipClassBytes(classFile, attrLen);
			}
		}
	}
}

void resovleClassFileCodeAttr(ClassFile * classFile, Class * c, MethodBlock * method)
{
	uint32_t attrLen = readClassUint32(classFile);
	method->maxStack = readClassUint16(classFile);
	method->maxLocals = readClassUint16(classFile);
	method->codeLen = readClassUint32(classFile);
	vmAssert(method->codeLen != 0);
	method->code = vmCalloc(method->codeLen, sizeof(uint8_t));
	for (uint32_t i = 0; i < method->codeLen; i++)
		method->code[i] = readClassUint8(classFile);
	
	uint16_t exceptionTableLength = readClassUint16(classFile);
	if (exceptionTableLength > 0){
		method->exceptionTable = vmCalloc(1, sizeof(ExceptionsTable)+sizeof(uint16_t)*exceptionTableLength);
		/* TODO */
	}

	/* resovle class file method CODE's attr */

	uint16_t codeAttrCount = readClassUint16(classFile);
	for (uint16_t i = 0; i < codeAttrCount; i++)
	{
		const char * codeAttrName = getConstantPoolMUTF8(c, readClassUint16(classFile));
		vmAssert(codeAttrName != NULL);
		/* skip all code now */
		if (strcmp(codeAttrName, "LineNumberTable") == 0)
		{
			uint32_t attrLen = readClassUint32(classFile);
			skipClassBytes(classFile, attrLen);
		}else if (strcmp(codeAttrName, "LocalVariableTable") == 0)
		{
			uint32_t attrLen = readClassUint32(classFile);
			skipClassBytes(classFile, attrLen);
		}else{
			uint32_t attrLen = readClassUint32(classFile);
			skipClassBytes(classFile, attrLen);
		}
	}

}

void resovleClassFileMethods(ClassFile * classFile, Class * c, uint16_t methodBlockCount)
{
	if (methodBlockCount == 0)
		return;
	c->methods = vmCalloc(methodBlockCount, sizeof(MethodBlock));
	for (uint16_t i = 0; i < methodBlockCount; i++)
	{
		uint16_t nameIndex = 0;
		uint16_t descriptorIndex = 0;
		uint16_t attrCount = 0;
		MethodBlock * method = c->methods + i;
		method->classMember.accessFlags = readClassUint16(classFile);
		nameIndex = readClassUint16(classFile);
		descriptorIndex = readClassUint16(classFile);
		method->classMember.name = getConstantPoolMUTF8(c, nameIndex);
		method->classMember.descriptor = getConstantPoolMUTF8(c, descriptorIndex);
		attrCount = readClassUint16(classFile);
		for (uint16_t j = 0; j < attrCount; j++)
		{
			uint16_t attrNameIndex = readClassUint16(classFile);
			const char * attrName = getConstantPoolMUTF8(c, attrNameIndex);
			vmAssert(attrName != NULL);
			if (strcmp(attrName, "Code") == 0)
			{
				resovleClassFileCodeAttr(classFile, c, method);
			}else{
				uint32_t skipBytes = readClassUint32(classFile);
				skipClassBytes(classFile, skipBytes);
			}
		}
	}
}

Class * parseClassFile(ClassFile *classFile)
{
	Class * c = NULL;

	readAndCheckMagic(classFile);

	if (readAndCheckVersion(classFile) < 0)
		return NULL;

	c = vmCalloc(1, sizeof(Class));
	vmAssert(c != NULL);

	prepareClassConstantPool(classFile, c);
	resolveClassConstantPool(classFile, c);

	c->accessFlags = readClassUint16(classFile);
	uint16_t thisClass = readClassUint16(classFile);
	uint16_t superClass = readClassUint16(classFile);

	c->interfaceNamesCount = readClassUint16(classFile);
	if (c->interfaceNamesCount > 0)
	{
		c->interfaceNames = vmCalloc(c->interfaceNamesCount, sizeof(char*));
	}

	c->fieldBlockCount = readClassUint16(classFile);
	resovleClassFileFields(classFile, c, c->fieldBlockCount);

	c->methodsCount = readClassUint16(classFile);
	resovleClassFileMethods(classFile, c, c->methodsCount);

	uint16_t classAttrCount = readClassUint16(classFile);
	for (uint16_t i = 0; i < classAttrCount; i++)
	{
		uint16_t classAttrNameIndex = readClassUint16(classFile);
		uint32_t classAttrLenght = readClassUint32(classFile);
		const char * classAttrName = getConstantPoolMUTF8(c, classAttrNameIndex);

		if (strcmp(classAttrName, "SourceFile") == 0)
		{
			c->sourceFile = getConstantPoolMUTF8(c, readClassUint16(classFile));
		}else if (strcmp(classAttrName, "InnerClasses") == 0)
		{
			skipClassBytes(classFile, classAttrCount);
		}else{
			skipClassBytes(classFile, classAttrCount);
		}
		
	}
	
	return c;
}

void defineClass(ClassLoader * classLoader, Class * c)
{

}

void addClassLoaderList(ClassLoader * classLoader, Class * newClass)
{
	ClassList * classList = classLoader->classList;
	ClassList * preClassList = NULL;
	while (classList != NULL)
	{
		if (strcmp(classList->className, newClass->name) == 0)
			return;

		preClassList = classList;
		classList = classList->next;
	}

	classList = vmCalloc(1, sizeof(ClassList));
	classList->className = newClass->name;
	classList->classPtr = newClass;
	classList->next = NULL;

	if (preClassList == NULL)
		classLoader->classList = classList;

	if (preClassList != NULL)
		preClassList->next = classList;

	return;
}


Class * loadClass(VMInstance * vm, const char * bootClass)
{
	ClassFile * classFile = loadClassFile(&vm->configArgs, bootClass);
	vmAssert(classFile != NULL);

	Class * c = parseClassFile(classFile);
	vmAssert(c != NULL);

	vmFree(classFile);

	return c;
}

void releaseClass(Class * c)
{
	vmAssert(c != NULL);
	if (c->constantPool!=NULL)
		vmFree(c->constantPool);

	if (c->fieldBlock != NULL)
		vmFree(c->fieldBlock);

}