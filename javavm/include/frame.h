#ifndef __MICRO_VM_FRAME_H__
#define __MICRO_VM_FRAME_H__

#include <vm.h>
#include "class.h"

typedef union Slot{
	int32_t num;
	Object * ref;
}Slot;

typedef struct LocalVars
{
	uint32_t size;
	Slot slots[0];
}LocalVars;

typedef struct OperandStack{
	uint32_t maxSize;
	uint32_t size;
	Slot  slots[0];
}OperandStack;

typedef struct  Frame  {
	struct Frame * lower; // stack is implemented as linked list
	struct LocalVars * localVars;
	struct OperandStack * operandStack;
	struct Thread * thread;
	int32_t nextpc;
	struct MethodBlock * method;	
}Frame;

#endif