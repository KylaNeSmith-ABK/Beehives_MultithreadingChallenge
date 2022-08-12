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

int GetRandomInt(int min = 5, int max = 15)
{
	int seedInt = std::chrono::system_clock::now().time_since_epoch().count();
	std::minstd_rand generator(seedInt);
	return ((generator() % (max-min)) + min);
}

class Beehive
{
private:
	std::string Name_ = "Beehive";

	int MaxTimeHoneyProduced_ = 2;
	int TimesHoneyProducedCounter_ = 0;

	bool bIsActive_ = true;
	std::thread* ActiveBeehiveThread_ = nullptr;

	void Active()
	{
		while (bIsActive_)
		{
			// wait 5 - 10 seconds
			int randInterval = GetRandomInt();
			std::this_thread::sleep_for(std::chrono::seconds(randInterval));

			ProduceHoney();
		}
	
	}

	// locks and fills Honey Container
	void ProduceHoney()
	{
		std::unique_lock<std::mutex> lock(g_HoneyMutex);
		g_HoneyEmpty.wait(lock, []() { return !g_bHoneyContainerFull; });

		FillHoneyContainer();

		lock.unlock();
		g_HoneyFull.notify_all();
	}

	// places Honey in the Container, increments counter, and prints out information
	void FillHoneyContainer()
	{
		TimesHoneyProducedCounter_++;
		g_bHoneyContainerFull = true;
		printf("%s has produced honey! Count: %i\n", Name_.c_str(), TimesHoneyProducedCounter_);
		
		if (TimesHoneyProducedCounter_ >= MaxTimeHoneyProduced_)
		{
			MakeInactive();
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

	// runs the Beehive's thread
	void Run()
	{
		ActiveBeehiveThread_ = new std::thread([this] {Active(); });
	}

	bool IsActive()
	{
		return bIsActive_;
	}

	void MakeInactive()
	{
		bIsActive_ = false;
		printf("%s is no longer active\n", Name_.c_str());
	}
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
			CollectHoney();
		}
	}

	// locks and takes the Honey
	void CollectHoney()
	{
		std::unique_lock<std::mutex> lock(g_HoneyMutex);
		g_HoneyFull.wait(lock, []() {return g_bHoneyContainerFull; });

		TakeHoney();

		lock.unlock();
		g_HoneyEmpty.notify_one();
	}

	// removes Honey from the Container, increments counter, and prints out information
	void TakeHoney()
	{
		HoneyCollectedCounter_++;
		g_bHoneyContainerFull = false;
		printf("\n%s has collected honey %i times!\n\n", Name_.c_str(), HoneyCollectedCounter_);
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

	// runs the Farmers's thread
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
		// if even one is true, continue running
		bRun = HiveOne->IsActive()
			|| HiveTwo->IsActive()
			|| HiveThree->IsActive()
			|| HiveFour->IsActive()
			|| HiveFive->IsActive()
			|| g_bHoneyContainerFull
			|| g_bContainerInUse;
	}

	// stop the farmer
	FarmerJohn->MakeInactive();

	printf("\n\nThe farmer collected honey %i times\n", FarmerJohn->GetHoneyCollectedCount());

	// clean up
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