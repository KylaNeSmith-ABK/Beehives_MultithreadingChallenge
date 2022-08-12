#pragma once

#include <iostream>
#include <thread>
#include <chrono>

#include "Utils.h"
#include "Actor.h"
#include "HoneyContainer.h"

class Farmer : public Actor
{
private:
	int ContainerInteractionsCounter_ = 0;
	int HoneyCollectedCounter_ = 0;

	HoneyContainer* HoneyContainer_Ptr_ = nullptr;

	void Active() override;

	// locks and takes the Honey
	void CollectHoney();

	// removes Honey from the Container, increments counter, and prints out information
	void TakeHoney();

public:
	Farmer(std::string name, HoneyContainer& honeyContainer);
	~Farmer();

	void MakeInactive() { bIsActive_ = false; }

	int GetHoneyCollectedCount() { return HoneyCollectedCounter_; }
};