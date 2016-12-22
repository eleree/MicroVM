#include "thread.h"

#if 0
void pushFrame(FrameStack * stack, Frame * frame)
{
	if (stack->size >= stack->maxSize)
	{
		printf("java.lang.StackOverflowError\n");
		exit(1);
	}

	if (stack->top != NULL)
	{
		frame->lower = stack->top;
	}
	stack->top = frame;
	stack->size++;
}

Frame * popFrame(FrameStack * stack)
{
	Frame * frame = NULL;
	if (stack->top == NULL)
	{
		printf("jvm stack is empty!\n");
		exit(-1);
	}

	frame = stack->top;
	stack->top = stack->top->lower;
	stack->size--;
	return frame;
}
#endif 

uint32_t  totalFrams(FrameStack * stack)
{
	uint32_t i = 0;
	Frame * frame = stack->top;
	while (frame != NULL)
	{
		i++;
		frame = frame->lower;
	}
	return i;
}

Frame * topFrame(FrameStack * stack)
{
	return stack->top;
}

void freeStack(FrameStack * stack)
{
	if (stack != NULL)
		vmFree(stack);
}

bool isStackEmpty(FrameStack * stack)
{
	return stack->top == NULL;
}

#if 0
void clearStack(FrameStack* stack)
{
	while (!isStackEmpty(stack))
		popFrame(stack);
}
#endif

FrameStack * newFrameStack(uint32_t maxSize)
{
	FrameStack * stack = vmCalloc(1, sizeof(FrameStack));
	stack->maxSize = maxSize;
	return stack;
}

Thread * newThread(void)
{
	Thread  * thread = NULL;
	thread = calloc(1, sizeof(Thread));
	thread->stack = newFrameStack(1024);
	return thread;
}

int32_t getThreadPC(Thread * thread)
{
	return thread->pc;
}

void setThreadPC(Thread * thread, int32_t pc)
{
	thread->pc = pc;
}

void pushThreadFrame(Thread * thread, Frame * frame)
{
	FrameStack * stack = thread->stack;
	if (stack->size >= stack->maxSize)
	{
		printf("java.lang.StackOverflowError\n");
		exit(1);
	}

	if (stack->top != NULL)
	{
		frame->lower = stack->top;
	}
	stack->top = frame;
	stack->size++;
}

Frame * popThreadFrame(Thread * thread)
{
	Frame * frame = NULL;
	FrameStack * stack = thread->stack;
	if (stack->top == NULL)
	{
		printf("jvm stack is empty!\n");
		exit(-1);
	}

	frame = stack->top;
	stack->top = stack->top->lower;
	stack->size--;
	return frame;
}

Frame * getCurrentFrame(Thread * thread)
{
	return topFrame(thread->stack);
}

bool isThreadStackEmpty(Thread * thread)
{
	return thread->stack->top == NULL;
}

Frame* * getThreadFrames(Thread * thread, int32_t skip, int32_t * remain)
{
	/* count total frame */
	Frame * top = thread->stack->top;

	*remain = thread->stack->size - skip;
	if (skip >= (int32_t)thread->stack->size)
	{
		printf("getThreadFrames skip:%d > stackSize:%d\n", skip, thread->stack->size);
		exit(200);
	}

	Frame * *frames = vmCalloc(thread->stack->size, sizeof(Frame*));

	for (int32_t i = 0; i < skip; i++)
		top = top->lower;
	for (int32_t i = 0; i < *remain; i++)
	{
		frames[i] = top;
		top = top->lower;
	}
	return frames;
}

void clearThreadStack(Thread *thread)
{
	while (!isStackEmpty(thread->stack))
		popThreadFrame(thread);
}