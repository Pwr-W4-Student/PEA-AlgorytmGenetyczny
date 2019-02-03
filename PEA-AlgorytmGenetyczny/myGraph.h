//#include"myStack.h"
#include <vector>
#include <time.h>
#include<fstream>
#include <random>
#include<iostream>
#include <string>

using namespace std;

class myGraph
{
	private:
		int size;
		int **matrix;
		int szczyt; 
		int tmpSzczyt;
		int Stos[100];
		int tmpStos[100];
		int *shortestPath;
		int shortestPathWeight;
		int tmpShortestPathWeight;
		std::vector <int> bestPath;
		std::vector <int> tmpPath;
		double Temperature;
		double minTemperature;
		double coolingRate;
		void swapVertex();

public:
	void StartCounter();
	double GetCounter();
	myGraph(string filePath);
		~myGraph();
		int getShortestPathWeight();
		int getSize();
		void display();
		int pathLength(std::vector<int> vector);
		std::vector<int> getBestPath();

		void TSP_GeneticAlgorithm(int, int, float,float, double);
	public:
		float wspMutacji;
		float wspKrzyzowania;
		int wielkoscPopulacji;
		int iloscPokolen; 
		std::vector<std::vector<int>> populacja;

	private:
		bool czyMutowac();
		bool czyKrzyzowac();
		void krzyzuj(double);
		void mutuj(double);
		void sortujOsobniki();
		bool isAlreadyThere(std::vector<int>, int);
		double PCFreq = 0.0;
		__int64 CounterStart = 0;

		long long int timer;


		struct doCompare
		{
			doCompare(myGraph& info) : m_info(info) { }
			myGraph& m_info;

			bool operator()(std::vector<int> sciezka1, std::vector<int> sciezka2)
			{
				return m_info.pathLength(sciezka1) < m_info.pathLength(sciezka2);
			}
		};
};