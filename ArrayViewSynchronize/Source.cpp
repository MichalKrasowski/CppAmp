#include <amp.h>
#include <iostream>

using namespace concurrency;

int main()
{
	int aCpu[] = { 1,2,3,4,5,6 };
	int bCpu[] = { 1,2,3,4,5,6 };

	array_view<int, 1> aView(6, aCpu);
	array_view<int, 1> bView(6, bCpu);

	parallel_for_each(aView.extent, [=](index<1> idx) restrict(amp)
	{
		aView(idx) = 2 * aView(idx);
		bView(idx) = 2 * bView(idx);
	});

	aView.synchronize();

	for (int i = 0; i < 6; i++)
	{
		std::cout << " aCpu[" << i << "] = " <<  aCpu[i] << " ";
		std::cout << "aView[" << i << "] = " << aView[i] << " ";
		std::cout << " bCpu[" << i << "] = " <<  bCpu[i] << " ";
		std::cout << "bView[" << i << "] = " << bView[i] << "\n";

	}

	std::getchar();

}