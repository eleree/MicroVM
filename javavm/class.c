#include "class.h"
#include "classfile.h"
#include "thread.h"

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
			cp->cpItem.unresolved.nameIndex = readClassUint16(classFile);
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
			cp->cpItem.u32 = readClassUint32(classFile);
			break;
		case CONSTATNT_FLOAT:
			cp->cpItem.unresolved.bits = readClassUint32(classFile);
			break;
		case CONSTATNT_LONG:
			cp->cpItem.unresolved.bits = ((uint64_t)readClassUint32(classFile)) << 32;
			cp->cpItem.unresolved.bits |= readClassUint32(classFile);
			break;
		case CONSTATNT_DOUBLE:
			cp->cpItem.unresolved.bits = ((uint64_t)readClassUint32(classFile)) << 32;
			cp->cpItem.unresolved.bits |= readClassUint32(classFile);
			break;
		case CONSTATNT_NAME_AND_TYPE:
			cp->cpItem.nameAndType.nameIndex = readClassUint16(classFile);
			cp->cpItem.nameAndType.descriptorIndex = readClassUint16(classFile);
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

int64_t intToLong(uint32_t high, uint32_t low)
{
	typedef union LongInt
	{
		struct bitsData{
			uint32_t low;
			uint32_t high;
		};
		int64_t  longData;
	}LongInt;

	LongInt transData;
	transData.high = high;
	transData.low = low;
	return transData.longData;
}

double longToDouble(uint64_t l)
{
	typedef union DoubleLong
	{
		uint64_t bitsData;
		double doubleData;
	}DoubleLong;

	DoubleLong transData;
	transData.bitsData = l;
	return transData.doubleData;
}


void resolveClassConstantPool(ClassFile * classFile, Class * c)
{

	for (uint16_t i = 1; i < c->constantPoolCount; i++)
	{
		ConstantPool * cp = c->constantPool + i;
		uint8_t tag = cp->cpType;
		switch (tag)
		{
		case CONSTATNT_CLASS:
			do{
				uint16_t classNameIndex = cp->cpItem.unresolved.nameIndex;
				cp->cpItem.classRef = vmCalloc(1, sizeof(ClassRef));
				cp->cpItem.classRef->symbolicRef.className = getConstantPoolMUTF8(c, classNameIndex);
				cp->cpItem.classRef->symbolicRef.fromClass = c;
			} while (0);			
			break;
		default:
			break;
		}
	}

	for (uint16_t i = 1; i < c->constantPoolCount; i++)
	{
		ConstantPool * cp = c->constantPool + i;
		uint8_t tag = cp->cpType;
		switch (tag)
		{
		case CONSTATNT_CLASS:
			//cp->cpItem.bits = readClassUint16(classFile);
			break;
		case CONSTATNT_FIELDREF:
		{
			{
				uint16_t classIndex = cp->cpItem.unresolved.fieldRef.classIndex;
				uint16_t nameAndTypeIndex = cp->cpItem.unresolved.fieldRef.nameAndTypeIndex;
				cp->cpItem.fieldRef = vmCalloc(1, sizeof(FieldRef));
				cp->cpItem.fieldRef->attachClass = c;
				cp->cpItem.fieldRef->symbolicRef.fromClass = c;
				cp->cpItem.fieldRef->symbolicRef.className = (c->constantPool + classIndex)->cpItem.classRef->symbolicRef.className;
				cp->cpItem.fieldRef->name = getConstalPoolNameAndTypeName(c, nameAndTypeIndex);
				cp->cpItem.fieldRef->descriptor = getConstalPoolNameAndTypeDescriptor(c, nameAndTypeIndex);
			}				
		}
			break;
		case CONSTATNT_METHODREF:
		{
			uint16_t classIndex = cp->cpItem.unresolved.methodRef.classIndex;
			uint16_t nameAndTypeIndex = cp->cpItem.unresolved.methodRef.nameAndTypeIndex;
			cp->cpItem.methodRef = vmCalloc(1, sizeof(MethodRef));
			cp->cpItem.methodRef->symbolicRef.fromClass = c;
			cp->cpItem.methodRef->symbolicRef.className = (c->constantPool + classIndex)->cpItem.classRef->symbolicRef.className;
			cp->cpItem.methodRef->name = getConstalPoolNameAndTypeName(c, nameAndTypeIndex);
			cp->cpItem.methodRef->descriptor = getConstalPoolNameAndTypeDescriptor(c, nameAndTypeIndex);
		}
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
			cp->cpItem.s64 = cp->cpItem.unresolved.bits;
			break;
		case CONSTATNT_DOUBLE:
			cp->cpItem.doubleVal = longToDouble(cp->cpItem.unresolved.bits);
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

const char * getConstalPoolNameAndTypeName(Class * c, uint16_t nameAndTypeIndex)
{
	if (c->constantPool[nameAndTypeIndex].cpType == CONSTATNT_NAME_AND_TYPE)
	{
		return getConstantPoolMUTF8(c, c->constantPool[nameAndTypeIndex].cpItem.nameAndType.nameIndex);
	}
	return NULL;
}

const char * getConstalPoolNameAndTypeDescriptor(Class * c, uint16_t nameAndTypeIndex)
{
	if (c->constantPool[nameAndTypeIndex].cpType == CONSTATNT_NAME_AND_TYPE)
	{
		return getConstantPoolMUTF8(c, c->constantPool[nameAndTypeIndex].cpItem.nameAndType.descriptorIndex);
	}
	return NULL;
}

MethodRef * getClassConstantPoolMethodRef(Class * c, uint16_t index)
{
	vmAssert(c->constantPool[index].cpType == CONSTATNT_METHODREF);
	if (c->constantPool[index].cpType == CONSTATNT_METHODREF)
	{
		return c->constantPool[index].cpItem.methodRef;
	}
	return NULL;
}

void resovleClassFileFields(ClassFile * classFile, Class * c, uint16_t fieldBlockCount)
{
	if (fieldBlockCount == 0)
		return;

	c->fields = vmCalloc(fieldBlockCount, sizeof(FieldBlock));
	for (uint16_t i = 0; i < fieldBlockCount; i++)
	{
		uint16_t nameIndxe = 0;
		uint16_t descriptorIndex = 0;
		FieldBlock * field = c->fields + i;
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

#define SCAN_SIG(p, D, S)			\
	p++;               /* skip start ( */	\
	while (*p != ')') {	\
	if ((*p == 'J') || (*p == 'D')) {\
		D;					\
		p++;					\
	}else {	\
	S;					\
	if (*p == '[')				\
	for (p++; *p == '['; p++);		\
	if (*p == 'L')				\
	while (*p++ != ';');		\
	else					\
	p++;				\
	}						\
	}						\
	p++;               /* skip end ) */


void resovleClassFileMethods(ClassFile * classFile, Class * c, uint16_t methodBlockCount)
{
	if (methodBlockCount == 0)
		return;
	/* prepare class methods */
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

		/* caculate method args count */
		{
			int count = 0;
			const char *sig = method->classMember.descriptor;
			/* calculate argument count from signature */
			SCAN_SIG(sig, count += 2, count++);
			if (method->classMember.accessFlags & ACC_STATIC)
				method->argSlotCount = count;
			else
				method->argSlotCount = count + 1;
			printf("Method Descriptor:%s, argSlot:%d\n", method->classMember.descriptor, 
																	method->argSlotCount);
		}

		method->classMember.attachClass = c;
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

	c->name = (c->constantPool + thisClass)->cpItem.classRef->symbolicRef.className;

	if (superClass != 0) 
		c->superClassName = (c->constantPool + superClass)->cpItem.classRef->symbolicRef.className;

	c->interfacesCount = readClassUint16(classFile);	
	if (c->interfacesCount > 0)
	{
		c->interfaces = vmCalloc(c->interfacesCount, sizeof(Class));
		for (uint16_t i = 0; i < c->interfacesCount; i++)
		{
			uint16_t interfaceIndex = readClassUint16(classFile);
			const char * interfaceName = getConstantPoolMUTF8(c, interfaceIndex);
		}
		
		//c->interfaceNames = vmCalloc(c->interfaceNamesCount, sizeof(char*));
	}

	c->fieldsCount = readClassUint16(classFile);
	resovleClassFileFields(classFile, c, c->fieldsCount);

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

void resolveSuperClass(ClassLoader * classLoader, Class * subClass)
{
	if (strcmp(subClass->name, "java/lang/Object") != 0)
		subClass->superClass = loadClass(classLoader, subClass->superClassName);
}

void resolveInterfaces(ClassLoader * classLoader, Class * subClass)
{
	if (subClass->interfacesCount == 0)
		return;
	if (subClass->interfaceNamesCount != subClass->interfacesCount)
		printf("Interface Name Count != Interface Count, maybe this is a BUG.\n");

	subClass->interfaces = calloc(subClass->interfacesCount, sizeof(Class *));
	for (uint16_t i = 0; i < subClass->interfacesCount; i++)
	{
		subClass->interfaces[i] = loadClass(classLoader, subClass->interfaceNames[i]);
	}
}


//jvms-5.4
void defineClass(ClassLoader * classLoader, Class * c)
{
	c->classLoader =  classLoader;
	resolveSuperClass(classLoader, c);
	resolveInterfaces(classLoader, c);
	addClassLoaderList(classLoader, c);
}

void verify(Class * class)
{

}

void calcInstanceFieldCount(Class * c)
{
	uint16_t slotId = 0;
	if (c->superClass != NULL)
	{
		slotId = c->superClass->instanceSlotCount;
	}
	for (uint16_t i = 0; i < c->fieldsCount; i++)
	{
		if (!isFieldStatic(&c->fields[i]))
		{
			c->fields[i].slotId = slotId;
			slotId++;
			if (isFieldLongOrDouble(&c->fields[i]))
				slotId++;
		}
	}
	c->instanceSlotCount = slotId;
}

void calcClassStaticFieldCount(Class * c)
{
	uint16_t slotId = 0;
	for (uint16_t i = 0; i < c->fieldsCount; i++)
	{
		if (isFieldStatic(&c->fields[i]))
		{
			c->fields[i].slotId = slotId;
			slotId++;
			if (isFieldLongOrDouble(&c->fields[i]))
				slotId++;
		}
	}
	c->staticSlotCount = slotId;
}

void initClassStaticField(Class * c)
{
	c->staticVars = NULL;
	if (c->staticSlotCount == 0)
		return;
	c->staticVars = calloc(c->staticSlotCount, sizeof(Slot));
	for (uint16_t i = 0; i < c->fieldsCount; i++)
	{
		if (isFieldStatic(c->fields + i) && isFieldFinal(c->fields + i))
		{
			//initStaticFinalVar(c, c->fields + i);
		}
	}
}

void prepare(Class * c) 
{
	calcInstanceFieldCount(c);
	calcClassStaticFieldCount(c);
	initClassStaticField(c);
}

void linkClass(ClassLoader * classLoader, Class * c)
{
	verify(c);
	prepare(c);	
}

Class * loadClass(ClassLoader * classLoader, const char * className)
{
	ClassFile * classFile = loadClassFile(classLoader, className);
	vmAssert(classFile != NULL);

	Class * c = parseClassFile(classFile);
	vmAssert(c != NULL);

	defineClass(classLoader, c);

	linkClass(classLoader, c);

	vmFree(classFile);

	return c;
}

void releaseClass(Class * c)
{
	vmAssert(c != NULL);
	if (c->constantPool!=NULL)
		vmFree(c->constantPool);

	if (c->fields != NULL)
		vmFree(c->fields);

}


void initClass(Thread * thread, Class * c)
{

}

bool isMethodStatic(MethodBlock * method)
{
	vmAssert(method != NULL);
	return	0 != (method->classMember.accessFlags & ACC_STATIC);
}


MethodBlock * getClassStaticMethod(Class * c, const char * name, const char * descriptor)
{
	for (uint16_t i = 0; i < c->methodsCount; i++)
	{
		if (isMethodStatic(c->methods + i) &&
			strcmp(c->methods[i].classMember.name, name) == 0 &&
			strcmp(c->methods[i].classMember.descriptor, descriptor) == 0)
			return (c->methods + i);
	}
	return NULL;
}

MethodBlock * getClassMainMethod(Class * c)
{
	return getClassStaticMethod(c, "main", "([Ljava/lang/String;)V");
}