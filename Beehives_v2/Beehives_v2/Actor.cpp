#include "Actor.h"

Actor::Actor(std::string name)
	: Name_(name)
{
}

Actor::~Actor()
{
	if (ActiveThread_Ptr_)
	{
		ActiveThread_Ptr_->join();
	}
	delete ActiveThread_Ptr_;
}

void Actor::LinkMutex(std::mutex* mutex, std::condition_variable* condVar1, std::condition_variable* cond_var2)
{
	Mutex_Ptr_ = mutex;
	CondVar1_Ptr_ = condVar1;
	CondVar2_Ptr_ = cond_var2;
}
