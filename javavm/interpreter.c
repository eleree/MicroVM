#include "class.h"
#include "thread.h"
#include "include\opcodes.h"

void interpret(Class * c, MethodBlock * method, Thread * thread, int argc, char ** argv)
{
	Frame * frame = NULL;
		
	int32_t pc = 0;
	uint8_t opcode = 0;

	for (;;)
	{
		frame = getCurrentFrame(thread);
		opcode = *(method->code + pc);
		printf("pc:%d opcode: %d, name:%s\n",pc, opcode, opcodeName(opcode));
		pc += opcodeLen(opcode);

		if (opcode == opc_return)
		{
			break;
		}
	}
}