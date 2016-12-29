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
	popOperandSlot(frame->operandStack);
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

int32_t execute_INVOKE_VIRTUAL(Frame * frame, uint16_t cpIndex)
{
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
