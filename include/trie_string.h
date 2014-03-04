
#ifndef __TRIE_STRING_H__
#define __TRIE_STRING_H__

#include <CVector.h>
#include <string_.h>

const int Base = 4;
const int Mult = 4;

template <typename T>
struct TrieString
{
	TrieString* childs[Base];
	T n;

	char val;

	void init(char val);
	
	TrieString(char val);

	T& create(char* s);

	CVector<String> get_keys(String s);

	TrieString& move(TrieString& other);

	public :

		~TrieString();

		TrieString();

		TrieString (const TrieString& other);

		TrieString& operator= (TrieString other);
		
		T& operator[](char* s);

		bool is_empty();
	
		CVector<String> get_keys();
};

#include <trie_string.cpp>

#endif