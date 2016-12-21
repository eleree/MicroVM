#ifndef __MICRO_VM_CLASS_FILE_H__
#define __MICRO_VM_CLASS_FILE_H__ 

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "vm.h"

typedef struct ClassFile{
	uint64_t size;
	uint8_t * data;
	uint8_t content[1];
}ClassFile;

int32_t readClassBytes(ClassFile * classFile);
uint8_t readClassUint8(ClassFile * classFile);
uint16_t readClassUint16(ClassFile * classFile);
uint32_t readClassUint32(ClassFile * classFile);
uint64_t readClassUint64(ClassFile * classFile);
int32_t skipClassBytes(ClassFile * classFile, uint32_t skipBytes);

ClassFile * loadClassFile(VMConfigArgs * configArgs, const char * classname);

#endif