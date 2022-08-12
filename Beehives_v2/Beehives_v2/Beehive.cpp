#include "Beehive.h"

Beehive::Beehive(std::string name, HoneyContainer& honeyContainer, int maxTimeHoneyProduced)
	: Actor(name)
	, HoneyContainer_Ptr_(&honeyContainer)
	, MaxHoneyToProduce_(maxTimeHoneyProduced)
{
}

Beehive::~Beehive()
{
	Actor::~Actor();
}

void Beehive::MakeInactive()
{
	bIsActive_ = false;
	printf("%s is no longer active\n", Name_.c_str());
}

void Beehive::Active()
{
	while (bIsActive_)
	{
		// wait 5 - 10 seconds
		int randInterval = GetRandomInt();
		std::this_thread::sleep_for(std::chrono::seconds(randInterval));

		ProduceHoney();
	}
}

void Beehive::ProduceHoney()
{
	std::unique_lock<std::mutex> lock(g_HoneyMutex);
	g_HoneyEmpty.wait(lock, []() { return !g_bHoneyContainerFull; });

	FillHoneyContainer();

	lock.unlock();
	g_HoneyFull.notify_all();
}

void Beehive::FillHoneyContainer()
{
	TimesHoneyProducedCounter_++;
	g_bHoneyContainerFull = true;
	printf("%s has produced honey! Count: %i\n", Name_.c_str(), TimesHoneyProducedCounter_);

	if (TimesHoneyProducedCounter_ >= MaxTimeHoneyProduced_)
	{
		MakeInactive();
	}
}