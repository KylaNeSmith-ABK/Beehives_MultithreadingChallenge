#pragma once

#include <iostream>
#include <mutex>


class HoneyContainer
{
private:
	std::string Name_ = "HoneyContainerName";

	int MaxCapacity_ = 10;
	int HoneyAmount_ = 0;

	bool bIsFull_ = false;
	bool bIsBeingUsed_ = false;

	std::mutex g_HoneyMutex_;
	std::condition_variable g_HoneyFull_, g_HoneyEmpty_;

public:
	HoneyContainer(std::string name, int maxCapacity = 10);
	~HoneyContainer();

	static std::mutex g_HoneyMutex_;
	static std::condition_variable g_HoneyFull_, g_HoneyEmpty_;

	void AddHoney(int ammount);
	int TakeHoney();

};