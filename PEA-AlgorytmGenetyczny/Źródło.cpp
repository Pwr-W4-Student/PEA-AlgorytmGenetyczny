#include <iostream>
#include "myGraph.h"
#include <Windows.h>
#include<string>
#include <cstdlib>

using namespace std;

const char * menu = "1. Wczytaj plik\n2. wyswietl graf\n3. algrytm genetyczny\n4. test\n0. zamknij program\n";

string filePath;// = "D:\\403.txt";


double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

void runMenu(void)
{
	myGraph * graph;// = new myGraph((char*)filePath);
	bool exit = true;
	system("cls");
	do
	{
		system("cls");
		cout << menu;
		int chosen;
		cin >> chosen;
		switch (chosen)
		{
			case 1:
				//delete graph;
				cout << "podaj nazwe pliku" << endl;
				cin >> filePath;
				filePath += ".txt";

				graph = new myGraph(filePath);
				break;
			case 2:
				system("cls");
				if (graph != nullptr) graph->display();
				else cout << "\nGraph hasn't been loaded to program yet.\n";
				system("pause");
				break;
			case 3:
			{
				if (graph != nullptr)
				{
						int popSize, generations;
						float wspMutacji, wspKrzyzowania;
						double temperatureMax, temperatureMin, coolingRate;
						cout << "liczba populacji: ";
						cin >> popSize;
						if (popSize == 0) break;
						cout << "iteracje: ";
						cin >> generations;
						if (generations == 0) break;
						cout << "krzyzowanie [0,1) (use dot '.') : ";
						cin >> wspKrzyzowania;
						if (wspKrzyzowania == 0) break;
						cout << "mutacja [0,1) (use dot '.') : ";
						cin >> wspMutacji;
						if (wspMutacji == 0) break;
						double stopTime;
						cout << "Podaj czas stopu [s] ";
						cin >> stopTime;
						double time;
						StartCounter();
						graph->TSP_GeneticAlgorithm(popSize, generations, wspKrzyzowania, wspMutacji, stopTime*1000);
						time = GetCounter();
						cout << "Time: " << time/1000 << "s";
					cout << "\nResult: " << graph->getShortestPathWeight() << endl;
						cout << endl;
						system("pause");
						break;
				}
				break;
			}
			case 4:
			{
				int l = 2;
				double time;
				int result = 0;
				for (int i = 90; i <= 90; i += 30)
				{
					for (int j = 960; j <= 30720; j *= 2)
					{
						cout << "populacja: " << i << endl;
						cout << "iteracje: " << j << endl;
						time = 0;
						result = 0;
						StartCounter();
						for (int m = 0; m < l; m++)
						{
							StartCounter();
							graph->TSP_GeneticAlgorithm(i, j, 0.8, 0.01, 480 * 1000);
							result += graph->getShortestPathWeight();
							time += GetCounter();
						}
						
						cout << "Time: " << time / 1000 /l;
						cout << "\nResult: " << result/l << endl << endl << endl;
					}
				}
				system("pause");
				break;
			}

			case 0: 
			{	
				exit = false;
				break;
			}
			default:
				cout << "You've entered wrong numer\nTRY AGAIN!!!\n\n\n";
				system("pause");
				break;
		}
	} while (exit);
	delete graph;
	cout << "Goodbye :)\n";
	system("pause");
	return;
}


int main(void)
{
	runMenu();
	return 0; 
}
