#pragma once

#include <iostream>
#include <thread>
#include <chrono>

#include "Utils.h"
#include "Actor.h"
#include "HoneyContainer.h"

class Beehive : public Actor
{
private:
	int MaxHoneyToProduce_ = 2;
	int HoneyProducedCounter_ = 0;

	HoneyContainer* HoneyContainer_Ptr_ = nullptr;

	std::mutex* g_HoneyMutex_Ptr_ = nullptr;
	std::condition_variable* g_HoneyFull_Ptr_ = nullptr;
	std::condition_variable* g_HoneyEmpty_Ptr_ = nullptr;

	void Active();

	// locks and fills Honey Container
	void ProduceHoney();

	// places Honey in the Container, increments counter, and prints out information
	void FillHoneyContainer();

public:
	Beehive(std::string name, HoneyContainer& honeyContainer, int maxHoneyToProduce = 3);
	~Beehive();

	void MakeInactive() override;
};