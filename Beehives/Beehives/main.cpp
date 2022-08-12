#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>

class Beehive;
class Farmer;

std::mutex g_HoneyMutex;
std::condition_variable g_HoneyFull, g_HoneyEmpty;
bool g_bHoneyContainerFull = false;
bool g_bContainerInUse = false;

int GetRandomInt()
{
	int seedInt = std::chrono::system_clock::now().time_since_epoch().count();
	std::minstd_rand generator(seedInt);
	return ((generator() % 10) + 5);
}

class Beehive
{
private:
	std::string Name_ = "Beehive";

	int MaxTimeHoneyProduced_ = 2;
	int TimesHoneyProducedCounter_ = 0;

	bool bIsActive_ = true;
	std::thread* ActiveBeehiveThread_ = nullptr;

	//bool bHasHoney = false;

	void Active()
	{
		while (bIsActive_)
		{
			//int randInterval = rand() % 15 + 1;

			// --- ATTEMPT 1 && 5---
			int randInterval = GetRandomInt();
			std::this_thread::sleep_for(std::chrono::seconds(randInterval));

			std::unique_lock<std::mutex> lock(g_HoneyMutex);
			g_HoneyEmpty.wait(lock, []() { return !g_bHoneyContainerFull; });

			ProduceHoney();

			lock.unlock();
			g_HoneyFull.notify_all();


			// --- ATTEMPT 2 ---
			/*std::lock_guard<std::mutex> lock(g_HoneyMutex);
			while (g_bHoneyContainerFull)
			{

			}

			ProduceHoney();*/


			//if (TimesHoneyProducedCounter_ >= MAXTIMESHONEYPRODUCED_)
			//{
			//	bIsActive_ = false;
			//	printf("%s is no longer active\n", Name_.c_str());
			//	//std::cout << Name_ << " is no longer active" << std::endl;
			//}


			// --- ATTEMPT 4 ---
			/*int randInterval = GetRandomInt();
			std::this_thread::sleep_for(std::chrono::seconds(randInterval));

			while (g_bContainerInUse || g_bHoneyContainerFull)
			{

			}

			g_bContainerInUse = true;
			ProduceHoney();
			g_bContainerInUse = false;*/


			// --- ATTEMPT 3 ---
			/*if (!bHasHoney)
			{
				int randInterval = GetRandomInt();
				std::this_thread::sleep_for(std::chrono::seconds(randInterval));
				bHasHoney = true;
			}

			if (bHasHoney && !g_bContainerInUse && !g_bHoneyContainerFull)
			{
				g_bContainerInUse = true;
				ProduceHoney();
				g_bContainerInUse = false;
			}*/
		}
	
	}

	void ProduceHoney()
	{
		TimesHoneyProducedCounter_++;
		g_bHoneyContainerFull = true;
		printf("%s has produced honey! Count: %i\n", Name_.c_str(), TimesHoneyProducedCounter_);
		//std::cout << Name_ << " has produced honey!" << std::endl;

		// --- ATTEMPT 3 && 4 && 5---
		if (TimesHoneyProducedCounter_ >= MaxTimeHoneyProduced_)
		{
			bIsActive_ = false;
			printf("%s is no longer active\n", Name_.c_str());
			//std::cout << Name_ << " is no longer active" << std::endl;
		}
	}

public:
	Beehive(std::string name, int maxTimeHoneyProduced = 3)
		: Name_(name)
		, MaxTimeHoneyProduced_(maxTimeHoneyProduced)
	{}

	~Beehive()
	{
		if (ActiveBeehiveThread_)
		{
			ActiveBeehiveThread_->join();
		}
		delete ActiveBeehiveThread_;
	}

	void Run()
	{
		ActiveBeehiveThread_ = new std::thread([this] {Active(); });
	}

	bool IsActive() { return bIsActive_; }
};

class Farmer
{
private:
	std::string Name_ = "Farmer";
	int HoneyCollectedCounter_ = 0;

	bool bIsActive_ = true;
	std::thread* ActiveFarmerThread_ = nullptr;

	void Active()
	{
		while (bIsActive_)
		{
			// --- ATTEMPT 1 && 5---
			std::unique_lock<std::mutex> lock(g_HoneyMutex);
			g_HoneyFull.wait(lock, []() {return g_bHoneyContainerFull; });

			CollectHoney();

			lock.unlock();
			g_HoneyEmpty.notify_one();


			// --- ATTEMPT 2 ---
			/*std::lock_guard<std::mutex> lock(g_HoneyMutex);
			if (g_bHoneyContainerFull)
			{
				CollectHoney();
			}*/


			// --- ATTEMPT 3 && 4---
			/*if (g_bHoneyContainerFull)
			{
				while (g_bContainerInUse)
				{

				}

				g_bContainerInUse = true;
				CollectHoney();
				g_bContainerInUse = false;
			}*/
		}
	}

	void CollectHoney()
	{
		HoneyCollectedCounter_++;
		g_bHoneyContainerFull = false;
		printf("\n%s has collected honey %i times!\n\n", Name_.c_str(), HoneyCollectedCounter_);
		//std::cout << Name_ << " has collected honey " << HoneyCollectedCounter_ << " times!" << std::endl;
	}

public:
	Farmer(std::string name)
		: Name_(name)
	{

	}

	~Farmer()
	{
		if (ActiveFarmerThread_)
		{
			ActiveFarmerThread_->join();
		}
		delete ActiveFarmerThread_;
	}

	void Run()
	{
		ActiveFarmerThread_ = new std::thread([this] {Active(); });
	}

	bool IsActive()
	{
		return bIsActive_;
	}

	void MakeInactive()
	{
		bIsActive_ = false;
	}

	int GetHoneyCollectedCount()
	{
		return HoneyCollectedCounter_;
	}
};

int main()
{
	bool bRun = true;

	Beehive* HiveOne = new Beehive("France", 2);
	HiveOne->Run();

	Beehive* HiveTwo = new Beehive("Britain");
	HiveTwo->Run();

	Beehive* HiveThree = new Beehive("Germany", 6);
	HiveThree->Run();

	Beehive* HiveFour = new Beehive("Japan", 2);
	HiveFour->Run();

	Beehive* HiveFive = new Beehive("USA", 1);
	HiveFive->Run();


	Farmer* FarmerJohn = new Farmer("John");
	FarmerJohn->Run();

	while (bRun)
	{
		bRun = HiveOne->IsActive()
			|| HiveTwo->IsActive()
			|| HiveThree->IsActive()
			|| HiveFour->IsActive()
			|| HiveFive->IsActive()
			|| g_bHoneyContainerFull
			|| g_bContainerInUse;
	}

	FarmerJohn->MakeInactive();

	std::cout << std::endl;
	printf("\nThe farmer collected honey %i times\n", FarmerJohn->GetHoneyCollectedCount());
	//std::cout << "The farmer collected honey " << FarmerJohn->GetHoneyCollectedCount() << " times" << std::endl;

	delete HiveOne;
	HiveOne = nullptr;
	delete HiveTwo;
	HiveTwo = nullptr;
	delete HiveThree;
	HiveThree = nullptr;
	delete HiveFour;
	HiveFour = nullptr;
	delete HiveFive;
	HiveFive = nullptr;


	delete FarmerJohn;
	FarmerJohn = nullptr;
}