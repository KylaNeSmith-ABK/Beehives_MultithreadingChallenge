#pragma once

#include <iostream>
#include <thread>
#include <chrono>

class Actor
{
private:
	std::thread* ActiveThread_Ptr_ = nullptr;

	// what the Actor should be doing while active
	virtual void Active() = 0;
	
protected:
	std::string Name_ = "ActorName";

	bool bIsActive_ = true;

public:
	Actor(std::string name);
	~Actor();

	// runs the Actor's activity
	void Run() { ActiveThread_Ptr_ = new std::thread([this] { Active(); }); }

	bool IsActive() { return bIsActive_; }

	virtual void MakeInactive() { bIsActive_ = false; }
};