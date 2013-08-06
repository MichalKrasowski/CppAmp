#pragma once
#include <iostream>

class ArrayPrettyPrinter
{
public:
	ArrayPrettyPrinter(void);
	~ArrayPrettyPrinter(void);

	template<typename Iterable>
	static void print(Iterable collection, unsigned int cols)
	{
		auto begin = collection.begin();
		auto end = collection.end();
		for(int i = 0; begin != end; ++begin, ++i) 
		{
			std::cout << *begin << ((i % 4 == 3) ? "\n" : ",\t");
		}
	}
};

