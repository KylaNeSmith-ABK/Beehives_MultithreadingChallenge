#include "Farmer.h"

Farmer::Farmer(std::string name, HoneyContainer& honeyContainer)
	: Actor(name)
	, HoneyContainer_Ptr_(&honeyContainer)
{
}

Farmer::~Farmer()
{
	Actor::~Actor();
}

void Farmer::Active()
{
	while (bIsActive_)
	{
		CollectHoney();
	}
}

void Farmer::CollectHoney()
{
	std::unique_lock<std::mutex> lock(g_HoneyMutex);
	g_HoneyFull.wait(lock, []() {return g_bHoneyContainerFull; });

	TakeHoney();

	lock.unlock();
	g_HoneyEmpty.notify_one();
}

void Farmer::TakeHoney()
{
	HoneyCollectedCounter_++;
	g_bHoneyContainerFull = false;
	printf("\n%s has collected honey %i times!\n\n", Name_.c_str(), HoneyCollectedCounter_);
}


