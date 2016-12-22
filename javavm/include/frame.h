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


OperandStack * newOperandStack(uint32_t maxStack);
void freeOperandStack(OperandStack* operandStack);

void pushOperandInt(OperandStack* operandStack, int32_t val);
int32_t popOperandInt(OperandStack* operandStack);

void pushOperandFloat(OperandStack* operandStack, float val);
float popOperandFloat(OperandStack* operandStack);

void pushOperandLong(OperandStack* operandStack, int64_t val);
int64_t popOperandLong(OperandStack* operandStack);

void pushOperandDouble(OperandStack* operandStack, double val);
double popOperandDouble(OperandStack* operandStack);

void pushOperandRef(OperandStack * operandStack, Object * ref);
Object * popOperandRef(OperandStack * operandStack);

void pushOperandSlot(OperandStack * operandStack, Slot * slot);
Slot * popOperandSlot(OperandStack * operandStack);

Object * getOperandRefFromTop(OperandStack * self, uint16_t n);

void pushOperandBoolean(OperandStack* operandStack, bool val);
bool popOperandBoolean(OperandStack* operandStack);

void clearOperandStack(OperandStack * self);
#endif