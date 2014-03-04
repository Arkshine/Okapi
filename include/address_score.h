
#ifndef __ADDRESS_SCORE_H__
#define __ADDRESS_SCORE_H__

struct AddressScore
{
	int address;
	float score;

	AddressScore(int address=0,float score=0.0f);
};

bool operator <(AddressScore a,AddressScore b);
bool operator >(AddressScore a,AddressScore b);

#endif
