// Works almost the same as CppAmpVectorAddition with
// the difference in indexing -- index is 2-dimentional.

#include <amp.h>
#include <iostream>

using namespace concurrency;

int main()
{
	const int size = 6;

	// Two vectors to add up.
	int a[] = { 1,2,3,4,5,6 }; 
	int b[] = { 1,2,3,4,5,6 };

	// Vector to hold the sum
	int c[size];

	// Array views for gpu to read and write data from and to vectors
	array_view<const int, 2> aView(2, 3, a);
	array_view<const int, 2> bView(2, 3, b);
	array_view<      int, 2> cView(2, 3, c);

	// Optimisation, informs amp not to initialize cView - it will be afterall overwritten.
	cView.discard_data();

	// Kernel adding up vectors. 
	// Nice thing to remember is that 'extent' mens simply 'range'.
	parallel_for_each(cView.extent, [=](index<2> idx) restrict(amp)
	{
		cView(idx) = aView(idx) + bView(idx);
	}
	);

	// Print the sum
	std::cout << "<";
	
	for (int i = 0; i < 2; i++)
	for (int j = 0; j < 3; j++)
	{
		std::cout 
			<< cView[i][j] 
			<< ((3*i + j  != size-1) ? ", " : "");
	}
	
	std::cout << ">" << std::endl;
	std::getchar();
}