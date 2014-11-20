//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Okapi Module
//

#include <CVector.h>
#include <string.h>

template <typename T>
T create_empty();

template <typename T>
bool check_empty(T& a);

template <typename T, int Base>
void Trie<T, Base>::init(unsigned int val)
{
	this->val = val;
	memset(childs, 0, sizeof(Trie*) * Base);
	n = create_empty<T>();
}

template <typename T, int Base>
Trie<T, Base>::Trie(int mult)
{
	init(mult);
}

template <typename T, int Base>
T& Trie<T, Base>::create(unsigned int number, int mult)
{
	if (!number)
	{
		return n;
	}
	else
	{
		unsigned int digit = number % Base;
		number /= Base;

		Trie*& child = childs[digit];

		if (!child)
			child = new Trie(digit*mult);

		return child->create(number, mult*Base);
	}
}

template <typename T, int Base>
CVector<int> Trie<T, Base>::get_keys(int val)
{
	CVector<int> list;

	if (!check_empty(n))
	{
		list.push_back(val);
	}

	for (int i=0; i < Base; i++)
	{
		Trie<T, Base>*& child = childs[i];

		if (child)
		{
			CVector<int> list_new = child->get_keys(child->val + val);

			list.merge(list_new);
		}
	}

	return list;
}

template <typename T, int Base>
Trie<T, Base>& Trie<T, Base>::move(Trie& other)
{
	this->val = other.val;
	memcpy(this->childs, other.childs, sizeof(Trie*) * Base);
	this->n = other.n;

	other.val = 1;
	memset(other.childs, 0, sizeof(Trie*) * Base);
	other.n = create_empty<T>();

	return *this;
}

template <typename T, int Base>
Trie<T, Base>::~Trie()
{
	for (int i=0; i < Base; i++)
		delete childs[i];
}

template <typename T, int Base>
Trie<T, Base>::Trie()
{
	init(1);
}

template <typename T, int Base>
Trie<T, Base>::Trie(const Trie& other)
{
	move(const_cast<Trie&>(other));
}

template <typename T, int Base>
Trie<T, Base>& Trie<T, Base>::operator= (Trie other)
{
	return move(other);
}

template <typename T, int Base>
T& Trie<T, Base>::operator[](unsigned int number)
{
	return this->create(number, 1);
}

template <typename T, int Base>
bool Trie<T, Base>::is_empty()
{
	if (!check_empty<T>(n))
		return false;

	for (int i=0; i < Base; i++)
	{
		Trie*& child = childs[i];

		if (child)
		{
			if (!check_empty< Trie<T, Base> >(*child))
				return false;
		}
	}

	return true;
}

template <typename T, int Base>
CVector<int> Trie<T, Base>::get_keys()
{
	return this->get_keys(0);
}
