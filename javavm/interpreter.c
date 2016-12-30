#include "class.h"
#include "thread.h"
#include "include\opcodes.h"
#include "frame.h"
#include "include\interpreter.h"


void InvokeMethod(Frame * invokerFrame, MethodBlock * method) {
	Thread * thread = invokerFrame->thread;
	Frame * frame = newFrame(thread, method, method->maxLocals, method->maxStack);
	pushThreadFrame(thread, frame);

	int32_t argSlotSlot = method->argSlotCount;
	if (argSlotSlot > 0)
	{
		for (int32_t i = argSlotSlot - 1; i >= 0; i--)
		{
			Slot * slot = popOperandSlotRef(invokerFrame->operandStack);
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

#define CASE_OPCODE_RETURN(label,frame,operand)\
	void execute_##label(Frame *, void *); \
	case label:\
	execute_##label(frame, operand); \
	return;

#define CASE_SIMPLE_OPCODE(label,function)\
	case label:\
	{function; }break;

	frame->lastpc = frame->nextpc;
	frame->nextpc += getOpcodeLen(opcode);

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
		CASE_SIMPLE_OPCODE(opc_iload_0, pushOperandInt(operandStack, getLocalVarsInt(localVars, 0)));
		CASE_SIMPLE_OPCODE(opc_iload_1, pushOperandInt(operandStack, getLocalVarsInt(localVars, 1)));
		CASE_SIMPLE_OPCODE(opc_iload_2, pushOperandInt(operandStack, getLocalVarsInt(localVars, 2)));
		CASE_SIMPLE_OPCODE(opc_iload_3, pushOperandInt(operandStack, getLocalVarsInt(localVars, 3)));
		CASE_SIMPLE_OPCODE(opc_lload_0, pushOperandLong(operandStack, getLocalVarsLong(localVars, 0)));
		CASE_SIMPLE_OPCODE(opc_lload_1, pushOperandLong(operandStack, getLocalVarsLong(localVars, 1)));
		CASE_SIMPLE_OPCODE(opc_lload_2, pushOperandLong(operandStack, getLocalVarsLong(localVars, 2)));
		CASE_SIMPLE_OPCODE(opc_lload_3, pushOperandLong(operandStack, getLocalVarsLong(localVars, 3)));
		CASE_SIMPLE_OPCODE(opc_fload_0, pushOperandFloat(operandStack, getLocalVarsFloat(localVars, 0)));
		CASE_SIMPLE_OPCODE(opc_fload_1, pushOperandFloat(operandStack, getLocalVarsFloat(localVars, 1)));
		CASE_SIMPLE_OPCODE(opc_fload_2, pushOperandFloat(operandStack, getLocalVarsFloat(localVars, 2)));
		CASE_SIMPLE_OPCODE(opc_fload_3, pushOperandFloat(operandStack, getLocalVarsFloat(localVars, 3)));
		CASE_SIMPLE_OPCODE(opc_dload_0, pushOperandDouble(operandStack, getLocalVarsDouble(localVars, 0)));
		CASE_SIMPLE_OPCODE(opc_dload_1, pushOperandDouble(operandStack, getLocalVarsDouble(localVars, 1)));
		CASE_SIMPLE_OPCODE(opc_dload_2, pushOperandDouble(operandStack, getLocalVarsDouble(localVars, 2)));
		CASE_SIMPLE_OPCODE(opc_dload_3, pushOperandDouble(operandStack, getLocalVarsDouble(localVars, 3)));
		CASE_SIMPLE_OPCODE(opc_aload_0, pushOperandRef(operandStack, getLocalVarsRef(localVars, 0)));
		CASE_SIMPLE_OPCODE(opc_aload_1, pushOperandRef(operandStack, getLocalVarsRef(localVars, 1)));
		CASE_SIMPLE_OPCODE(opc_aload_2, pushOperandRef(operandStack, getLocalVarsRef(localVars, 2)));
		CASE_SIMPLE_OPCODE(opc_aload_3, pushOperandRef(operandStack, getLocalVarsRef(localVars, 3)));
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
			CASE_OPCODE(opc_dup, frame, NULL);
			CASE_OPCODE(opc_dup_x1, frame, NULL);
			CASE_OPCODE(opc_dup_x2, frame, NULL);
			CASE_OPCODE(opc_dup2, frame, NULL);
			CASE_OPCODE(opc_dup2_x1, frame, NULL);
			CASE_OPCODE(opc_dup2_x2, frame, NULL);
			CASE_OPCODE(opc_swap, frame, NULL);
			CASE_OPCODE(opc_iadd, frame, NULL);
			CASE_OPCODE(opc_ladd, frame, NULL);
			CASE_OPCODE(opc_fadd, frame, NULL);
			CASE_OPCODE(opc_dadd, frame, NULL);
			CASE_OPCODE(opc_isub, frame, NULL);		
			CASE_OPCODE(opc_lsub, frame, NULL);			
			CASE_OPCODE(opc_fsub, frame, NULL);
			CASE_OPCODE(opc_dsub, frame, NULL);
			CASE_OPCODE(opc_imul, frame, NULL);
			CASE_OPCODE(opc_lmul, frame, NULL);	
			CASE_OPCODE(opc_fmul, frame, NULL);
			CASE_OPCODE(opc_dmul, frame, NULL);
			CASE_OPCODE(opc_idiv, frame, NULL);
			CASE_OPCODE(opc_ldiv, frame, NULL);
			CASE_OPCODE(opc_fdiv, frame, NULL);
			CASE_OPCODE(opc_ddiv, frame, NULL);
			CASE_OPCODE(opc_irem, frame, NULL);
			CASE_OPCODE(opc_lrem, frame, NULL);
			CASE_OPCODE(opc_frem, frame, NULL);
			CASE_OPCODE(opc_drem, frame, NULL);
			CASE_SIMPLE_OPCODE(opc_ineg, pushOperandInt(operandStack, -popOperandInt(operandStack)));			
			CASE_SIMPLE_OPCODE(opc_lneg, pushOperandLong(operandStack, -popOperandLong(operandStack)));
			CASE_SIMPLE_OPCODE(opc_fneg, pushOperandFloat(operandStack, -popOperandFloat(operandStack)));					
			CASE_SIMPLE_OPCODE(opc_dneg, pushOperandDouble(operandStack, -popOperandDouble(operandStack)));
			CASE_OPCODE(opc_ishl, frame, NULL);
			CASE_OPCODE(opc_lshl, frame, NULL);
			CASE_OPCODE(opc_ishr, frame, NULL);
			CASE_OPCODE(opc_lshr, frame, NULL);
			CASE_OPCODE(opc_iushr, frame, NULL);
			CASE_OPCODE(opc_lushr, frame, NULL);
			CASE_OPCODE(opc_land, frame, NULL);
			CASE_OPCODE(opc_ior, frame, NULL);
			CASE_OPCODE(opc_lor, frame, NULL);
			CASE_OPCODE(opc_ixor, frame, NULL);
			CASE_OPCODE(opc_lxor, frame, NULL);
			CASE_OPCODE(opc_iinc, frame, &operand);
			CASE_OPCODE(opc_i2l, frame, NULL);		
			CASE_OPCODE(opc_i2f, frame, NULL);
			CASE_OPCODE(opc_i2d, frame, NULL);
			CASE_OPCODE(opc_l2i, frame, NULL);
			CASE_OPCODE(opc_l2f, frame, NULL);
			CASE_OPCODE(opc_l2d, frame, NULL);
			CASE_OPCODE(opc_f2i, frame, NULL);
			CASE_OPCODE(opc_f2l, frame, NULL);
			CASE_OPCODE(opc_f2d, frame, NULL);
			CASE_OPCODE(opc_d2i, frame, NULL);
			CASE_OPCODE(opc_d2l, frame, NULL);			
			CASE_OPCODE(opc_d2f, frame, NULL);
			CASE_OPCODE(opc_i2b, frame, NULL);
			CASE_OPCODE(opc_i2c, frame, NULL);
			CASE_OPCODE(opc_i2s, frame, NULL);
			CASE_OPCODE(opc_lcmp, frame, NULL);
			CASE_OPCODE(opc_fcmpl, frame, NULL);
			CASE_OPCODE(opc_fcmpg, frame, NULL);
			CASE_OPCODE(opc_dcmpl, frame, NULL);
			CASE_OPCODE(opc_dcmpg, frame, NULL);
			CASE_OPCODE_RETURN(opc_ifeq, frame, NULL);
			CASE_OPCODE_RETURN(opc_ifne, frame, NULL);
			CASE_OPCODE_RETURN(opc_iflt, frame, NULL);
			CASE_OPCODE_RETURN(opc_ifge, frame, NULL);
			CASE_OPCODE_RETURN(opc_ifgt, frame, NULL);
			CASE_OPCODE_RETURN(opc_ifle, frame, NULL);
			CASE_OPCODE_RETURN(opc_if_icmpeq, frame, &operand);
			CASE_OPCODE_RETURN(opc_if_icmpne, frame, &operand);
			CASE_OPCODE_RETURN(opc_if_icmplt, frame, &operand);
			CASE_OPCODE_RETURN(opc_if_icmpge, frame, &operand);
			CASE_OPCODE_RETURN(opc_if_icmpgt, frame, &operand);	
			CASE_OPCODE_RETURN(opc_if_icmple, frame, &operand);
			CASE_OPCODE_RETURN(opc_if_acmpeq, frame, &operand);
			CASE_OPCODE_RETURN(opc_if_acmpne, frame, &operand);
			CASE_SIMPLE_OPCODE(opc_goto, frame->nextpc = frame->thread->pc + (int16_t)operand.s);
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
			CASE_OPCODE(opc_invokevirtual,frame, &operand);			
		case opc_invokespecial:
			break;
			CASE_OPCODE(opc_invokestatic, frame, &operand);
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
		frame->thread->pc = pc;
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