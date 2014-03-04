
#include <address_score.h>
#include <s_address_line.h>
#include <s_steps_address.h>
#include <function.h>

template <typename T>
T max_val();

template <>
AddressScore max_val<AddressScore>()
{
	return AddressScore(0,0);
}

template <>
int max_val<int>()
{
	return 0;
}

template <>
s_address_line max_val<s_address_line>()
{
	return s_address_line(0,0);
}

template <>
s_steps_address max_val<s_steps_address>()
{
	return s_steps_address(0,INT_MAX);
}

template <typename T>
T create_empty();

template <>
bool create_empty<bool>();

template <>
int create_empty<int>();

template <>
Function* create_empty<Function*>()
{
	return NULL;
}


template <typename T>
bool check_empty(T& a);

template <>
bool check_empty(CVector<int>& a);

template <>
bool check_empty(Command*& a);

template <>
bool check_empty<bool>(bool& a);

template <>
bool check_empty<float>(float& a);

template <>
bool check_empty<int>(int& a);

template <typename T>
T create_empty()
{
	return T();
}

template <>
bool create_empty<bool>()
{
	return false;
}

template <>
int create_empty<int>()
{
	return 0;
}

template <>
Command* create_empty()
{
	return NULL;
}

template <typename T>
bool check_empty(T& a)
{
	return a.is_empty();
}

template <>
bool check_empty(CVector<int>& a)
{
	return a.size() == 0;
}

template <>
bool check_empty(Command*& a)
{
	return a == NULL;
}

template <>
bool check_empty(Function*& a)
{
	return a == NULL;
}

template <>
bool check_empty<bool>(bool& a)
{
	return a == false;
}

template <>
bool check_empty<float>(float& a)
{
	return a == 0.0f;
}

template <>
bool check_empty<int>(int& a)
{
	return a == 0;
}