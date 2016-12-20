#include "classfile.h"
#include "class.h"

int32_t readClassBytes(ClassFile * classFile)
{
	return 0;
}

uint8_t readClassUint8(ClassFile * classFile)
{
	uint8_t data = 0x00;
	data |= *(classFile->data++);

	return data;
}

uint16_t readClassUint16(ClassFile * classFile)
{
	uint16_t data = 0x00;
	for (int i = 0; i < 2; i++)
	{
		data <<= 8;
		data |= *(classFile->data++);
	}

	return data;
}

uint16_t * readClassUint16s(ClassFile * classFile)
{
	uint16_t needLen = readClassUint16(classFile);
	uint16_t * data = vmCalloc(needLen, sizeof(uint16_t));
	for (int i = 0; i < 2; i++)
	{
		data[i] = *(classFile->data++);
	}
	return data;
}

uint32_t readClassUint32(ClassFile * classFile)
{
	uint32_t data = 0x00;
	for (int i = 0; i < 4; i++)
	{
		data <<= 8;
		data |= *(classFile->data++);
	}

	return data;
}

uint64_t readClassUint64(ClassFile * classFile)
{
	uint64_t data = 0x00;
	for (int i = 0; i < 8; i++)
	{
		data <<= 8;
		data |= *(classFile->data++);
	}

	return data;
}

