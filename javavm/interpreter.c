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
	case opc_wide:
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
		printf("pc:%d opcode: %d, name:%s\n",pc, opcode, getOpcodeName(opcode));
		processOpcode(frame, &pc, opcode, method->code + pc + 1, getOpcodeLen(opcode));

		if (opcode == opc_return)
		{
			break;
		}
	}
}