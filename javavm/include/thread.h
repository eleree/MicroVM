#ifndef __MICRO_VM_THREAD_H__
#define __MICRO_VM_THREAD_H__ 

#include <vm.h>
#include <frame.h>

typedef struct FrameStack  {
	uint32_t maxSize;
	uint32_t size;
	Frame * top;// stack is implemented as linked list
}FrameStack;

typedef struct Thread
{
	int32_t pc;   
	FrameStack *stack;	
}Thread;



Thread * newThread(void);
void pushThreadFrame(Thread * thread, Frame * frame);
Frame * getCurrentFrame(Thread * thread);
int32_t getFrameNextPC(Frame *frame);

#endif