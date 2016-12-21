#ifndef __MICRO_VM_FRAME_H__
#define __MICRO_VM_FRAME_H__

#include <vm.h>
typedef struct Stack{
	union{
		uintptr_t * stack;
	};
}Stack;
typedef struct  Frame  {
	struct Frame * lower; // stack is implemented as linked list
	struct LocalVars * localVars;
	struct Stack * operandStack;
	struct Thread * thread;
	int32_t pc;
	struct Method * method;	
}Frame;

#endif