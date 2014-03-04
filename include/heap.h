
#ifndef __HEAP_MIN_H__
#define __HEAP_MIN_H__

#include <CVector.h>

template <typename T>
T max_val();

template <typename T>
struct CompareMin
{
     static bool op(T a,T b)
	 {
		 return a < b;
	 }
};

template <typename T>
struct CompareMax
{
     static bool op(T a,T b)
	 {
		 return a > b;
	 }
};

template <typename T,class Compare>
struct Heap
{
	CVector<T> data;

		void fix(size_t id)
		{
			if(id >= data.size())
				return;

			T& cur = data[id];

			size_t left_id = id*2;
			
			if(left_id >= data.size())
				return;

			T left = data[left_id];
			
			int lower_id = left_id;
			T lower = left;

			size_t right_id = left_id+1;

			if(right_id < data.size())
			{
				T right = data[right_id];
				
				if(Compare::op(right,left))
				{
					lower = right;
					lower_id = right_id;
				}
			}

			if(Compare::op(lower,cur))
			{
				T aux = data[id];
				data[id] = lower;
				data[lower_id] = aux;
				
				fix(lower_id);
			}		
		}
		
	public:
	
		Heap() 
		{
			data.push_back(T());
		}

		void insert(T value)
		{
			data.push_back(value);

			int index = data.size() - 1;

			while(true)
			{
				if(index == 1)
					break;

				int parent_index = index/2;

				if( Compare::op(data[index],data[parent_index]) )
				{
					T aux = data[parent_index];
					data[parent_index] = data[index];
					data[index] = aux;

					index = parent_index;
					continue;
				}

				break;
			}
		}

		T get()
		{
			if(data.size() == 1)
				return max_val<T>();

			T min_val = data[1];
			data[1] = data[data.size()-1];
			data.pop_back();

			fix(1);

			return min_val;
		}

		bool empty()
		{
			return data.size() == 1;
		}
};

template <typename T>
struct HeapMin : public Heap< T , CompareMin<T> >
{

};

template <typename T>
struct HeapMax : public Heap< T , CompareMax<T> >
{

};

#endif