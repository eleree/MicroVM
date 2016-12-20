#ifndef __MICRO_VM_CLASS_H__
#define __MICRO_VM_CLASS_H__ 

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "porting\config.h"

struct Method;
struct Field;
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

typedef struct ConstantPool
{
	uint8_t cpType;
	union{
		char * mutf8String;
		uintptr_t * classRef;
		uintptr_t * fieldRef;
		uintptr_t * methodRef;
		uintptr_t * interfaceMethodRef;
		uintptr_t * string;
		uint64_t bits;
		union{
			uint16_t nameAndTypeIndex;
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
	uint16_t fieldsCount;
	//struct Field *fields;
	uint16_t methodsCount;
	//struct Method * methods;
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
	char * sourceFile;
}Class;
#endif