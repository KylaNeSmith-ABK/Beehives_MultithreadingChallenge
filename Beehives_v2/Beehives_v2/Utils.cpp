#include "Utils.h"

int GetRandomInt(int min, int max)
{
	int seedInt = std::chrono::system_clock::now().time_since_epoch().count();
	std::minstd_rand generator(seedInt);
	return ((generator() % (max - min)) + min);
}
