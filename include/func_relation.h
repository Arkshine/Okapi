
#ifndef __FUNC_RELATION_H__
#define __FUNC_RELATION_H__

struct FuncRelation
{
	int address;
	int relation;

	FuncRelation(int address=0,int relation=0) : address(address), relation(relation)
	{

	}
};

#endif