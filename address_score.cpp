
#include <address_score.h>

AddressScore::AddressScore(int address,float score) : address(address), score(score)
{

}

bool operator <(AddressScore a,AddressScore b)
{
	return a.score < b.score;
}

bool operator >(AddressScore a,AddressScore b)
{
	return a.score > b.score;
}
