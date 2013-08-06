// Micha³ Krasowski July 2013
// Simple array_view tiling example
#include <amp.h>
#include <iostream>

using namespace concurrency;

const int ROWS = 4;
const int COLS = 4;

int main()
{
	// 16-element flat array is given
	int a[] = { 1,  2,  3,  4,
			    5,  6,  7,  8,
			    9, 10, 11, 12,
			   13, 14, 15, 16 };
	
	// definition of 2-dim array ranges
	extent<2> matrix(ROWS, COLS);
	
	array_view<int, 2> arrayView(matrix, a);

	parallel_for_each(
		arrayView.extent.tile<2,2>(), // creation of tiled_extent of size 2x2 elements
		[=](tiled_index<2,2> t_idx) restrict(amp) // tiled index size in lambda must match the size of the extent
		{
			arrayView[t_idx.global] = t_idx.tile[0] + t_idx.tile[1];
		}
	); 

	arrayView.synchronize();

	for (int i = 0; i < ROWS*COLS; i++)
	{
		std::cout << a[i] << ", " << ((i%4 == 3) ? "\n" : "");
	}

	char wait;
	std::cin >> wait;
}