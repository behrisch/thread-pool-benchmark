#include <iostream>
#include <cstdlib>
#include "pool.hpp"

using namespace std;

const int COUNT = 1'000'000;
const int REPS = 10;

int main(int argc, char** argv)
{
	cout << "simple_thread_pool";
	{
		srand(0);
		simple_thread_pool tp;
		for (int i = 0; i < COUNT; ++i)
			tp.enqueue_work([i]()
				{
					int reps = REPS + (REPS * (rand() % 5));
					for (int n = 0; n < reps; ++n)
						int x = i + rand();
				});
	};

	cout << "thread_pool";
	{
		srand(0);
		thread_pool tp;
		for (int i = 0; i < COUNT; ++i)
			tp.enqueue_work([i]()
				{
					int reps = REPS + (REPS * (rand() % 5));
					for (int n = 0; n < reps; ++n)
						int x = i + rand();
				});
	};
}
