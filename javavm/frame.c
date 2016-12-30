#include "frame.h"
#include "thread.h"
LocalVars * newLocalVars(uint32_t maxLocals)
{
	LocalVars * localVars = NULL;
	if (maxLocals == 0)
		return NULL;
	localVars = vmCalloc(1, sizeof(LocalVars)+sizeof(Slot)*maxLocals);
	localVars->size = maxLocals;
	return localVars;
}

void freeLocalVars(LocalVars * localVars)
{
	if (localVars != NULL)
	{
		if (localVars->size > 0)
			vmFree(localVars->slots);
		vmFree(localVars);
	}
}

void setLocalVarsInt(LocalVars * localVars, int32_t index, int32_t val)
{
	localVars->slots[index].num = val;
}

int32_t getLocalVarsInt(LocalVars * localVars, int32_t index)
{
	return localVars->slots[index].num;
}

void setLocalVarsFloat(LocalVars * localVars, int32_t index, float val)
{
	FloatInt transData;
	transData.floatData = val;
	localVars->slots[index].num = transData.bitsData;
}

float getLocalVarsFloat(LocalVars * localVars, int32_t index)
{
	FloatInt transData;
	transData.bitsData = localVars->slots[index].num;
	return transData.floatData;
}

void setLocalVarsLong(LocalVars * localVars, int32_t index, int64_t val)
{
	localVars->slots[index].num = val & 0xFFFFFFFF;
	localVars->slots[index + 1].num = (uint32_t)(val >> 32);
}

int64_t getLocalVarsLong(LocalVars * localVars, int64_t index)
{
	LongInt transData;

	transData.high = localVars->slots[index + 1].num;
	transData.low = localVars->slots[index].num;
	return transData.longData;
}

void setLocalVarsDouble(LocalVars * localVars, int32_t index, double val)
{
	DoubleLong transData;
	transData.doubleData = val;
	setLocalVarsLong(localVars, index, transData.bitsData);
}

double getLocalVarsDouble(LocalVars * localVars, int32_t index)
{
	DoubleLong transData;

	transData.bitsData = getLocalVarsLong(localVars, index);

	return transData.doubleData;
}

void setLocalVarsRef(LocalVars * localVars, int32_t index, Object * ref)
{
	localVars->slots[index].ref = ref;
}

Object * getLocalVarsRef(LocalVars * localVars, int32_t index)
{
	return localVars->slots[index].ref;
}

void setLocalVarsSlot(LocalVars * localVars, int32_t index, Slot * slot)
{
	localVars->slots[index].num = slot->num;
	localVars->slots[index].ref = slot->ref;
}

Object * getLocalVarsThis(LocalVars * self)
{
	return getLocalVarsRef(self, 0);
}

bool getLocalVarsBoolean(LocalVars * self, int32_t index)
{
	return getLocalVarsInt(self, index) == 1;
}

OperandStack * newOperandStack(uint32_t maxStack)
{
	OperandStack * operandStack = NULL;

	if (maxStack == 0)
		return NULL;

	operandStack = vmCalloc(1, sizeof(OperandStack)+sizeof(Slot)*maxStack);
	operandStack->maxSize = maxStack;
	operandStack->size = 0;
	return operandStack;
}

void freeOperandStack(OperandStack* operandStack)
{
	if (operandStack != NULL)
	{
		if (operandStack->maxSize > 0)
			vmFree(operandStack->slots);
		vmFree(operandStack);
	}
}

void pushOperandInt(OperandStack* operandStack, int32_t val)
{
	uint32_t operandStackSize = operandStack->size;
	operandStack->slots[operandStackSize].num = val;
	operandStack->size++;
}

int32_t popOperandInt(OperandStack* operandStack)
{
	operandStack->size--;
	return operandStack->slots[operandStack->size].num;
}

void pushOperandBoolean(OperandStack* operandStack, bool val)
{
	if (val)
		pushOperandInt(operandStack, 1);
	else
		pushOperandInt(operandStack, 0);
}

bool popOperandBoolean(OperandStack* operandStack)
{
	return popOperandInt(operandStack) == 1;
}

void pushOperandFloat(OperandStack* operandStack, float val)
{
	FloatInt transData;
	transData.floatData = val;
	operandStack->slots[operandStack->size].num = transData.bitsData;
	operandStack->size++;
}

float popOperandFloat(OperandStack* operandStack)
{
	FloatInt transData;
	operandStack->size--;
	transData.bitsData = operandStack->slots[operandStack->size].num;
	return transData.floatData;
}

void pushOperandLong(OperandStack* operandStack, int64_t val)
{
	operandStack->slots[operandStack->size].num = (uint32_t)val;
	operandStack->slots[operandStack->size + 1].num = (uint32_t)(val >> 32);
	operandStack->size += 2;
}

int64_t popOperandLong(OperandStack* operandStack)
{
	LongInt transData;

	operandStack->size -= 2;
	transData.high = operandStack->slots[operandStack->size + 1].num;
	transData.low = operandStack->slots[operandStack->size].num;

	return transData.longData;
}

void pushOperandDouble(OperandStack* operandStack, double val)
{
	DoubleLong transData;
	transData.doubleData = val;
	pushOperandLong(operandStack, transData.bitsData);
}

double popOperandDouble(OperandStack* operandStack)
{
	DoubleLong transData;
	transData.bitsData = popOperandLong(operandStack);
	return  transData.doubleData;
}

void pushOperandRef(OperandStack * operandStack, Object * ref)
{
	operandStack->slots[operandStack->size].ref = ref;
	operandStack->size++;
}

Object * popOperandRef(OperandStack * operandStack)
{
	operandStack->size--;
	return operandStack->slots[operandStack->size].ref;
}

void pushOperandSlot(OperandStack * operandStack, Slot * slot)
{
	operandStack->slots[operandStack->size].num = slot->num;
	operandStack->slots[operandStack->size].ref = slot->ref;
	operandStack->size++;
}

void popOperandSlot(OperandStack * operandStack)
{
	operandStack->size--;
}

Slot * popOperandSlotRef(OperandStack * operandStack)
{
	operandStack->size--;
	return &operandStack->slots[operandStack->size];	
}

Object * getOperandRefFromTop(OperandStack * self, uint16_t n)
{
	return self->slots[self->size - 1 - n].ref;
}

void clearOperandStack(OperandStack * self)
{
	self->size = 0;
	for (uint32_t i = 0; i < self->maxSize; i++)
	{
		self->slots[i].ref = NULL;
	}
}


Frame * newFrame(Thread * thread, struct MethodBlock * method, uint32_t maxLocals, uint32_t maxStack)
{
	Frame * frame = vmCalloc(1, sizeof(Frame));
	frame->localVars = newLocalVars(maxLocals);
	frame->operandStack = newOperandStack(maxStack);
	frame->thread = thread;
	frame->method = method;
	return frame;
}

LocalVars * getFrameLocalVars(Frame * frame)
{
	return frame->localVars;
}

OperandStack * getFrameOperandStack(Frame * frame)
{
	return frame->operandStack;
}

int32_t getFrameNextPC(Frame *frame)
{
	return frame->nextpc;
}

void setFrameNextPC(Frame *frame, int32_t nextPC)
{
	frame->nextpc = nextPC;
}

void revertFrameNextPC(Frame * frame)
{
	frame->nextpc = frame->thread->pc;
}

void freeFrame(Frame * frame)
{
	if (frame != NULL)
	{
		freeLocalVars(frame->localVars);
		freeOperandStack(frame->operandStack);
		vmFree(frame);
	}
}
