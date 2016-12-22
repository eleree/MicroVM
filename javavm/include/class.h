#ifndef __MICRO_VM_CLASS_H__
#define __MICRO_VM_CLASS_H__ 

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "porting\config.h"

struct MethodBlock;
struct FieldBlock;
struct Class;
struct ClassLoader;

enum ConstantType
{
	CONSTATNT_CLASS = 7,
	CONSTATNT_FIELDREF = 9,
	CONSTATNT_METHODREF = 10,
	CONSTATNT_INTERFACE_METHODREF = 11,
	CONSTATNT_STRING = 8,
	CONSTATNT_INTEGER = 3,
	CONSTATNT_FLOAT = 4,
	CONSTATNT_LONG = 5,
	CONSTATNT_DOUBLE = 6,
	CONSTATNT_NAME_AND_TYPE = 12,
	CONSTATNT_UTF8 = 1,
	CONSTATNT_METHOD_HANDLE = 15,
	CONSTATNT_METHOD_TYPE = 16,
	CONSTATNT_INVOKE_DYNAMIC = 18,
};

enum {
	ACC_PUBLIC = 0x0001, // class field method
	ACC_PRIVATE = 0x0002, //       field method
	ACC_PROTECTED = 0x0004, //       field method
	ACC_STATIC = 0x0008, //       field method
	ACC_FINAL = 0x0010, // class field method
	ACC_SUPER = 0x0020, // class
	ACC_SYNCHRONIZED = 0x0020, //             method
	ACC_VOLATILE = 0x0040, //       field
	ACC_BRIDGE = 0x0040, //             method
	ACC_TRANSIENT = 0x0080, //       field
	ACC_VARARGS = 0x0080, //             method
	ACC_NATIVE = 0x0100, //             method
	ACC_INTERFACE = 0x0200, // class
	ACC_ABSTRACT = 0x0400, // class       method
	ACC_STRICT = 0x0800, //             method
	ACC_SYNTHETIC = 0x1000, // class field method
	ACC_ANNOTATION = 0x2000, // class
	ACC_ENUM = 0x4000 // class field
};

typedef struct ConstantPool
{
	uint8_t cpType;
	union{
		char * mutf8String;
		uintptr_t * classRef;
		uintptr_t * fieldRef;
		uintptr_t * methodRef;
		uintptr_t * interfaceMethodRef;
		uint16_t stringIndex;
		uint64_t bits;
		union{
			uint16_t nameIndxe;
			uint16_t nameAndTypeIndex;
			uint16_t stringIndex;
			union{
				uint16_t nameIndxe;
				uint16_t descriptorIndex;
			}nameAndType;
			union{
				uint16_t classIndex;
				uint16_t nameAndTypeIndex;
			}fieldRef;
			union{
				uint16_t classIndex;
				uint16_t nameAndTypeIndex;
			}methodRef;
			union{
				uint16_t classIndex;
				uint16_t nameAndTypeIndex;
			}interfaceMethodRef;
			union{
				uint8_t referenceKind;
			}methodHandle;
		}unresolved;
	}cpItem;

}ConstantPool;

typedef struct Class
{
	uint16_t accessFlags;
	char * name;
	char * packageName;
	char * superClassName;
	uint16_t interfaceNamesCount;
	char* * interfaceNames; // interfaceName  array
	uint16_t constantPoolCount;
	struct ConstantPool * constantPool;
	uint16_t fieldBlockCount;
	struct FieldBlock * fieldBlock;
	uint16_t methodsCount;
	struct MethodBlock * methods;
	struct Class * superClass;
	uint16_t interfacesCount;
	struct Class * *interfaces;
	uint32_t instanceSlotCount;
	uint32_t staticSlotCount;
	uint32_t slotCount;
	//Slot * staticVars;
	//struct ClassLoader * classLoader;
	bool initStarted;
	//Object * jClass;
	const char * sourceFile;
}Class;

/* definition of ClassMember */
typedef struct ClassMember
{
	uint16_t accessFlags;
	const char * name;
	const char * descriptor;
	const char * signature;
	struct Class * attachClass;
	uint8_t * annotationData;
	uint32_t annotationDataLen;
}ClassMember;

/* definition of Field */
typedef struct FieldBlock
{
	struct ClassMember classMember;
	union{
		uint16_t index;
		uint32_t floatBits;
		uint32_t doubleBits;
		uint32_t integer;
		uint64_t longInteger;
	}constantValue;
}FieldBlock;

typedef struct LineNumberTableEntry
{
	uint16_t startPc;
	uint16_t lineNumber;
}LineNumberTableEntry;

typedef struct LineNumberTable{
	uint16_t length;
	LineNumberTableEntry entry[0];
}LineNumberTable;

typedef struct ClassRef {
	struct ConstantPool * constantPool;
	char * className;
	Class * c;
}ClassRef;

typedef struct ExceptionHandler
{
	int32_t startPc;
	int32_t endPc;
	int32_t handlerPc;
	struct ClassRef *catchType;
}ExceptionHandler;

typedef struct ExceptionHandlerTable
{
	int32_t handlerCount;
	ExceptionHandler * exceptionHandler;
}ExceptionHandlerTable;

typedef struct ExceptionsTable{	
	uint16_t number;
	uint16_t indexTable[0];
}ExceptionsTable;

typedef struct MethodBlock
{
	struct ClassMember classMember;
	uint16_t maxStack;
	uint16_t maxLocals;
	uint32_t codeLen;
	uint8_t * code;
	uint16_t argSlotCount;
	struct ExceptionHandlerTable * exceptionHandlerTable;
	struct ExceptionsTable * exceptionTable;
	LineNumberTable * lineNumberTable;
	//struct MethodDescriptor * parsedDescriptor;
	//uint8_t * parameterAnnotationData;
	//uint32_t parameterAnnotationDataLen;
	//uint8_t * annotationDefaultData;
	//uint32_t annotationDefaultDataLen;
}MethodBlock;

typedef struct ClassList
{
	char * className;
	struct Class * classPtr;
	struct ClassList * next;
}ClassList;

typedef struct ClassLoader
{
	ClassList * classList;
}ClassLoader;


typedef struct Object {
	struct Class* c;
	int32_t  dataCount;
	uint16_t dataType;
	void * data;

	int32_t extraCount;
	uint16_t extraType;
	void * extra;
}Object;

Class * parseClassFile(struct ClassFile * classFile);
Class * loadClass(struct VMInstance * vm, const char * bootClass);

MethodBlock * getClassMainMethod(Class * c);

#endif