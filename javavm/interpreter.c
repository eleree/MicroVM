#include "class.h"
#include "thread.h"
#include "include\opcodes.h"
#include "frame.h"

typedef union{
	uint8_t c;
	uint16_t s;
	union{
		uint16_t index;
		uint8_t count;
		uint8_t zero;
	}invokeInterface;
	union{
		uint16_t index;
		uint8_t dimensions;
	}multiAnewArray;
}Operand;

void processOpcode(Frame * frame, int32_t * pc, uint8_t opcode, uint8_t * operandBytes, uint8_t opcodeLen)
{
	Operand operand = { 0 };

	switch (opcodeLen)
	{
	case 2:
		operand.c = *operandBytes;
		break;
	case 3:
		operand.s = (*operandBytes) << 8 | *(operandBytes + 1);
	default:
		break;
	}

	switch (opcode)
	{
	case opc_nop:
		break;
	case opc_aconst_null:
		pushOperandRef(frame->operandStack, NULL);
		break;
	case opc_iconst_m1:
		break;
	case opc_iconst_0:
		break;
	case opc_iconst_1:
		break;
	case opc_iconst_2:
		break;
	case opc_iconst_3:
		break;
	case opc_iconst_4:
		break;
	case opc_iconst_5:
		break;
	case opc_lconst_0:
		break;
	case opc_lconst_1:
		break;
	case opc_fconst_0:
		break;
	case opc_fconst_1:
		break;
	case opc_fconst_2:
		break;
	case opc_dconst_0:
		break;
	case opc_dconst_1:
		break;
	case opc_bipush:
		break;
	case opc_sipush:
		break;
	case opc_ldc:
		switch (frame->method->classMember.attachClass->constantPool[operand.c].cpType){
		case CONSTATNT_INTEGER:
			pushOperandInt(frame->operandStack, frame->method->classMember.attachClass->constantPool[operand.c].cpItem.u32);
			break;
		case CONSTATNT_FLOAT:
			break;
		case CONSTATNT_STRING:
			break;
		case CONSTATNT_CLASS:
			break;
		}
		break;
	case opc_ldc_w:
		break;
	case opc_ldc2_w:
		break;
	case opc_iload:
		break;
	case opc_lload:
		break;
	case opc_fload:
		break;
	case opc_dload:
		break;
	case opc_aload:
		break;
	case opc_iload_0:
		break;
	case opc_iload_1:
		break;
	case opc_iload_2:
		break;
	case opc_iload_3:
		break;
	case opc_lload_0:
		break;
	case opc_lload_1:
		break;
	case opc_lload_2:
		break;
	case opc_lload_3:
		break;
	case opc_fload_0:
		break;
	case opc_fload_1:
		break;
	case opc_fload_2:
		break;
	case opc_fload_3:
		break;
	case opc_dload_0:
		break;
	case opc_dload_1:
		break;
	case opc_dload_2:
		break;
	case opc_dload_3:
		break;
	case opc_aload_0:
		break;
	case opc_aload_1:
		break;
	case opc_aload_2:
		break;
	case opc_aload_3:
		break;
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
	case opc_istore:
		break;
	case opc_lstore:
		break;
	case opc_fstore:
		break;
	case opc_dstore:
		break;
	case opc_astore:
		break;
	case opc_istore_0:
		break;
	case opc_istore_1:
		break;
	case opc_istore_2:
		break;
	case opc_istore_3:
		break;
	case opc_lstore_0:
		break;
	case opc_lstore_1:
		break;
	case opc_lstore_2:
		break;
	case opc_lstore_3:
		break;
	case opc_fstore_0:
		break;
	case opc_fstore_1:
		break;
	case opc_fstore_2:
		break;
	case opc_fstore_3:
		break;
	case opc_dstore_0:
		break;
	case opc_dstore_1:
		break;
	case opc_dstore_2:
		break;
	case opc_dstore_3:
		break;
	case opc_astore_0:
		break;
	case opc_astore_1:
		break;
	case opc_astore_2:
		break;
	case opc_astore_3:
		break;
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
	case opc_pop:
		break;
	case opc_pop2:
		break;
	case opc_dup:
		break;
	case opc_dup_x1:
		break;
	case opc_dup_x2:
		break;
	case opc_dup2:
		break;
	case opc_dup2_x1:
		break;
	case opc_dup2_x2:
		break;
	case opc_swap:
		break;
	case opc_iadd:
		break;
	case opc_ladd:
		break;
	case opc_fadd:
		break;
	case opc_dadd:
		break;
	case opc_isub:
		break;
	case opc_lsub:
		break;
	case opc_fsub:
		break;
	case opc_dsub:
		break;
	case opc_imul:
		break;
	case opc_lmul:
		break;
	case opc_fmul:
		break;
	case opc_dmul:
		break;
	case opc_idiv:
		break;
	case opc_ldiv:
		break;
	case opc_fdiv:
		break;
	case opc_ddiv:
		break;
	case opc_irem:
		break;
	case opc_lrem:
		break;
	case opc_frem:
		break;
	case opc_drem:
		break;
	case opc_ineg:
		break;
	case opc_lneg:
		break;
	case opc_fneg:
		break;
	case opc_dneg:
		break;
	case opc_ishl:
		break;
	case opc_lshl:
		break;
	case opc_ishr:
		break;
	case opc_lshr:
		break;
	case opc_iushr:
		break;
	case opc_lushr:
		break;
	case opc_iand:
		break;
	case opc_land:
		break;
	case opc_ior:
		break;
	case opc_lor:
		break;
	case opc_ixor:
		break;
	case opc_lxor:
		break;
	case opc_iinc:
		break;
	case opc_i2l:
		break;
	case opc_i2f:
		break;
	case opc_i2d:
		break;
	case opc_l2i:
		break;
	case opc_l2f:
		break;
	case opc_l2d:
		break;
	case opc_f2i:
		break;
	case opc_f2l:
		break;
	case opc_f2d:
		break;
	case opc_d2i:
		break;
	case opc_d2l:
		break;
	case opc_d2f:
		break;
	case opc_i2b:
		break;
	case opc_i2c:
		break;
	case opc_i2s:
		break;
	case opc_lcmp:
		break;
	case opc_fcmpl:
		break;
	case opc_fcmpg:
		break;
	case opc_dcmpl:
		break;
	case opc_dcmpg:
		break;
	case opc_ifeq:
		break;
	case opc_ifne:
		break;
	case opc_iflt:
		break;
	case opc_ifge:
		break;
	case opc_ifgt:
		break;
	case opc_ifle:
		break;
	case opc_if_icmpeq:
		break;
	case opc_if_icmpne:
		break;
	case opc_if_icmplt:
		break;
	case opc_if_icmpge:
		break;
	case opc_if_icmpgt:
		break;
	case opc_if_icmple:
		break;
	case opc_if_acmpeq:
		break;
	case opc_if_acmpne:
		break;
	case opc_goto:
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
		break;
	case opc_lreturn:
		break;
	case opc_freturn:
		break;
	case opc_dreturn:
		break;
	case opc_areturn:
		break;
	case opc_return:
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

	*pc += getOpcodeLen(opcode);
}

void interpret(Class * c, MethodBlock * method, Thread * thread, int argc, char ** argv)
{
	Frame * frame = NULL;

	int32_t pc = 0;
	uint8_t opcode = 0;

	for (;;)
	{
		frame = getCurrentFrame(thread);
		opcode = *(method->code + pc);
		printf("pc:%d opcode: %d, name:%s\n", pc, opcode, getOpcodeName(opcode));
		processOpcode(frame, &pc, opcode, method->code + pc + 1, getOpcodeLen(opcode));

		if (opcode == opc_return)
		{
			break;
		}
	}
}