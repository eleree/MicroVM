#ifndef __MICRO_VM_ASSERT_H__
#define __MICRO_VM_ASSERT_H__ 

#include <stdio.h>
#include <stdlib.h>
#include "porting\config.h"

#ifdef MICRO_VM_DEBUG_ASSERT
#define vmAssert(expression) \
	do {\
		if (!(expression)){ printf("Assertion Fail:%s,File:%s,Line:%d\n", #expression, __FILE__, __LINE__); \
		system("pause");\
		exit(-1);}\
	}while (0);
#else
#define vmAssert(expression)
#endif

#endif