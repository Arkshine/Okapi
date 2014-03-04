
#ifndef __GAME_LIBRARY_H__
#define __GAME_LIBRARY_H__

#include <s_library.h>
#include <heap.h>
#include <s_address_line.h>
#include <CVector.h>
#include <trie.h>
#include <s_steps_address.h>
#include <address_score.h>
#include <string_.h>
#include <func_relation.h>
#include <trie_string.h>

typedef Trie10< Trie10<bool> > call_tree_t;

class GameLibrary : public s_library
{
	CVector<int> base_offsets;
	call_tree_t call_tree;

	Trie10< Trie2 < Trie10 <bool> > > neighs_at_depth_cache;

	Trie10< Trie10 < int > > relationship_cache;

	HeapMin<s_address_line> get_call_list()
	{
		HeapMin<s_address_line> call_list;

		for(int offset=0; (int) offset<this->length; offset++)
		{
			unsigned char *address = ((unsigned char*)((unsigned char*)this->address + offset));

			if(*address == 0xE8)
			{
				int call = *((int*)(address+1));

				int address_called = (int)address + call + 1 + sizeof(void*);
			
				int address_called_offset = address_called - (int) this->address;

				if( (address_called_offset>=0) && (address_called_offset < this->length) )
				{
					call_list.insert(s_address_line(offset,address_called_offset));
					call_list.insert(s_address_line(address_called_offset,0));
				}
			}
		}
		
		for(size_t i=0;i<this->base_offsets.size();i++)
		{
			int offset = base_offsets[i];

			call_list.insert(s_address_line(offset,0));
		}

		return call_list;
	}

	void fill_call_tree(HeapMin<s_address_line> call_list);

	protected:

		virtual CVector<int> get_base_offsets() = 0;
		
		void init()
		{
			CVector<int> offsets = this->get_base_offsets();
			this->base_offsets.merge(offsets);
			
			HeapMin<s_address_line> call_list = get_call_list();

			fill_call_tree(call_list);
		}

	public:

		virtual String get_name() = 0;

		GameLibrary(s_library s)
		{
			this->address = s.address;
			this->length = s.length;
			this->handle = s.handle;
		}

		char* get_address_sym(void* address)
		{
			return get_address_symbol((void*)((int)this->address + (int)address));
		}

		int find_func(char* function_name)
		{
			int address = (int)find_function(this,function_name);

			if(!address)
				return 0;

			return address - (int)this->address;
		}

		CVector<int> get_neighs_at_depth(int address,int depth)
		{
			Trie10<bool>& cached = this->neighs_at_depth_cache[address][depth];
			CVector<int> keys = cached.get_keys();

			if(keys.size())
			{
				return keys;
			}

			Trie10<bool> visited_steps;

			CVector<int> neighs_at_level;

			HeapMin<s_steps_address> heap;

			heap.insert(s_steps_address(address,1));

			while(!heap.empty())
			{
				s_steps_address s = heap.get();

				if(s.steps == depth)
				{
					cached[s.address] = true;

					neighs_at_level.push_back(s.address);
					continue;
				}
		
				CVector<int> neighs = call_tree[s.address].get_keys();
	
				for(size_t i=0;i<neighs.size();i++)
				{	
					int neigh = neighs[i];

					if(visited_steps[neigh])
						continue;

					heap.insert(s_steps_address(neigh,s.steps+1));
					visited_steps[neigh] = true;
				}
			}
	
			return neighs_at_level;
		}

		int get_steps(int a,int b,int steps,int max_steps)
		{
			Trie10<bool> visited_steps;
			HeapMin<s_steps_address> heap;

			heap.insert(s_steps_address(a,1));

			while(!heap.empty())
			{
				s_steps_address s = heap.get();

				if(s.address == b)
					return s.steps;
		
				CVector<int> neighs = call_tree[s.address].get_keys();
	
				if((s.steps+1) <= max_steps)
				{
					for(size_t i=0;i<neighs.size();i++)
					{	
						int neigh = neighs[i];

						if(visited_steps[neigh])
							continue;

						heap.insert(s_steps_address(neigh,s.steps+1));
						visited_steps[neigh] = true;
					}
				}
			}
	
			return 0;
		}

		int relationship(int a,int b)
		{
			int& r_cached = relationship_cache[a][b];

			if(r_cached)
				return r_cached;

			int n1 = get_steps(a,b,0,0xF);
			int n2 = get_steps(b,a,0,n1);

			int r = min(n1,n2);

			r_cached = r;

			return r;
		}

		CVector<int> get_distances(int offset)
		{
			CVector<int> distances;
			
			for(size_t i=0;i<this->base_offsets.size();i++)
			{
				int offset2 = base_offsets[i];

				int r = this->relationship(offset,offset2);

				distances.push_back(r);
			}

			return distances;
		}

		Trie10< CVector<int> > calc_distances_score_map(CVector<int> distances)
		{
			Trie10< CVector<int> > map;

			for(size_t i=0;i<this->base_offsets.size();i++)
			{
				if(!distances[i])
					continue;

				int offset = this->base_offsets[i];
				
				for(int add =-1; add<=1; add++)
				{					
					CVector<int> neighs = this->get_neighs_at_depth(offset,distances[i] + add);
					
					for(size_t i=0;i<neighs.size();i++)
					{
						int neigh = neighs[i];

						CVector<int>& hits = map[neigh];
				
						hits.push_back(2 - abs(add));
					}
				}
			}

			return map;
		}

		HeapMax<AddressScore> calc_distances_score_heap(CVector<int> distances)
		{
			Trie10< CVector<int> > distances_score_map = this->calc_distances_score_map(distances);
			return this->calc_distances_score_heap(distances_score_map);
		}
		
		/*

		HeapMax<AddressScore> calc_distances_score_heap(String treemap)
		{
			CVector<int> distances = decode_distances(treemap);

			Trie10< CVector<int> > distances_score_map = this->calc_distances_score_map(distances);
			HeapMax<AddressScore> distances_score_heap =  this->calc_distances_score_heap(distances_score_map);

			return distances_score_heap;
		}

		int get_distances_best_match(String treemap,CVector<FuncRelation> relations)
		{
			CVector<int> distances = decode_distances(treemap);
			
			HeapMax<AddressScore> distances_score_heap = calc_distances_score_heap(treemap);

			return get_distances_best_match(distances_score_heap,distances,relations);
		}
		*/

		int get_distances_best_match(HeapMax<AddressScore> distances_score_heap,CVector<int> distances,CVector<FuncRelation> relations)
		{
			if(distances_score_heap.empty())
				return 0;

			if(!relations.size())
			{
				AddressScore address_score = distances_score_heap.get();
				return address_score.address;
			}

			Trie10< Trie10<bool> > restritions = this->parse_restritions(relations);

			while(!distances_score_heap.empty())
			{
				AddressScore address_score = distances_score_heap.get();

				int address = address_score.address;
				float score = address_score.score;

				CVector<int> addresses = restritions.get_keys();

				bool satisfied = true;

				for(size_t i=0;i<addresses.size();i++)
				{
					int relation_address = addresses[i];

					if(!restritions[relation_address][address])
					{
						satisfied = false;
						break;
					}
				}

				if(!satisfied)
					continue;

				return address;
			}
			
			return 0;
		}

		int get_distances_best_match(CVector<int> distances,CVector<FuncRelation> relations)
		{
			HeapMax<AddressScore> distances_score_heap = calc_distances_score_heap(distances);

			return get_distances_best_match(distances_score_heap,distances,relations);	
		}

		HeapMax<AddressScore> calc_distances_score_heap(Trie10< CVector<int> > distances_score_map)
		{
			CVector<int> keys = distances_score_map.get_keys();

			HeapMax<AddressScore> heap;

			for(size_t i=0;i<keys.size();i++)
			{
				int key = keys[i];

				CVector<int>& hits = distances_score_map[key];
			
				int my_hits = 0;

				for(size_t j=0;j<hits.size();j++)
				{
					int hit = hits[j]; 
				
					my_hits += (hit);
				}

				heap.insert(AddressScore(key,(float)my_hits / (float)(2*this->base_offsets.size()) ));
			}

			return heap;
		}

		String encode_distances(CVector<int> distances)
		{
			String s;

			size_t i = 0;

			while(i < distances.size())
			{
				unsigned int sum = 0;
				unsigned int mult = 1;

				size_t j=0;
				for(;j<=5 && i<distances.size();j++,i++)
				{
					sum += mult * distances[i];
					mult *= 0x10;
				}

				while(sum)
				{
					unsigned int x = sum % 91;
					sum /= 91;

					x += 32;

					if(x == '%')
					{
						s+='{';
					}
					else if(x == '^')
					{
						s+='|';
					}
					else if (x =='"')
					{
						s+='}';
					}
					else if (x == '.')
					{
						s+='~';
					}
					else
					{
						s.sprintf("%c",x);
					}
				}

				if(i == distances.size())
					s += ((char)(6 - j) + 49);
				else
					s += '.';
			}

			return s;
		}

		CVector<int> decode_distances(String compressed)
		{
			CVector<int> distances;
			
			char* data = compressed.get_ptr();

			char c;
			unsigned int sum = 0;
			int mult = 1;

			while(c=(*data++))
			{
				int n = 0;

				if(!data[0])
				{
					n = c - 49;

					c = '.';
				}

				if(c == '.')
				{
					for(int i=0;i<=5-n;i++)
					{
						int x = sum % 16;
						sum /= 16;

						distances.push_back(x);
					}

					sum = 0;
					mult = 1;

					continue;
				}

				if(c == '{')
					c = '%';
				else if(c == '|')
					c = '^';
				else if (c =='}')
					c = '"';
				else if (c == '~')
					c = '.';

				c -= 32;

				sum += mult * c;
				mult *= 91;
			}

			if(distances.size() != this->base_offsets.size())
				return CVector<int>();

			return distances;
		}

		bool has_address(int address)
		{
			return this->call_tree[address][address];
		}

		Trie10< Trie10<bool> > parse_restritions(CVector<FuncRelation> relations)
		{
			Trie10< Trie10<bool> > restritions;

			for(size_t i=0;i<relations.size();i++)
			{
				FuncRelation relation = relations[i];

				CVector<int> neighs = this->get_neighs_at_depth(relation.address,relation.relation);

				for(size_t i=0;i<neighs.size();i++)
				{
					int neigh = neighs[i];

					if(relation.relation == this->relationship(relation.address,neigh))
					{
						restritions[relation.address][neigh] = true;
					}
				}
			}

			return restritions;
		}

		bool contains_address(long address)
		{
			return (address >= (long)this->address) && (address < ((long)this->address + this->length));
		}
};

#endif