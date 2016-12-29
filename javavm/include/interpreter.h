#include "vm.h"
#include "frame.h"
#include "thread.h"

typedef union{
	uint8_t b;
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
	struct {
		uint8_t Index;
		int8_t Const;
	}iinc;
}Operand;

void InvokeMethod(Frame * invokerFrame, MethodBlock * method);
/* instructions */
void execute_INVOKE_STATIC(struct Frame * frame, uint16_t index);
int32_t execute_INVOKE_VIRTUAL(Frame * frame, uint16_t cpIndex);

int32_t execute_RETURN(Frame * frame);
int32_t execute_IRETURN(Frame * frame);
int32_t execute_LRETURN(Frame * frame);
int32_t execute_FRETURN(Frame * frame);
int32_t execute_DRETURN(Frame * frame);