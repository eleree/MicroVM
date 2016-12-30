#include <math.h>
#include "interpreter.h"

void execute_opc_nop(Frame * frame, void * operand)
{

}

void execute_opc_ldc(Frame * frame, void * operand)
{
	int32_t index = ((Operand*)operand)->b;
	OperandStack * operandStack = frame->operandStack;
	ConstantPool * cp = frame->method->classMember.attachClass->constantPool+index;
	int16_t type = cp->cpType;
	ClassLoader * classLoader = frame->method->classMember.attachClass->classLoader;
	switch (type)
	{
		case CONSTATNT_INTEGER:
			pushOperandInt(operandStack, cp->cpItem.s32);
			break;
		case CONSTATNT_FLOAT:
			pushOperandFloat(operandStack, cp->cpItem.floatVal);
			break;
		case CONSTATNT_STRING:
			//pushOperandRef(operandStack, jString(classLoader, getClassConstantPoolStringRef(cp, index)));
			break;
		case CONSTATNT_CLASS:
			break;
		default:
			break;
	}
}

void execute_opc_ldc_w(Frame * frame, void * operand)
{
	int32_t index = ((Operand*)operand)->s;
	OperandStack * operandStack = frame->operandStack;
	ConstantPool * cp = frame->method->classMember.attachClass->constantPool + index;
	int16_t type = cp->cpType;
	ClassLoader * classLoader = frame->method->classMember.attachClass->classLoader;
	switch (type)
	{
		case CONSTATNT_INTEGER:
			pushOperandInt(operandStack, cp->cpItem.s32);
			break;
		case CONSTATNT_FLOAT:
			pushOperandFloat(operandStack, cp->cpItem.floatVal);
			break;
		case CONSTATNT_STRING:
			//pushOperandRef(operandStack, jString(classLoader, getClassConstantPoolStringRef(cp, index)));
			break;
		case CONSTATNT_CLASS:
			break;
		default:
			break;
	}
}

void execute_opc_ldc2_w(Frame * frame, void * operand)
{
	int32_t index = ((Operand*)operand)->s;
	OperandStack * operandStack = frame->operandStack;
	ConstantPool * cp = frame->method->classMember.attachClass->constantPool + index;
	int16_t type = cp->cpType;
	switch (cp->cpType)
	{
		case CONSTATNT_LONG:
			pushOperandLong(operandStack, cp->cpItem.s64);
			break;
		case CONSTATNT_DOUBLE:
			pushOperandDouble(operandStack, cp->cpItem.doubleVal);
			break;
		default:
			printf("java.lang.ClassFormatError\n");
			exit(0);
			break;
	}
}

void execute_opc_pop(Frame * frame, void * operand)
{
	popOperandSlot(frame->operandStack);
}

void execute_opc_pop2(Frame * frame, void * operand)
{
	popOperandSlot(frame->operandStack);
	popOperandSlot(frame->operandStack);
}

void execute_opc_dup(Frame * frame, void * operand)
{
	Slot * slot = popOperandSlotRef(frame->operandStack);
	pushOperandSlot(frame->operandStack, slot);
	pushOperandSlot(frame->operandStack, slot);
}

void execute_opc_dup_x1(Frame * frame, void * operand)
{
	Slot slot1, slot2;
	memcpy(&slot1,popOperandSlotRef(frame->operandStack),sizeof(Slot));
	memcpy(&slot2, popOperandSlotRef(frame->operandStack), sizeof(Slot));	
	pushOperandSlot(frame->operandStack, &slot1);
	pushOperandSlot(frame->operandStack, &slot2);
	pushOperandSlot(frame->operandStack, &slot1);
}

void execute_opc_dup_x2(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Slot slot1, slot2, slot3;
	memcpy(&slot1, popOperandSlotRef(operandStack), sizeof(Slot));
	memcpy(&slot2, popOperandSlotRef(operandStack), sizeof(Slot));
	memcpy(&slot3, popOperandSlotRef(operandStack), sizeof(Slot));
	pushOperandSlot(operandStack, &slot1);
	pushOperandSlot(operandStack, &slot3);
	pushOperandSlot(operandStack, &slot2);
	pushOperandSlot(operandStack, &slot1);
}

void execute_opc_dup2(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Slot slot1, slot2;
	memcpy(&slot1, popOperandSlotRef(operandStack), sizeof(Slot));
	memcpy(&slot2, popOperandSlotRef(operandStack), sizeof(Slot));
	pushOperandSlot(operandStack, &slot2);
	pushOperandSlot(operandStack, &slot1);
	pushOperandSlot(operandStack, &slot2);
	pushOperandSlot(operandStack, &slot1);
}

void execute_opc_dup2_x1(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Slot slot1, slot2, slot3;
	memcpy(&slot1, popOperandSlotRef(operandStack), sizeof(Slot));
	memcpy(&slot2, popOperandSlotRef(operandStack), sizeof(Slot));
	memcpy(&slot3, popOperandSlotRef(operandStack), sizeof(Slot));
	pushOperandSlot(operandStack, &slot2);
	pushOperandSlot(operandStack, &slot1);
	pushOperandSlot(operandStack, &slot3);
	pushOperandSlot(operandStack, &slot2);
	pushOperandSlot(operandStack, &slot1);
}

void execute_opc_dup2_x2(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Slot slot1, slot2, slot3, slot4;
	memcpy(&slot1, popOperandSlotRef(operandStack), sizeof(Slot));
	memcpy(&slot2, popOperandSlotRef(operandStack), sizeof(Slot));
	memcpy(&slot3, popOperandSlotRef(operandStack), sizeof(Slot));
	memcpy(&slot4, popOperandSlotRef(operandStack), sizeof(Slot));
	pushOperandSlot(operandStack, &slot2);
	pushOperandSlot(operandStack, &slot1);
	pushOperandSlot(operandStack, &slot4);
	pushOperandSlot(operandStack, &slot3);
	pushOperandSlot(operandStack, &slot2);
	pushOperandSlot(operandStack, &slot1);
}

void execute_opc_swap(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Slot slot1, slot2;
	memcpy(&slot1, popOperandSlotRef(operandStack), sizeof(Slot));
	memcpy(&slot2, popOperandSlotRef(operandStack), sizeof(Slot));
	pushOperandSlot(operandStack, &slot1);
	pushOperandSlot(operandStack, &slot2);
}

void execute_opc_iadd(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v1 = popOperandInt(operandStack);
	int32_t v2 = popOperandInt(operandStack);
	int32_t result = v1 + v2;
	pushOperandInt(operandStack, result);
}

void execute_opc_ladd(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t v1 = popOperandLong(operandStack);
	int64_t v2 = popOperandLong(operandStack);
	int64_t result = v1 + v2;
	pushOperandLong(operandStack, result);
}

void execute_opc_fadd(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	float v1 = popOperandFloat(operandStack);
	float v2 = popOperandFloat(operandStack);
	float result = v1 + v2;
	pushOperandFloat(operandStack, result);
}

void execute_opc_dadd(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	double v1 = popOperandDouble(operandStack);
	double v2 = popOperandDouble(operandStack);
	double result = v1 + v2;
	pushOperandDouble(operandStack, result);
}

void execute_opc_isub(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);
	int32_t result = v1 - v2;
	pushOperandInt(operandStack, result);
}

void execute_opc_lsub(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t v2 = popOperandLong(operandStack);
	int64_t v1 = popOperandLong(operandStack);
	int64_t result = v1 - v2;
	pushOperandLong(operandStack, result);
}

void execute_opc_fsub(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	float v2 = popOperandFloat(operandStack);
	float v1 = popOperandFloat(operandStack);
	float result = v1 - v2;
	pushOperandFloat(operandStack, result);
}

void execute_opc_dsub(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	double v2 = popOperandDouble(operandStack);
	double v1 = popOperandDouble(operandStack);
	double result = v1 - v2;
	pushOperandDouble(operandStack, result);
}

void execute_opc_imul(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);
	int32_t result = v1 * v2;
	pushOperandInt(operandStack, result);
}

void execute_opc_lmul(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t v2 = popOperandLong(operandStack);
	int64_t v1 = popOperandLong(operandStack);
	int64_t result = v1 * v2;
	pushOperandLong(operandStack, result);
}

void execute_opc_fmul(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	float v2 = popOperandFloat(operandStack);
	float v1 = popOperandFloat(operandStack);
	float result = v1 * v2;
	pushOperandFloat(operandStack, result);
}

void execute_opc_dmul(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	double v2 = popOperandDouble(operandStack);
	double v1 = popOperandDouble(operandStack);
	double result = v1 * v2;
	pushOperandDouble(operandStack, result);
}

void execute_opc_idiv(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);
	if (v2 == 0)
	{
		printf("java.lang.ArithmeticException: / by zero");
		exit(1);
	}
	int32_t result = v1 / v2;
	pushOperandInt(operandStack, result);
}

void execute_opc_ldiv(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t v2 = popOperandLong(operandStack);
	int64_t v1 = popOperandLong(operandStack);
	if (v2 == 0)
	{
		printf("java.lang.ArithmeticException: / by zero");
		exit(1);
	}

	int64_t result = v1 / v2;
	pushOperandLong(operandStack, result);
}

void execute_opc_fdiv(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	float v2 = popOperandFloat(operandStack);
	float v1 = popOperandFloat(operandStack);
	float result = v1 / v2;
	pushOperandFloat(operandStack, result);
}

void execute_opc_ddiv(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	double v2 = popOperandDouble(operandStack);
	double v1 = popOperandDouble(operandStack);
	double result = v1 / v2;
	pushOperandDouble(operandStack, result);
}

void execute_opc_irem(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);
	if (v2 == 0)
	{
		printf("java.lang.ArithmeticException: / by zero");
		exit(1);
	}

	int32_t result = v1 % v2;
	pushOperandInt(operandStack, result);
}

void execute_opc_lrem(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t v2 = popOperandLong(operandStack);
	int64_t v1 = popOperandLong(operandStack);
	if (v2 == 0)
	{
		printf("java.lang.ArithmeticException: / by zero");
		exit(1);
	}

	int64_t result = v1 % v2;
	pushOperandLong(operandStack, result);
}

void execute_opc_frem(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	float v2 = popOperandFloat(operandStack);
	float v1 = popOperandFloat(operandStack);
	float result = (float)fmod(v1, v2);
	pushOperandFloat(operandStack, result);
}

void execute_opc_drem(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	double v2 = popOperandDouble(operandStack);
	double v1 = popOperandDouble(operandStack);
	double result = fmod(v1, v2);
	pushOperandDouble(operandStack, result);
}

void execute_opc_ishl(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);
	uint8_t s = v2 & 0x1F;
	int32_t result = v1 << s;
	pushOperandInt(operandStack, result);
}

void execute_opc_lshl(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v2 = popOperandInt(operandStack);
	int64_t v1 = popOperandLong(operandStack);
	uint8_t s = v2 & 0x3F;
	int64_t result = v1 << s;
	pushOperandLong(operandStack, result);
}

void execute_opc_ishr(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);
	uint8_t s = v2 & 0x1F;
	int32_t result = v1 >> s;
	pushOperandInt(operandStack, result);
}

void execute_opc_lshr(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v2 = popOperandInt(operandStack);
	int64_t v1 = popOperandLong(operandStack);
	uint8_t s = v2 & 0x3F;
	int64_t result = v1 >> s;
	pushOperandLong(operandStack, result);
}

void execute_opc_iushr(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);
	uint8_t s = v2 & 0x1F;
	int32_t result = (int32_t)(((uint32_t)v1) >> s);
	pushOperandInt(operandStack, result);
}

void execute_opc_lushr(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v2 = popOperandInt(operandStack);
	int64_t v1 = popOperandLong(operandStack);
	uint8_t s = v2 & 0x3F;
	int64_t result = (int64_t)(((uint64_t)v1) >> s);
	pushOperandLong(operandStack, result);
}

void execute_opc_iand(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v1 = popOperandInt(operandStack);
	int32_t v2 = popOperandInt(operandStack);
	int32_t result = v1 & v2;
	pushOperandInt(operandStack, result);
}

void execute_opc_land(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t v1 = popOperandLong(operandStack);
	int64_t v2 = popOperandLong(operandStack);
	int64_t result = v1 & v2;
	pushOperandLong(operandStack, result);
}

void execute_opc_ior(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v1 = popOperandInt(operandStack);
	int32_t v2 = popOperandInt(operandStack);
	int32_t result = v1 | v2;
	pushOperandInt(operandStack, result);
}

void execute_opc_lor(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t v1 = popOperandLong(operandStack);
	int64_t v2 = popOperandLong(operandStack);
	int64_t result = v1 | v2;
	pushOperandLong(operandStack, result);
}

void execute_opc_ixor(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t v1 = popOperandInt(operandStack);
	int32_t v2 = popOperandInt(operandStack);
	int32_t result = v1 ^ v2;
	pushOperandInt(operandStack, result);
}

void execute_opc_lxor(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t v1 = popOperandLong(operandStack);
	int64_t v2 = popOperandLong(operandStack);
	int64_t result = v1 ^ v2;
	pushOperandLong(operandStack, result);
}

void execute_opc_iinc(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	LocalVars * localVars = frame->localVars;
	int32_t val = getLocalVarsInt(localVars, ((Operand*)operand)->iinc.Index);
	val += ((Operand*)operand)->iinc.Const;
	setLocalVarsInt(localVars, ((Operand*)operand)->iinc.Index, val);
}

void execute_opc_i2l(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t i = popOperandInt(operandStack);
	int64_t l = (int64_t)i;
	pushOperandLong(operandStack, l);
}

void execute_opc_i2f(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t i = popOperandInt(operandStack);
	float f = (float)i;
	pushOperandFloat(operandStack, f);
}

void execute_opc_i2d(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t i = popOperandInt(operandStack);
	double f = (double)i;
	pushOperandDouble(operandStack, f);
}

void execute_opc_l2i(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t l = popOperandLong(operandStack);
	int32_t i = (int32_t)l;
	pushOperandInt(operandStack, i);
}

void execute_opc_l2f(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t l = popOperandLong(operandStack);
	float f = (float)l;
	pushOperandFloat(operandStack, f);
}

void execute_opc_l2d(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t l = popOperandLong(operandStack);
	double d = (double)l;
	pushOperandDouble(operandStack, d);
}

void execute_opc_f2i(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	float f = popOperandFloat(operandStack);
	int32_t i = (int32_t)f;
	pushOperandInt(operandStack, i);
}

void execute_opc_f2l(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	float f = popOperandFloat(operandStack);
	int64_t l = (int64_t)f;
	pushOperandLong(operandStack, l);
}

void execute_opc_f2d(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	float f = popOperandFloat(operandStack);
	double d = (double)f;
	pushOperandDouble(operandStack, d);
}

void execute_opc_d2i(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	double d = popOperandDouble(operandStack);
	int32_t i = (int32_t)d;
	pushOperandInt(operandStack, i);
}

void execute_opc_d2l(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	double d = popOperandDouble(operandStack);
	int64_t l = (int64_t)d;
	pushOperandLong(operandStack, l);
}

void execute_opc_d2f(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	double d = popOperandDouble(operandStack);
	float f = (float)d;
	pushOperandFloat(operandStack, f);
}

void execute_opc_i2b(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t i = popOperandInt(operandStack);
	int8_t b = (int8_t)i;
	pushOperandInt(operandStack, b);
}

void execute_opc_i2c(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t i = popOperandInt(operandStack);
	int16_t c = (int16_t)i;
	pushOperandInt(operandStack, c);
}

void execute_opc_i2s(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int32_t i = popOperandInt(operandStack);
	int16_t s = (int16_t)i;
	pushOperandInt(operandStack, s);
}

void execute_opc_lcmp(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	int64_t v2 = popOperandLong(operandStack);
	int64_t v1 = popOperandLong(operandStack);
	if (v1 > v2)
		pushOperandInt(operandStack, 1);
	else if (v1 == v2)
		pushOperandInt(operandStack, 0);
	else
		pushOperandInt(operandStack, -1);
}

void execute_opc_fcmpl(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	bool gFlag = false;
	float v2 = popOperandFloat(operandStack);
	float v1 = popOperandFloat(operandStack);

	if (v1 > v2){
		pushOperandInt(operandStack, 1);
	}
	else if (v1 == v2){
		pushOperandInt(operandStack, 0);
	}
	else if (v1 < v2){
		pushOperandInt(operandStack, -1);
	}
	else if (gFlag){
		pushOperandInt(operandStack, 1);
	}
	else {
		pushOperandInt(operandStack, -1);
	}
}

void execute_opc_fcmpg(Frame * frame, void * operand)
{
	bool gFlag = true;
	OperandStack * operandStack = frame->operandStack;
	float v2 = popOperandFloat(operandStack);
	float v1 = popOperandFloat(operandStack);

	if (v1 > v2){
		pushOperandInt(operandStack, 1);
	}
	else if (v1 == v2){
		pushOperandInt(operandStack, 0);
	}
	else if (v1 < v2){
		pushOperandInt(operandStack, -1);
	}
	else if (gFlag){
		pushOperandInt(operandStack, 1);
	}
	else {
		pushOperandInt(operandStack, -1);
	}
}

void execute_opc_dcmpl(Frame * frame, void * operand)
{
	bool gFlag = false;
	OperandStack * operandStack = frame->operandStack;
	double v2 = popOperandDouble(operandStack);
	double v1 = popOperandDouble(operandStack);

	if (v1 > v2){
		pushOperandInt(operandStack, 1);
	}
	else if (v1 == v2){
		pushOperandInt(operandStack, 0);
	}
	else if (v1 < v2){
		pushOperandInt(operandStack, -1);
	}
	else if (gFlag){
		pushOperandInt(operandStack, 1);
	}
	else {
		pushOperandInt(operandStack, -1);
	}
}

void execute_opc_dcmpg(Frame * frame, void * operand)
{
	bool gFlag = true;
	OperandStack * operandStack = frame->operandStack;
	double v2 = popOperandDouble(operandStack);
	double v1 = popOperandDouble(operandStack);

	if (v1 > v2){
		pushOperandInt(operandStack, 1);
	}
	else if (v1 == v2){
		pushOperandInt(operandStack, 0);
	}
	else if (v1 < v2){
		pushOperandInt(operandStack, -1);
	}
	else if (gFlag){
		pushOperandInt(operandStack, 1);
	}
	else {
		pushOperandInt(operandStack, -1);
	}
}

void execute_opc_ifeq(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t val = popOperandInt(operandStack);
	if (val == 0)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_ifne(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t val = popOperandInt(operandStack);
	if (val != 0)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_iflt(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t val = popOperandInt(operandStack);
	if (val < 0)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_ifge(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t val = popOperandInt(operandStack);
	if (val >= 0)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_ifgt(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t val = popOperandInt(operandStack);
	if (val > 0)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_ifle(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t val = popOperandInt(operandStack);
	if (val <= 0)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_if_icmpeq(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);

	if (v1 == v2)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_if_icmpne(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);

	if (v1 != v2)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_if_icmplt(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);

	if (v1 < v2)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_if_icmpge(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);

	if (v1 >= v2)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_if_icmpgt(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);

	if (v1 > v2)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_if_icmple(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	int32_t v2 = popOperandInt(operandStack);
	int32_t v1 = popOperandInt(operandStack);

	if (v1 <= v2)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_if_acmpeq(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	Object * ref2 = popOperandRef(operandStack);
	Object * ref1 = popOperandRef(operandStack);
	if (ref2 == ref1)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_if_acmpne(Frame * frame, void * operand)
{
	OperandStack * operandStack = frame->operandStack;
	Operand * opernadData = operand;
	Object * ref2 = popOperandRef(operandStack);
	Object * ref1 = popOperandRef(operandStack);
	if (ref2 != ref1)
		frame->nextpc = frame->nextpc + (int16_t)opernadData->s;
}

void execute_opc_invokestatic(Frame * frame, void * operand)
{

}

//jvms 5.4.3.3.Method Resolution
void execute_INVOKE_STATIC(struct Frame * frame, uint16_t index)
{
	OperandStack * operandStack = frame->operandStack;

	MethodRef * methodRef = getClassConstantPoolMethodRef(frame->method->classMember.attachClass, index);

	MethodBlock * method = resolveMethod(methodRef);

	if (!isMethodStatic(method))
	{
		printf("java.lang.IncompatibleClassChangeError\n");
		exit(131);
	}

	Class * c = method->classMember.attachClass;

	if (!c->initStarted)
	{
		//revertFrameNextPC(frame);		
		frame->nextpc = frame->thread->pc;
		initClass(frame->thread, c);
	}

	printf("Invode classs:%s method:%s,desc:%s\n", methodRef->symbolicRef.className, methodRef->name, methodRef->descriptor);
	InvokeMethod(frame, method);
}

// Return void from method
int32_t execute_RETURN(Frame * frame)
{
	Frame * currentFrame = popThreadFrame(frame->thread);
	freeFrame(currentFrame);
	return 0;
}

// Return int from method
int32_t execute_IRETURN(Frame * frame)
{
	Thread * thread = frame->thread;
	OperandStack * operandStack = frame->operandStack;

	Frame * currentFrame = popThreadFrame(thread);
	Frame * invokerFrame = getCurrentFrame(thread);

	int32_t val = popOperandInt(currentFrame->operandStack);

	freeFrame(currentFrame);

	pushOperandInt(invokerFrame->operandStack, val);

	return 0;
}

int32_t execute_LRETURN(Frame * frame)
{
	Thread * thread = frame->thread;
	OperandStack * operandStack = frame->operandStack;

	Frame * currentFrame = popThreadFrame(thread);
	Frame * invokerFrame = getCurrentFrame(thread);

	int64_t val = popOperandLong(currentFrame->operandStack);

	freeFrame(currentFrame);
	pushOperandLong(invokerFrame->operandStack, val);

	return 0;
}

// Return float from method
int32_t execute_FRETURN(Frame * frame)
{
	Thread * thread = frame->thread;
	OperandStack * operandStack = frame->operandStack;

	Frame * currentFrame = popThreadFrame(thread);
	Frame * invokerFrame = getCurrentFrame(thread);

	float val = popOperandFloat(currentFrame->operandStack);

	freeFrame(currentFrame);

	pushOperandFloat(invokerFrame->operandStack, val);

	return 0;
}

// Return double from method
int32_t execute_DRETURN(Frame * frame)
{
	Thread * thread = frame->thread;
	OperandStack * operandStack = frame->operandStack;

	Frame * currentFrame = popThreadFrame(thread);
	Frame * invokerFrame = getCurrentFrame(thread);

	double val = popOperandDouble(currentFrame->operandStack);

	freeFrame(currentFrame);

	pushOperandDouble(invokerFrame->operandStack, val);

	return 0;
}

// Invoke instance method; dispatch based on class
void _println(OperandStack * operandStack, const char * descriptor)
{
	if (strncmp(descriptor, "(Z)V", 4) == 0)
	{
		printf("%s\n", popOperandInt(operandStack) ? "true" : "fasle");
	}
	else if (strncmp(descriptor, "(C)V", 4) == 0)
	{
		printf("%c\n", popOperandInt(operandStack));
	}
	else if (strncmp(descriptor, "(I)V", 4) == 0)
	{
		printf("%d\n", popOperandInt(operandStack));
	}
	else if (strncmp(descriptor, "(B)V", 4) == 0)
	{
		printf("%d\n", popOperandInt(operandStack));
	}
	else if (strncmp(descriptor, "(S)V", 4) == 0)
	{
		printf("%d\n", popOperandInt(operandStack));
	}
	else if (strncmp(descriptor, "(F)V", 4) == 0)
	{
		printf("%f\n", popOperandFloat(operandStack));
	}
	else if (strncmp(descriptor, "(J)V", 4) == 0)
	{
		printf("%lld\n", popOperandLong(operandStack));
	}
	else if (strncmp(descriptor, "(D)V", 4) == 0)
	{
		printf("%llf\n", popOperandDouble(operandStack));
	}
	else
	{
		//printf("println:%s\n", descriptor);
	}
	if (strcmp(descriptor, "(Ljava/lang/String;)V") == 0)
	{
		//Object * jStr = popOperandRef(operandStack);
		//String * x = goString(jStr);
		//printf("println:%s\n", x->data);
	}

	popOperandRef(operandStack);
}

int32_t execute_opc_invokevirtual(Frame * frame, void * operand)
{
	uint16_t cpIndex = ((Operand*)operand)->s;
	OperandStack * operandStack = frame->operandStack;
	MethodRef * methodRef = getClassConstantPoolMethodRef(frame->method->classMember.attachClass, cpIndex);
	MethodBlock * method = resolveMethod(methodRef);
	Class * currentClass = frame->method->classMember.attachClass;

	if (isMethodStatic(method))
	{
		printf("java.lang.IncompatibleClassChangeError\n");
		exit(134);
	}

	Object * ref = getOperandRefFromTop(frame->operandStack, method->argSlotCount - 1);
	if (ref == NULL)
	{
		// hack!
		if (strcmp(methodRef->name, "println") == 0)
		{
			_println(frame->operandStack, methodRef->descriptor);
			return 0;
		}
		printf("invoke methodRef:%s %s %s\n", methodRef->name, methodRef->descriptor, methodRef->symbolicRef.className);
		printf("java.lang.NullPointerException\n");
		exit(135);
	}

	/*
	if (isMethodProtected(method) &&
		isClassSuperClassOf(method->classMember.attachClass, currentClass) &&
		strcmp(method->classMember.attachClass->packageName, currentClass->packageName) != 0 &&
		ref->class != currentClass &&
		!isSubClassOf(ref->class, currentClass)){

		if (!isClassArray(ref->class) && strcmp(method->classMember.name, "clone") == 0)
		{
			printf("java.lang.IllegalAccessError\n");
			exit(200);
		}
	}

	MethodBlock * methodToBeInvoked = lookupMethodInClass(ref->class, methodRef->name,
		methodRef->descriptor);

	if (methodToBeInvoked == NULL || isMethodAbstract(methodToBeInvoked))
	{
		printf("java.lang.AbstractMethodError\n");
		exit(136);
	}
	
	InvokeMethod(frame, methodToBeInvoked);
	*/
	return 0;
}
