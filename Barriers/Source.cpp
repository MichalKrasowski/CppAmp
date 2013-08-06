#include <amp.h>

#include <iostream>
#include <vector>

#include <ArrayPrettyPrinter.h>

using namespace concurrency;

const int ROWS = 4;
const int COLS = 4;

int main()
{

#pragma region vector and array_view initialisation

	// source vector initialisation
	std::vector<int> sourceVector;
	for (int i = 0; i < ROWS*COLS; i++)
	{
		sourceVector.push_back( i );
	}
	
	#pragma endregion

#pragma region C++AMP solution

	// copy vector for C++AMP
	std::vector<int> ampVector = sourceVector;

	// array view initialization
	extent<2> ext(ROWS, COLS);
	array_view<int, 2> arrayView(ext, ampVector);

	// add 1 and then square each value in the array
	parallel_for_each(
		arrayView.extent.tile<2,2>(),
		[=](tiled_index<2,2> idx) restrict(amp)
		{
			arrayView[idx] = arrayView[idx] + 1;
			
			idx.barrier.wait();
			
			arrayView[idx] = arrayView[idx] * arrayView[idx];

			// Can it be removed? Why?
			// idx.barrier.wait();
		}
	);

	// sychronize v with arrayView
	arrayView.synchronize();	// Can it be removed? Why?
								// No, it's required.
								// Without it arrayView won't copy the results
								// from arrayView back to ampVector
#pragma endregion

#pragma region C++ bruteforce for comparison

	std::vector<int> cppVector = sourceVector;
	for (unsigned int i = 0; i < cppVector.size(); i++)
	{
		cppVector[i]  = cppVector[i] + 1;
		cppVector[i] *= cppVector[i];
	}

#pragma endregion


#pragma region print the data
	
	ArrayPrettyPrinter::print(ampVector, COLS);

#pragma endregion

#pragma region test value correctness
	if (ampVector.size() != cppVector.size()) 
		throw "The size of cppVector mismatches the size of ampVector!";

	for (unsigned int i = 0; i < ampVector.size(); i++)
	{
		if (ampVector[i] != cppVector[i]) {
			std::cout 
				<< ampVector[i] 
				<< " = "
				<< "v["		    << i << "]" << " != "
				<< "cppVector[" << i << "]"
				<< " = "
				<< cppVector[i] << std::endl; 
		}
	}

#pragma endregion

	char wait;
	std::cin >> wait;

	// expected result:
	// 1, 4, 9, 16, ...
}