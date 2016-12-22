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

typedef union FloatInt
{
	uint32_t bitsData;
	float floatData;
}FloatInt;

typedef union LongInt
{
	struct bitsData{
		uint32_t low;
		uint32_t high;
	};
	int64_t  longData;
}LongInt;

typedef union DoubleLong
{
	uint64_t bitsData;
	double doubleData;
}DoubleLong;


Thread * newThread(void);
Frame * getCurrentFrame(Thread * thread);
int32_t getFrameNextPC(Frame *frame);

#endif