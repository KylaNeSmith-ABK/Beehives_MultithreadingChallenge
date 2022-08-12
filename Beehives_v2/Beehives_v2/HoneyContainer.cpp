#include "HoneyContainer.h"

HoneyContainer::HoneyContainer(std::string name, int maxCapacity)
{
}

HoneyContainer::~HoneyContainer()
{
}

void HoneyContainer::AddHoney(int amount)
{
	HoneyAmount_ += amount;
}

int HoneyContainer::TakeHoney()
{
	int ret = HoneyAmount_;
	HoneyAmount_ = 0;
	return ret;
}
