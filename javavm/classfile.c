#include "classfile.h"
#include "class.h"
#include "include\utf8.h"

#pragma warning(disable:4996)

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

int32_t skipClassBytes(ClassFile * classFile, uint32_t skipBytes)
{
	for (uint32_t i = 0; i < skipBytes; i++)
		readClassUint8(classFile);
	return 0;
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

ClassFile * readClassFileDirEntry(char *classpath) 
{
	FILE *fd;
	uint64_t readLen = 0;
	uint64_t fileLen = 0;
	ClassFile * classFile = NULL;
	if ((fd = fopen(classpath, "rb")) == NULL)
		return NULL;

	/* get class file size */
	fseek(fd, 0L, SEEK_END);
	fileLen = ftell(fd);
	fseek(fd, 0L, SEEK_SET);
	
	/* allocate necessary memory */
	classFile = vmCalloc(1,(size_t)(sizeof(ClassFile)+sizeof(uint8_t)*fileLen));
	readLen = fread((void *)classFile->content, sizeof(char), (size_t)fileLen, fd);

	fclose(fd);

	if (readLen == fileLen)
	{
		classFile->data = classFile->content;
		classFile->size = fileLen;
		return classFile;
	}
	
	vmFree(classFile);
	return NULL;
}

ClassFile *readClassFileZipEntry(const char * zippath, const char *classpath)
{
	vmAssert(classpath != NULL);
	vmAssert(zippath != NULL);
	
	return NULL;
}

ClassFile * readClassFileBootstrapDirEntry(const char * bootdir, const char * classpath)
{
	char fullpath[256] = { 0 };
	vmAssert(bootdir != NULL);
	vmAssert(classpath != NULL);
	
	strcat(fullpath, bootdir);
	strcat(fullpath, "/");
	strcat(fullpath, classpath);

	stringReplaceDot(fullpath, fullpath, strlen(fullpath));

	if (stringHasSuffix(fullpath, ".class") != 0)
		strcat(fullpath, ".class");

	return readClassFileDirEntry(fullpath);	
}


ClassFile * loadClassFile(ClassLoader * classLoader, const char * classname)
{
	ClassFile * classFile = NULL;
	
	if (classLoader->classpath != NULL)
		classFile = readClassFileBootstrapDirEntry(classLoader->classpath, classname);
	else if (classLoader->zippath != NULL)
		classFile = readClassFileBootstrapDirEntry(classLoader->classpath, classname);
	else
	{

	}
	
	return classFile;
}