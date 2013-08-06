#include <amp.h>
#include <iostream>

using namespace concurrency;

int main()
{
	const int size = 5;

	// two vectors to add up.
	int a[] = {1,2,3,4,5}; 
	int b[] = {1,2,3,4,5};

	// vector to hold the sum
	int c[size];

	// array views for gpu to read and write data from and to vectors
	array_view<const int, 1> aView(size, a);
	array_view<const int, 1> bView(size, b);
	array_view<      int, 1> cView(size, c);

	// optimisation, informs amp not to initialize cView - it will be afterall overwritten.
	cView.discard_data();

	// kernel adding up vectors.
	parallel_for_each(cView.extent, [=](index<1> idx) restrict(amp)
	{
		cView(idx) = aView(idx) + bView(idx);
	}
	);

	// print the sum
	std::cout << "<";
	for (int i = 0; i < size; i++)
	{
		std::cout << cView[i] <<  ((i != size-1) ? ", " : "");
	}
	std::cout << ">" << std::endl;
	std::getchar();
}