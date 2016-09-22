// line.cc

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
	const unsigned long long LINE = 1 << 10;	//2^10
	const unsigned GAP = 12;
	const unsigned REP = 32;

	static_assert(sizeof(unsigned char) == 1, "sizeof(unsigned char) != 1");

	std::vector<unsigned char> bits(1 << 26);	//Se declara un vector de 64mb
//Inicializacion del vector
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_int_distribution<unsigned> distribution;
	auto generator = std::bind(distribution, engine);
	std::generate(bits.begin(), bits.end(), generator);

	std::cout << "#" 
	          << std::setw(GAP - 1) << "line"
	          << std::setw(GAP    ) << "time"
	          << std::endl;
//El bucle pasa 1,2,4,8....
	for (unsigned long long line = 1; line <= LINE; line <<= 1)
	{
		using namespace std::chrono;
		//Define un vector, que lo ejecuta 32 veces y lo promedia
		std::vector<nanoseconds::rep> sec(REP);
		for (auto &s: sec)
		{
			auto start = high_resolution_clock::now();
                        // completar aquí	//con los dos bucles internos, los que recorren de line en line y line veces el de fuera.
						//for for						
						//bits[i]++; lo de dentro
			auto stop = high_resolution_clock::now();
			s = duration_cast<milliseconds>(stop - start).count();
		}

		std::cout << std::setw(GAP) << line
		          << std::setw(GAP) << std::fixed << std::setprecision(0)
		          << ec::median(sec.begin(), sec.end()) //cambiar mode por median
		          << std::endl;
	}

	return std::accumulate(bits.begin(), bits.end(), 0);
}
