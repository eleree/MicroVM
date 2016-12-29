#include "class.h"
#include "thread.h"
#include "include\opcodes.h"
#include "frame.h"
#include "include\interpreter.h"
#include <math.h>

void InvokeMethod(Frame * invokerFrame, MethodBlock * method) {
	Thread * thread = invokerFrame->thread;
	Frame * frame = newFrame(thread, method, method->maxLocals, method->maxStack);
	pushThreadFrame(thread, frame);

	int32_t argSlotSlot = method->argSlotCount;
	if (argSlotSlot > 0)
	{
		for (int32_t i = argSlotSlot - 1; i >= 0; i--)
		{
			Slot * slot = popOperandSlot(invokerFrame->operandStack);
			setLocalVarsSlot(frame->localVars, i, slot);
		}
	}
}


void processOpcode(Frame * frame, uint8_t opcode, uint8_t * operandBytes, uint8_t opcodeLen)
{
	Operand operand = { 0 };

	switch (opcodeLen)
	{
	case 2:
		operand.b = *operandBytes;
		break;
	case 3:
		if (opcode != opc_iinc)
			operand.s = (*operandBytes) << 8 | *(operandBytes + 1);
		else
		{
			operand.iinc.Index = *operandBytes;
			operand.iinc.Const = *(operandBytes + 1);
		}
	default:
		break;
	}

	OperandStack * operandStack = frame->operandStack;
	LocalVars * localVars = frame->localVars;

#define CASE_OPCODE(label,frame,operand)\
	void execute_##label(Frame *, void *); \
	case label:\
	execute_##label(frame, operand); \
	break;

#define CASE_SIMPLE_OPCODE(label,function)\
	case label:\
	{function;}break;

	switch (opcode)
	{
		CASE_OPCODE(opc_nop, frame, NULL);
		CASE_SIMPLE_OPCODE(opc_aconst_null, pushOperandRef(frame->operandStack, NULL));
		CASE_SIMPLE_OPCODE(opc_iconst_m1, pushOperandInt(frame->operandStack, -1));
		CASE_SIMPLE_OPCODE(opc_iconst_0, pushOperandInt(frame->operandStack, 0));
		CASE_SIMPLE_OPCODE(opc_iconst_1, pushOperandInt(frame->operandStack, 1));
		CASE_SIMPLE_OPCODE(opc_iconst_2, pushOperandInt(frame->operandStack, 2));
		CASE_SIMPLE_OPCODE(opc_iconst_3, pushOperandInt(frame->operandStack, 3));
		CASE_SIMPLE_OPCODE(opc_iconst_4, pushOperandInt(frame->operandStack, 4));
		CASE_SIMPLE_OPCODE(opc_iconst_5, pushOperandInt(frame->operandStack, 5));
		CASE_SIMPLE_OPCODE(opc_lconst_0, pushOperandLong(frame->operandStack, 0));
		CASE_SIMPLE_OPCODE(opc_lconst_1, pushOperandLong(frame->operandStack, 1));
		CASE_SIMPLE_OPCODE(opc_fconst_0, pushOperandFloat(frame->operandStack, 0));
		CASE_SIMPLE_OPCODE(opc_fconst_1, pushOperandFloat(frame->operandStack, 1));
		CASE_SIMPLE_OPCODE(opc_fconst_2, pushOperandFloat(frame->operandStack, 2));
		CASE_SIMPLE_OPCODE(opc_dconst_0, pushOperandDouble(frame->operandStack, 0));
		CASE_SIMPLE_OPCODE(opc_dconst_1, pushOperandDouble(frame->operandStack, 1));
		CASE_SIMPLE_OPCODE(opc_bipush, pushOperandInt(frame->operandStack, operand.b));
		CASE_SIMPLE_OPCODE(opc_sipush, pushOperandInt(frame->operandStack, operand.s));
		CASE_OPCODE(opc_ldc, frame, &operand);
		CASE_OPCODE(opc_ldc_w, frame, &operand);
		CASE_OPCODE(opc_ldc2_w, frame, &operand);
		CASE_SIMPLE_OPCODE(opc_iload, pushOperandInt(operandStack, getLocalVarsInt(localVars, operand.b)));
		CASE_SIMPLE_OPCODE(opc_lload, pushOperandLong(operandStack, getLocalVarsLong(localVars, operand.b)));			
		CASE_SIMPLE_OPCODE(opc_fload, pushOperandFloat(operandStack, getLocalVarsFloat(localVars, operand.b)));
		CASE_SIMPLE_OPCODE(opc_dload, pushOperandDouble(operandStack, getLocalVarsDouble(localVars, operand.b)));
		CASE_SIMPLE_OPCODE(opc_aload, pushOperandRef(operandStack, getLocalVarsRef(localVars, operand.b)));
		CASE_SIMPLE_OPCODE(opc_iload_0,	pushOperandInt(operandStack, getLocalVarsInt(localVars, 0)));
		CASE_SIMPLE_OPCODE(opc_iload_1,	pushOperandInt(operandStack, getLocalVarsInt(localVars, 1)));
		CASE_SIMPLE_OPCODE(opc_iload_2,	pushOperandInt(operandStack, getLocalVarsInt(localVars, 2)));
		CASE_SIMPLE_OPCODE(opc_iload_3,	pushOperandInt(operandStack, getLocalVarsInt(localVars, 3)));
		CASE_SIMPLE_OPCODE(opc_lload_0,	pushOperandLong(operandStack, getLocalVarsLong(localVars, 0)));
		CASE_SIMPLE_OPCODE(opc_lload_1,	pushOperandLong(operandStack, getLocalVarsLong(localVars, 1)));		
		CASE_SIMPLE_OPCODE(opc_lload_2,	pushOperandLong(operandStack, getLocalVarsLong(localVars, 2)));		
		CASE_SIMPLE_OPCODE(opc_lload_3, pushOperandLong(operandStack, getLocalVarsLong(localVars, 3)));
		CASE_SIMPLE_OPCODE(opc_fload_0,	pushOperandFloat(operandStack, getLocalVarsFloat(localVars, 0)));
		CASE_SIMPLE_OPCODE(opc_fload_1, pushOperandFloat(operandStack, getLocalVarsFloat(localVars, 1)));
		CASE_SIMPLE_OPCODE(opc_fload_2,	pushOperandFloat(operandStack, getLocalVarsFloat(localVars, 2)));
		CASE_SIMPLE_OPCODE(opc_fload_3,	pushOperandFloat(operandStack, getLocalVarsFloat(localVars, 3)));
		CASE_SIMPLE_OPCODE(opc_dload_0,	pushOperandDouble(operandStack, getLocalVarsDouble(localVars, 0)));
		CASE_SIMPLE_OPCODE(opc_dload_1, pushOperandDouble(operandStack, getLocalVarsDouble(localVars, 1)));
		CASE_SIMPLE_OPCODE(opc_dload_2,	pushOperandDouble(operandStack, getLocalVarsDouble(localVars, 2)));
		CASE_SIMPLE_OPCODE(opc_dload_3,	pushOperandDouble(operandStack, getLocalVarsDouble(localVars, 3)));		
		CASE_SIMPLE_OPCODE(opc_aload_0,	pushOperandRef(operandStack, getLocalVarsRef(localVars, 0)));
		CASE_SIMPLE_OPCODE(opc_aload_1,	pushOperandRef(operandStack, getLocalVarsRef(localVars, 1)));
		CASE_SIMPLE_OPCODE(opc_aload_2,	pushOperandRef(operandStack, getLocalVarsRef(localVars, 2)));		
		CASE_SIMPLE_OPCODE(opc_aload_3,	pushOperandRef(operandStack, getLocalVarsRef(localVars, 3)));
	case opc_iaload:
		break;
	case opc_laload:
		break;
	case opc_faload:
		break;
	case opc_daload:
		break;
	case opc_aaload:
		break;
	case opc_baload:
		break;
	case opc_caload:
		break;
	case opc_saload:
		break;
		CASE_SIMPLE_OPCODE(opc_istore, setLocalVarsInt(localVars, operand.b, popOperandInt(operandStack)));
		CASE_SIMPLE_OPCODE(opc_lstore, setLocalVarsLong(localVars, operand.b, popOperandLong(operandStack)));
		CASE_SIMPLE_OPCODE(opc_fstore, setLocalVarsFloat(localVars, operand.b, popOperandFloat(operandStack)));
		CASE_SIMPLE_OPCODE(opc_dstore, setLocalVarsDouble(localVars, operand.b, popOperandDouble(operandStack)));		
		CASE_SIMPLE_OPCODE(opc_astore, setLocalVarsRef(localVars, operand.b, popOperandRef(operandStack)));
		CASE_SIMPLE_OPCODE(opc_istore_0, setLocalVarsInt(localVars, 0, popOperandInt(operandStack)));
		CASE_SIMPLE_OPCODE(opc_istore_1, setLocalVarsInt(localVars, 1, popOperandInt(operandStack)));
		CASE_SIMPLE_OPCODE(opc_istore_2, setLocalVarsInt(localVars, 2, popOperandInt(operandStack)));		
		CASE_SIMPLE_OPCODE(opc_istore_3, setLocalVarsInt(localVars, 3, popOperandInt(operandStack)));		
		CASE_SIMPLE_OPCODE(opc_lstore_0, setLocalVarsLong(localVars, 0, popOperandLong(operandStack)));
		CASE_SIMPLE_OPCODE(opc_lstore_1, setLocalVarsLong(localVars, 1, popOperandLong(operandStack)));
		CASE_SIMPLE_OPCODE(opc_lstore_2, setLocalVarsLong(localVars, 2, popOperandLong(operandStack)));
		CASE_SIMPLE_OPCODE(opc_lstore_3, setLocalVarsLong(localVars, 3, popOperandLong(operandStack)));
		CASE_SIMPLE_OPCODE(opc_fstore_0, setLocalVarsFloat(localVars, 0, popOperandFloat(operandStack)));		
		CASE_SIMPLE_OPCODE(opc_fstore_1, setLocalVarsFloat(localVars, 1, popOperandFloat(operandStack)));
		CASE_SIMPLE_OPCODE(opc_fstore_2, setLocalVarsFloat(localVars, 2, popOperandFloat(operandStack)));
		CASE_SIMPLE_OPCODE(opc_fstore_3, setLocalVarsFloat(localVars, 3, popOperandFloat(operandStack)));
		CASE_SIMPLE_OPCODE(opc_dstore_0, setLocalVarsDouble(localVars, 0, popOperandDouble(operandStack)));
		CASE_SIMPLE_OPCODE(opc_dstore_1, setLocalVarsDouble(localVars, 1, popOperandDouble(operandStack)));
		CASE_SIMPLE_OPCODE(opc_dstore_2, setLocalVarsDouble(localVars, 2, popOperandDouble(operandStack)));
		CASE_SIMPLE_OPCODE(opc_dstore_3, setLocalVarsDouble(localVars, 3, popOperandDouble(operandStack)));
		CASE_SIMPLE_OPCODE(opc_astore_0, setLocalVarsRef(localVars, 0, popOperandRef(operandStack)));
		CASE_SIMPLE_OPCODE(opc_astore_1, setLocalVarsRef(localVars, 1, popOperandRef(operandStack)));
		CASE_SIMPLE_OPCODE(opc_astore_2, setLocalVarsRef(localVars, 2, popOperandRef(operandStack)));
		CASE_SIMPLE_OPCODE(opc_astore_3, setLocalVarsRef(localVars, 3, popOperandRef(operandStack)));
	case opc_iastore:
		break;
	case opc_lastore:
		break;
	case opc_fastore:
		break;
	case opc_dastore:
		break;
	case opc_aastore:
		break;
	case opc_bastore:
		break;
	case opc_castore:
		break;
	case opc_sastore:
		break;
		CASE_OPCODE(opc_pop, frame, NULL);		
		CASE_OPCODE(opc_pop2, frame, NULL);
	case opc_dup:
	{
					Slot * slot = popOperandSlot(operandStack);
					pushOperandSlot(operandStack, slot);
					pushOperandSlot(operandStack, slot);
	}
		break;
	case opc_dup_x1:
	{
					   Slot * slot1 = popOperandSlot(operandStack);
					   Slot * slot2 = popOperandSlot(operandStack);
					   pushOperandSlot(operandStack, slot1);
					   pushOperandSlot(operandStack, slot2);
					   pushOperandSlot(operandStack, slot1);
	}
		break;
	case opc_dup_x2:
	{
					   Slot * slot1 = popOperandSlot(operandStack);
					   Slot * slot2 = popOperandSlot(operandStack);
					   Slot * slot3 = popOperandSlot(operandStack);
					   pushOperandSlot(operandStack, slot1);
					   pushOperandSlot(operandStack, slot3);
					   pushOperandSlot(operandStack, slot2);
					   pushOperandSlot(operandStack, slot1);
	}
		break;
	case opc_dup2:
	{
					 Slot * slot1 = popOperandSlot(operandStack);
					 Slot * slot2 = popOperandSlot(operandStack);
					 pushOperandSlot(operandStack, slot2);
					 pushOperandSlot(operandStack, slot1);
					 pushOperandSlot(operandStack, slot2);
					 pushOperandSlot(operandStack, slot1);
	}
		break;
	case opc_dup2_x1:
	{
						Slot * slot1 = popOperandSlot(operandStack);
						Slot * slot2 = popOperandSlot(operandStack);
						Slot * slot3 = popOperandSlot(operandStack);
						pushOperandSlot(operandStack, slot2);
						pushOperandSlot(operandStack, slot1);
						pushOperandSlot(operandStack, slot3);
						pushOperandSlot(operandStack, slot2);
						pushOperandSlot(operandStack, slot1);
	}
		break;
	case opc_dup2_x2:
	{
						Slot * slot1 = popOperandSlot(operandStack);
						Slot * slot2 = popOperandSlot(operandStack);
						Slot * slot3 = popOperandSlot(operandStack);
						Slot * slot4 = popOperandSlot(operandStack);
						pushOperandSlot(operandStack, slot2);
						pushOperandSlot(operandStack, slot1);
						pushOperandSlot(operandStack, slot4);
						pushOperandSlot(operandStack, slot3);
						pushOperandSlot(operandStack, slot2);
						pushOperandSlot(operandStack, slot1);
	}
		break;
	case opc_swap:
	{
					 Slot * slot1 = popOperandSlot(operandStack);
					 Slot * slot2 = popOperandSlot(operandStack);
					 pushOperandSlot(operandStack, slot1);
					 pushOperandSlot(operandStack, slot2);
	}
		break;
	case opc_iadd:
	{
					 int32_t v1 = popOperandInt(operandStack);
					 int32_t v2 = popOperandInt(operandStack);
					 int32_t result = v1 + v2;
					 pushOperandInt(operandStack, result);
	}
		break;
	case opc_ladd:
	{
					 int64_t v1 = popOperandLong(operandStack);
					 int64_t v2 = popOperandLong(operandStack);
					 int64_t result = v1 + v2;
					 pushOperandLong(operandStack, result);
	}
		break;
	case opc_fadd:
	{
					 float v1 = popOperandFloat(operandStack);
					 float v2 = popOperandFloat(operandStack);
					 float result = v1 + v2;
					 pushOperandFloat(operandStack, result);
	}
		break;
	case opc_dadd:
	{
					 double v1 = popOperandDouble(operandStack);
					 double v2 = popOperandDouble(operandStack);
					 double result = v1 + v2;
					 pushOperandDouble(operandStack, result);
	}
		break;
	case opc_isub:
	{
					 int32_t v2 = popOperandInt(operandStack);
					 int32_t v1 = popOperandInt(operandStack);
					 int32_t result = v1 - v2;
					 pushOperandInt(operandStack, result);
	}
		break;
	case opc_lsub:
	{
					 int64_t v2 = popOperandLong(operandStack);
					 int64_t v1 = popOperandLong(operandStack);
					 int64_t result = v1 - v2;
					 pushOperandLong(operandStack, result);
	}
		break;
	case opc_fsub:
	{
					 float v2 = popOperandFloat(operandStack);
					 float v1 = popOperandFloat(operandStack);
					 float result = v1 - v2;
					 pushOperandFloat(operandStack, result);
	}
		break;
	case opc_dsub:
	{
					 double v2 = popOperandDouble(operandStack);
					 double v1 = popOperandDouble(operandStack);
					 double result = v1 - v2;
					 pushOperandDouble(operandStack, result);
	}
		break;
	case opc_imul:
	{
					 int32_t v2 = popOperandInt(operandStack);
					 int32_t v1 = popOperandInt(operandStack);
					 int32_t result = v1 * v2;
					 pushOperandInt(operandStack, result);
	}
		break;
	case opc_lmul:
	{
					 int64_t v2 = popOperandLong(operandStack);
					 int64_t v1 = popOperandLong(operandStack);
					 int64_t result = v1 * v2;
					 pushOperandLong(operandStack, result);
	}
		break;
	case opc_fmul:
	{
					 float v2 = popOperandFloat(operandStack);
					 float v1 = popOperandFloat(operandStack);
					 float result = v1 * v2;
					 pushOperandFloat(operandStack, result);
	}
		break;
	case opc_dmul:
	{
					 double v2 = popOperandDouble(operandStack);
					 double v1 = popOperandDouble(operandStack);
					 double result = v1 * v2;
					 pushOperandDouble(operandStack, result);
	}
		break;
	case opc_idiv:
	{
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
		break;
	case opc_ldiv:
	{
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
		break;
	case opc_fdiv:
	{
					 float v2 = popOperandFloat(operandStack);
					 float v1 = popOperandFloat(operandStack);
					 float result = v1 / v2;
					 pushOperandFloat(operandStack, result);
	}
		break;
	case opc_ddiv:
	{
					 double v2 = popOperandDouble(operandStack);
					 double v1 = popOperandDouble(operandStack);
					 double result = v1 / v2;
					 pushOperandDouble(operandStack, result);
	}
		break;
	case opc_irem:
	{
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
		break;
	case opc_lrem:
	{
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
		break;
	case opc_frem:
	{
					 float v2 = popOperandFloat(operandStack);
					 float v1 = popOperandFloat(operandStack);
					 float result = (float)fmod(v1, v2);
					 pushOperandFloat(operandStack, result);
	}
		break;
	case opc_drem:
	{
					 double v2 = popOperandDouble(operandStack);
					 double v1 = popOperandDouble(operandStack);
					 double result = fmod(v1, v2);
					 pushOperandDouble(operandStack, result);
	}
		break;
	case opc_ineg:
	{
					 int32_t val = popOperandInt(operandStack);
					 pushOperandInt(operandStack, -val);
	}
		break;
	case opc_lneg:
	{
					 int64_t val = popOperandLong(operandStack);
					 pushOperandLong(operandStack, -val);
	}
		break;
	case opc_fneg:
	{
					 float val = popOperandFloat(operandStack);
					 pushOperandFloat(operandStack, -val);
	}
		break;
	case opc_dneg:
	{
					 double val = popOperandDouble(operandStack);
					 pushOperandDouble(operandStack, -val);
	}
		break;
	case opc_ishl:
	{
					 int32_t v2 = popOperandInt(operandStack);
					 int32_t v1 = popOperandInt(operandStack);
					 uint8_t s = v2 & 0x1F;
					 int32_t result = v1 << s;
					 pushOperandInt(operandStack, result);
	}
		break;
	case opc_lshl:
	{
					 int32_t v2 = popOperandInt(operandStack);
					 int64_t v1 = popOperandLong(operandStack);
					 uint8_t s = v2 & 0x3F;
					 int64_t result = v1 << s;
					 pushOperandLong(operandStack, result);
	}
		break;
	case opc_ishr:
	{
					 int32_t v2 = popOperandInt(operandStack);
					 int32_t v1 = popOperandInt(operandStack);
					 uint8_t s = v2 & 0x1F;
					 int32_t result = v1 >> s;
					 pushOperandInt(operandStack, result);
	}
		break;
	case opc_lshr:
	{
					 int32_t v2 = popOperandInt(operandStack);
					 int64_t v1 = popOperandLong(operandStack);
					 uint8_t s = v2 & 0x3F;
					 int64_t result = v1 >> s;
					 pushOperandLong(operandStack, result);
	}
		break;
	case opc_iushr:
	{
					  int32_t v2 = popOperandInt(operandStack);
					  int32_t v1 = popOperandInt(operandStack);
					  uint8_t s = v2 & 0x1F;
					  int32_t result = (int32_t)(((uint32_t)v1) >> s);
					  pushOperandInt(operandStack, result);
	}
		break;
	case opc_lushr:
	{
					  int32_t v2 = popOperandInt(operandStack);
					  int64_t v1 = popOperandLong(operandStack);
					  uint8_t s = v2 & 0x3F;
					  int64_t result = (int64_t)(((uint64_t)v1) >> s);
					  pushOperandLong(operandStack, result);
	}
		break;
	case opc_iand:
	{
					 int32_t v1 = popOperandInt(operandStack);
					 int32_t v2 = popOperandInt(operandStack);
					 int32_t result = v1 & v2;
					 pushOperandInt(operandStack, result);
	}
		break;
	case opc_land:
	{
					 int64_t v1 = popOperandLong(operandStack);
					 int64_t v2 = popOperandLong(operandStack);
					 int64_t result = v1 & v2;
					 pushOperandLong(operandStack, result);
	}
		break;
	case opc_ior:
	{
					int32_t v1 = popOperandInt(operandStack);
					int32_t v2 = popOperandInt(operandStack);
					int32_t result = v1 | v2;
					pushOperandInt(operandStack, result);
	}
		break;
	case opc_lor:
	{
					int64_t v1 = popOperandLong(operandStack);
					int64_t v2 = popOperandLong(operandStack);
					int64_t result = v1 | v2;
					pushOperandLong(operandStack, result);
	}
		break;
	case opc_ixor:
	{
					 int32_t v1 = popOperandInt(operandStack);
					 int32_t v2 = popOperandInt(operandStack);
					 int32_t result = v1 ^ v2;
					 pushOperandInt(operandStack, result);
	}
		break;
	case opc_lxor:
	{
					 int64_t v1 = popOperandLong(operandStack);
					 int64_t v2 = popOperandLong(operandStack);
					 int64_t result = v1 ^ v2;
					 pushOperandLong(operandStack, result);
	}
		break;
	case opc_iinc:
	{
					 LocalVars * localVars = frame->localVars;
					 int32_t val = getLocalVarsInt(localVars, operand.iinc.Index);
					 val += operand.iinc.Const;
					 setLocalVarsInt(localVars, operand.iinc.Index, val);
	}
		break;
	case opc_i2l:
	{
					int32_t i = popOperandInt(operandStack);
					int64_t l = (int64_t)i;
					pushOperandLong(operandStack, l);
	}
		break;
	case opc_i2f:
	{
					int32_t i = popOperandInt(operandStack);
					float f = (float)i;
					pushOperandFloat(operandStack, f);
	}
		break;
	case opc_i2d:
	{
					int32_t i = popOperandInt(operandStack);
					double f = (double)i;
					pushOperandDouble(operandStack, f);
	}
		break;
	case opc_l2i:
	{
					int64_t l = popOperandLong(operandStack);
					int32_t i = (int32_t)l;
					pushOperandInt(operandStack, i);
	}
		break;
	case opc_l2f:
	{
					int64_t l = popOperandLong(operandStack);
					float f = (float)l;
					pushOperandFloat(operandStack, f);
	}
		break;
	case opc_l2d:
	{
					int64_t l = popOperandLong(operandStack);
					double d = (double)l;
					pushOperandDouble(operandStack, d);
	}
		break;
	case opc_f2i:
	{
					float f = popOperandFloat(operandStack);
					int32_t i = (int32_t)f;
					pushOperandInt(operandStack, i);
	}
		break;
	case opc_f2l:
	{
					float f = popOperandFloat(operandStack);
					int64_t l = (int64_t)f;
					pushOperandLong(operandStack, l);
	}
		break;
	case opc_f2d:
	{
					float f = popOperandFloat(operandStack);
					double d = (double)f;
					pushOperandDouble(operandStack, d);
	}
		break;
	case opc_d2i:
	{
					double d = popOperandDouble(operandStack);
					int32_t i = (int32_t)d;
					pushOperandInt(operandStack, i);
	}
		break;
	case opc_d2l:
	{
					double d = popOperandDouble(operandStack);
					int64_t l = (int64_t)d;
					pushOperandLong(operandStack, l);
	}
		break;
	case opc_d2f:
	{
					double d = popOperandDouble(operandStack);
					float f = (float)d;
					pushOperandFloat(operandStack, f);
	}
		break;
	case opc_i2b:
	{
					int32_t i = popOperandInt(operandStack);
					int8_t b = (int8_t)i;
					pushOperandInt(operandStack, b);
	}
		break;
	case opc_i2c:
	{
					int32_t i = popOperandInt(operandStack);
					int16_t c = (int16_t)i;
					pushOperandInt(operandStack, c);
	}
		break;
	case opc_i2s:
	{
					int32_t i = popOperandInt(operandStack);
					int16_t s = (int16_t)i;
					pushOperandInt(operandStack, s);
	}
		break;
	case opc_lcmp:
	{
					 int64_t v2 = popOperandLong(operandStack);
					 int64_t v1 = popOperandLong(operandStack);
					 if (v1 > v2)
						 pushOperandInt(operandStack, 1);
					 else if (v1 == v2)
						 pushOperandInt(operandStack, 0);
					 else
						 pushOperandInt(operandStack, -1);

	}
		break;
	case opc_fcmpl:
	{
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
		break;
	case opc_fcmpg:
	{
					  bool gFlag = true;
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
		break;
	case opc_dcmpl:
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
		break;
	case opc_dcmpg:
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
		break;
	case opc_ifeq:
	{
					 int32_t val = popOperandInt(operandStack);
					 if (val == 0)
						 frame->nextpc = frame->nextpc + (int16_t)operand.s;
					 return;
	}
		break;
	case opc_ifne:
	{
					 int32_t val = popOperandInt(operandStack);
					 if (val != 0)
						 frame->nextpc = frame->nextpc + (int16_t)operand.s;
	}
		break;
	case opc_iflt:
	{
					 int32_t val = popOperandInt(operandStack);
					 if (val < 0)
						 frame->nextpc = frame->nextpc + (int16_t)operand.s;
					 return;

	}
		break;
	case opc_ifge:
	{
					 int32_t val = popOperandInt(operandStack);
					 if (val >= 0)
						 frame->nextpc = frame->nextpc + (int16_t)operand.s;
					 return;

	}
		break;
	case opc_ifgt:
	{

					 int32_t val = popOperandInt(operandStack);
					 if (val > 0)
						 frame->nextpc = frame->nextpc + (int16_t)operand.s;
					 return;

	}
		break;
	case opc_ifle:
	{
					 int32_t val = popOperandInt(operandStack);
					 if (val <= 0)
						 frame->nextpc = frame->nextpc + (int16_t)operand.s;
					 return;

	}
		break;
	case opc_if_icmpeq:
	{
		{
			int32_t v2 = popOperandInt(operandStack);
			int32_t v1 = popOperandInt(operandStack);

			if (v1 == v2)
			{
				frame->nextpc = frame->nextpc + (int16_t)operand.s;
			}
			return;

		}
	}
		break;
	case opc_if_icmpne:
	{
						  int32_t v2 = popOperandInt(operandStack);
						  int32_t v1 = popOperandInt(operandStack);

						  if (v1 != v2)
						  {
							  frame->nextpc = frame->nextpc + (int16_t)operand.s;
						  }
						  return;

	}
		break;
	case opc_if_icmplt:
	{
						  int32_t v2 = popOperandInt(operandStack);
						  int32_t v1 = popOperandInt(operandStack);

						  if (v1 < v2)
						  {
							  frame->nextpc = frame->nextpc + (int16_t)operand.s;
						  }
						  return;

	}
		break;
	case opc_if_icmpge:
	{
						  int32_t v2 = popOperandInt(operandStack);
						  int32_t v1 = popOperandInt(operandStack);

						  if (v1 >= v2)
						  {
							  frame->nextpc = frame->nextpc + (int16_t)operand.s;
						  }
						  return;

	}
		break;
	case opc_if_icmpgt:
	{
						  int32_t v2 = popOperandInt(operandStack);
						  int32_t v1 = popOperandInt(operandStack);

						  if (v1 > v2)
						  {
							  frame->nextpc = frame->nextpc + (int16_t)operand.s;
						  }
	}
		break;
	case opc_if_icmple:
	{
						  int32_t v2 = popOperandInt(operandStack);
						  int32_t v1 = popOperandInt(operandStack);

						  if (v1 <= v2)
						  {
							  frame->nextpc = frame->nextpc + (int16_t)operand.s;
						  }
						  return;
	}
		break;
	case opc_if_acmpeq:
		do
		{
			Object * ref2 = popOperandRef(operandStack);
			Object * ref1 = popOperandRef(operandStack);
			if (ref2 == ref1)
				frame->nextpc = frame->nextpc + (int16_t)operand.s;
			return;
		} while (0);
		break;
	case opc_if_acmpne:
		do
		{
			Object * ref2 = popOperandRef(operandStack);
			Object * ref1 = popOperandRef(operandStack);
			if (ref2 != ref1)
				frame->nextpc = frame->nextpc + (int16_t)operand.s;
			return;
		} while (0);
		break;
	case opc_goto:
		frame->nextpc = frame->nextpc + (int16_t)operand.s;
		return;
		break;
	case opc_jsr:
		break;
	case opc_ret:
		break;
	case opc_tableswitch:
		break;
	case opc_lookupswitch:
		break;
	case opc_ireturn:
		execute_IRETURN(frame);
		break;
	case opc_lreturn:
		execute_LRETURN(frame);
		break;
	case opc_freturn:
		execute_FRETURN(frame);
		break;
	case opc_dreturn:
		execute_DRETURN(frame);
		break;
	case opc_areturn:
		break;
	case opc_return:
		execute_RETURN(frame);
		break;
	case opc_getstatic:
		break;
	case opc_putstatic:
		break;
	case opc_getfield:
		break;
	case opc_putfield:
		break;
	case opc_invokevirtual:
		execute_INVOKE_VIRTUAL(frame, operand.s);
		break;
	case opc_invokespecial:
		break;
	case opc_invokestatic:
		break;
	case opc_invokeinterface:
		break;
	case opc_xxxunusedxxx:
		break;
	case opc_new:
		break;
	case opc_newarray:
		break;
	case opc_anewarray:
		break;
	case opc_arraylength:
		break;
	case opc_athrow:
		break;
	case opc_checkcast:
		break;
	case opc_instanceof:
		break;
	case opc_monitorenter:
		break;
	case opc_monitorexit:
		break;
	case opc_wide:
		break;
	case opc_multianewarray:
		break;
	case opc_ifnull:
		break;
	case opc_ifnonnull:
		break;
	case opc_goto_w:
		break;
	case opc_jsr_w:
		break;
	case opc_breakpoint:
		break;
	case opc_aldc_ind_quick:
		break;
	case opc_aldc_ind_w_quick:
		break;
	case opc_invokestatic_quick:
		break;
	case opc_invokestatic_checkinit_quick:
		break;
	case opc_invokevirtual_quick:
		break;
	case opc_getfield_quick:
		break;
	case opc_agetfield_quick:
		break;
	case opc_vinvokevirtual_quick:
		break;
	case opc_invokevirtual_quick_w:
		break;
	case opc_putfield_quick:
		break;
	case opc_invokenonvirtual_quick:
		break;
	case opc_invokesuper_quick:
		break;
	case opc_invokeignored_quick:
		break;
	case opc_getfield2_quick:
		break;
	case opc_checkcast_quick:
		break;
	case opc_instanceof_quick:
		break;
	case opc_nonnull_quick:
		break;
	case opc_putfield2_quick:
		break;
	case opc_ainvokevirtual_quick:
		break;
	case opc_invokevirtualobject_quick:
		break;
	case opc_invokeinterface_quick:
		break;
	case opc_aldc_quick:
		break;
	case opc_ldc_quick:
		break;
	case opc_exittransition:
		break;
	case opc_dinvokevirtual_quick:
		break;
	case opc_aldc_w_quick:
		break;
	case opc_ldc_w_quick:
		break;
	case opc_aputfield_quick:
		break;
	case opc_getfield_quick_w:
		break;
	case opc_ldc2_w_quick:
		break;
	case opc_agetstatic_quick:
		break;
	case opc_getstatic_quick:
		break;
	case opc_getstatic2_quick:
		break;
	case opc_aputstatic_quick:
		break;
	case opc_putstatic_quick:
		break;
	case opc_putstatic2_quick:
		break;
	case opc_agetstatic_checkinit_quick:
		break;
	case opc_getstatic_checkinit_quick:
		break;
	case opc_getstatic2_checkinit_quick:
		break;
	case opc_aputstatic_checkinit_quick:
		break;
	case opc_putstatic_checkinit_quick:
		break;
	case opc_putstatic2_checkinit_quick:
		break;
	case opc_putfield_quick_w:
		break;
	case opc_new_checkinit_quick:
		break;
	case opc_new_quick:
		break;
	case opc_anewarray_quick:
		break;
	case opc_multianewarray_quick:
		break;
	case opc_prefix:
		break;
	case opc_invokeinit:
		break;

	}

	frame->lastpc = frame->nextpc;
	frame->nextpc += getOpcodeLen(opcode);
}

void dumpLocalVars(Frame * frame)
{
	LocalVars * localVars = frame->localVars;
	if (frame->localVars == NULL)
	{
		printf("  frame->localVars is NULL,Not log\n");
		return;
	}
	printf("  Local Vars:");
	for (uint32_t i = 0; i < localVars->size; i++)
	{
		printf("%d,", getLocalVarsInt(localVars, i));
	}
	printf("\n");
}
void interpret(Class * c, MethodBlock * method, Thread * thread, int argc, char ** argv)
{
	Frame * frame = NULL;

	int32_t pc = 0;
	uint8_t opcode = 0;

	for (;;)
	{
		frame = getCurrentFrame(thread);
		pc = frame->nextpc;
		opcode = *(method->code + pc);
		printf("pc:%d opcode: %d, name:%s\n", pc, opcode, getOpcodeName(opcode));
		processOpcode(frame, opcode, method->code + pc + 1, getOpcodeLen(opcode));
		dumpLocalVars(frame);
		if (opcode == opc_return)
		{
			break;
		}
	}
}