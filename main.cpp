#include "Model.h"
#include <ctime> 
#include <string>
#include <iostream>
#include <thread>
#include <mutex>          // std::mutex
#include <stack>
#include "Profile.hpp"
#include <fstream>
std::mutex mtx;
std::stack<double> temperatures;
string stateFileFolder = "c:\\XYModel_State\\";

double RequestTemperature()
{
	double t = -1;
	mtx.lock();
	if (temperatures.size() > 0)
	{
		t = temperatures.top();
		temperatures.pop();
		std::cout << "stack size: " << temperatures.size() << endl;
	}
	mtx.unlock();
	return t;
}
void ThreadDelegate(double temp)
{
	Model md(50, 50, 1.0, temp);
	md.Simulate(1000, 2500);
	md.SaveState(stateFileFolder + std::to_string(temp) + ".xyms");
}
void task(int id)
{
	do
	{
		srand((unsigned)time(0));
		double t = RequestTemperature();
		if (t < 0)
		{
			std::cout << "Thread End(" << id << ")" << endl;
			break;
		}
		std::cout << "Thread " << id << " new temperature: " << t << endl;
		Model md(100, 100, 1.0, t);
		std::cout << "Excuting Time for temperature " << t << " is " << Time_function(ThreadDelegate, t) << endl;
	} while (1);
	std::cout << "Task with ID=" << id << " exits" << endl;
}
void TestReadStateFile()
{
	ifstream file("c:\\XYModel_State\\1.000000.xyms");
	if (file.is_open())
	{
		int x, y;
		file >> x >> y;
		double v;
		file >> v;
	}
	
}
#define MAX_THREAD 12
int main()
{
	//TestReadStateFile();
	unsigned int nthreads = std::thread::hardware_concurrency();
	if (nthreads == 0)
		nthreads = 1;
	for (size_t i = 0; i < 100; i++)
	{
		temperatures.push(1.0 / ((i + 1) * 0.01));
	}
	for (size_t i = 0; i < 300; i++)
	{
		temperatures.push(1.0 / ((i + 1) * 0.2));
	}
	std::thread threads[MAX_THREAD];
	// spawn 10 threads:
	for (int i = 0; i< MAX_THREAD; ++i)
		threads[i] = std::thread(task, i + 1);

	for (auto& th : threads) th.join();
	std::cout << "all threads are done, press any key to exit..." << endl;
	std::getchar();
	return 0;
}