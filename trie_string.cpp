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
#include <string_.h>

template <typename T>
void TrieString<T>::init(char c)
{
	this->val = val;
	memset(childs, 0, sizeof(TrieString*) * Base);
	n = create_empty<T>();
}

template <typename T>
TrieString<T>::TrieString(char val)
{
	init(val);
}

template <typename T>
T& TrieString<T>::create(char* s)
{
	if (!*s)
	{
		return n;
	}
	else
	{
		char c = *s;

		TrieString* curr = this;
		TrieString** child;

		for (int i=1; i <= Mult; i++)
		{
			int d = c % Base;
			c /= Base;

			child = &curr->childs[d];

			if (!*child)
				*child = new TrieString(d);

			curr = *child;
		}

		return (*child)->create(s + 1);
	}
}

template <typename T>
void help(CVector<String>& list, TrieString<T>* t, String& s, int value, int inc, int count)
{
	if (!count)
	{
		String x = s;

		char c = value;

		x += c;

		CVector<String> list_new = t->get_keys(x);

		list.merge(list_new);

		return;
	}

	for (int i=0; i < Base; i++)
	{
		TrieString<T>*& child = t->childs[i];

		if (child)
		{
			String a = s;

			help(list, child, a, value + inc * i, inc * Base, count - 1);
		}
	}
}

template <typename T>
CVector<String> TrieString<T>::get_keys(String s)
{
	CVector<String> list;

	if (!check_empty(n))
	{
		String x = s;
		list.push_back(x);
	}

	help<T>(list, this, s, 0, 1, Mult);

	return list;
}

template <typename T>
TrieString<T>& TrieString<T>::move(TrieString& other)
{
	this->val = other.val;
	memcpy(this->childs, other.childs, sizeof(TrieString*) * Base);
	this->n = other.n;

	other.val = 1;
	memset(other.childs, 0, sizeof(TrieString*) * Base);
	other.n = create_empty<T>();

	return *this;
}

template <typename T>
TrieString<T>::~TrieString()
{
	for (int i=0; i < Base; i++)
		delete childs[i];
}

template <typename T>
TrieString<T>::TrieString()
{
	init(' ');
}

template <typename T>
TrieString<T>::TrieString(const TrieString& other)
{
	move(const_cast<TrieString&>(other));
}

template <typename T>
TrieString<T>& TrieString<T>::operator= (TrieString other)
{
	return move(other);
}

template <typename T>
T& TrieString<T>::operator[](char* s)
{
	return this->create(s);
}

template <typename T>
bool TrieString<T>::is_empty()
{
	if (!check_empty<T>(n))
		return false;

	for (int i=0; i < Base; i++)
	{
		TrieString*& child = childs[i];

		if (child)
		{
			if (!check_empty< TrieString<T> >(*child))
				return false;
		}
	}

	return true;
}

template <typename T>
CVector<String> TrieString<T>::get_keys()
{
	String s;
	return this->get_keys(s);
}
