// size.cc

#include <algorithm>  // accumulate bind for_each generate iota
#include <chrono>     // high_resolution_clock
#include <functional> // bind
#include <iomanip>    // setw
#include <iostream>   // cout
#include <random>     // *random* uniform_int_distribution
#include <vector>     // vector

#include "statistics.h" // mode

int main()
{
	const unsigned long long SIZE = 1 << 25;
	const unsigned GAP = 12;
	const unsigned REP = 32;
	const unsigned long long STEPS = 1000000;

	static_assert(sizeof(unsigned char) == 1, "sizeof(unsigned char) != 1");

	std::vector<unsigned char> bits(SIZE);

	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<unsigned> distribution;
	auto generator = std::bind(distribution, engine);
	std::generate(bits.begin(), bits.end(), generator);

	std::cout << "#" 
	          << std::setw(GAP - 1) << "size"
	          << std::setw(GAP    ) << "time"
	          << std::endl;

	for (unsigned long long size = 1 << 10; size <= SIZE; size <<= 1)
	{
		using namespace std::chrono;

		std::vector<nanoseconds::rep> sec(REP);
		for (auto &s: sec)
		{
			auto start = high_resolution_clock::now();
			// completar aquí
			auto stop = high_resolution_clock::now();
			s = duration_cast<nanoseconds>(stop - start).count() / STEPS;
		}

		std::cout << std::setw(GAP) << size
		          << std::setw(GAP) << std::fixed << std::setprecision(0)
		          << ec::mode(sec.begin(), sec.end()).begin()->first
		          << std::endl;
	}

	return std::accumulate(bits.begin(), bits.end(), 0);
}
