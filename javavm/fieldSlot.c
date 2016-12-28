#include "vm.h"
#include "class.h"

void setFieldSlotInt(FieldSlot * slot, uint16_t index, int32_t val)
{
	slot[index].num = val;
}

int32_t getFieldSlotInt(FieldSlot * slot, uint16_t index)
{
	return slot[index].num;
}

void setFieldSlotFloat(FieldSlot * slot, uint16_t index, float val)
{
	FloatInt transData;
	transData.floatData = val;
	slot[index].num = transData.bitsData;
}

float getFieldSlotFloat(FieldSlot * slot, uint16_t index)
{
	FloatInt transData;
	transData.bitsData = getFieldSlotInt(slot, index);
	return transData.floatData;
}

void setFieldSlotLong(FieldSlot * slot, uint16_t index, int64_t val)
{
	LongInt transData;
	transData.longData = val;
	slot[index].num = transData.low;
	slot[index + 1].num = transData.high;
}

int64_t getFieldSlotLong(FieldSlot * slot, uint16_t index)
{
	LongInt transData;
	transData.low = getFieldSlotInt(slot, index);
	transData.high = getFieldSlotInt(slot, index + 1);
	return transData.longData;
}

void setFieldSlotDouble(FieldSlot * slot, uint16_t index, double val)
{
	DoubleLong transData;
	transData.doubleData = val;
	setFieldSlotLong(slot, index, transData.bitsData);
}

double getFieldSlotDouble(FieldSlot * slot, uint16_t index)
{
	DoubleLong transData;
	transData.bitsData = getFieldSlotLong(slot, index);
	return transData.doubleData;
}


void setFieldSlotRef(FieldSlot * slot, uint16_t index, Object * ref)
{
	slot[index].ref = ref;
	if (ref->dataCount == 10)
		return;
}

Object * getFieldSlotRef(FieldSlot * slot, uint16_t index)
{
	return slot[index].ref;
}