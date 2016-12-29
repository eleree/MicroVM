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

typedef struct Frame  {
	struct Frame * lower; // stack is implemented as linked list
	struct LocalVars * localVars;
	struct OperandStack * operandStack;
	struct Thread * thread;
	int32_t nextpc;
	int32_t lastpc;
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


LocalVars * newLocalVars(uint32_t maxLocals);
void freeLocalVars(LocalVars * localVars);

void setLocalVarsInt(LocalVars * localVars, int32_t index, int32_t val);
int32_t getLocalVarsInt(LocalVars * localVars, int32_t index);

void setLocalVarsFloat(LocalVars * localVars, int32_t index, float val);
float getLocalVarsFloat(LocalVars * localVars, int32_t index);

void setLocalVarsLong(LocalVars * localVars, int32_t index, int64_t val);
int64_t getLocalVarsLong(LocalVars * localVars, int64_t index);

void setLocalVarsDouble(LocalVars * localVars, int32_t index, double val);
double getLocalVarsDouble(LocalVars * localVars, int32_t index);

void setLocalVarsRef(LocalVars * localVars, int32_t index, Object * ref);
Object * getLocalVarsRef(LocalVars * localVars, int32_t index);

Object * getLocalVarsRef(LocalVars * localVars, int32_t index);
void setLocalVarsSlot(LocalVars * localVars, int32_t index, Slot * slot);

Object * getLocalVarsThis(LocalVars * self);
bool getLocalVarsBoolean(LocalVars * self, int32_t index);

Frame * newFrame(struct Thread * thread, struct MethodBlock * method, uint32_t maxLocals, uint32_t maxStack);
void freeFrame(Frame * frame);

#endif