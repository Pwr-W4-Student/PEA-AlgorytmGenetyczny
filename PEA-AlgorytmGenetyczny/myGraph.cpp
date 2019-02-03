#include"myGraph.h"



using namespace std;

#include <windows.h>



void myGraph::StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double myGraph::GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}


myGraph::myGraph(string filePath)
{
	szczyt = 0;
	tmpSzczyt = 0;
	std::fstream file;
	file.open(filePath, std::fstream::in);
	if (!file.is_open())
	{
		std::cout << "Error occured while opening file.\nCheck given file path.\n";
	}
	file >> this->size;
	matrix = new int *[this->size];
	for (int i = 0; i < size; i++)
	{
		matrix[i] = new int[this->size]; //utworzenie macierzy dynamicznie
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			char * tmp = new char;
			file >> tmp;
			matrix[i][j] = (int) atoi(tmp); // wczytane dane z pliku konwertowane do macierzy
		}
	}

	file.close();
}


myGraph::~myGraph()
{
	for (int i=0; i < this->size; i++)
	{
		delete[] this->matrix[i];
	}
	delete[] matrix;
}

void myGraph::display()
{
	cout << "Graph has: " << this->size << " vertices.\nCosts of travelling from vertex A to B are as following: \n  " ;
	for (int i = 0; i < this->size; i++)
	{
		cout << "  " << i;
	}
	cout << endl;
	for (int i = 0; i < this->size; i++)
	{
		cout << "---";
	}
	cout << endl; 
	for (int i = 0; i < this->size; i++)
	{
		for (int j = 0; j < this->size; j++)
		{
			if (j == 0) cout << i << "| ";
			cout << this->matrix[i][j] << " ";
		}
		cout << endl;
	}
}

int myGraph::getSize()
{
	return this->size;
}

int myGraph::getShortestPathWeight()
{
	return this->shortestPathWeight;
}

int myGraph::pathLength(vector<int> vector)
{
	int result = 0;
	int tmp = 0; 
	for (int i = 1; i < this->size; i++) //-1 bo dla ostatniego dadawane jest recznie
	{
		result += matrix[tmp][vector[i]];
		tmp = vector[i];
	}
	result += matrix[tmp][0];
	return result;
}

void myGraph::swapVertex() 
{
	int vertex1 = rand() % (this->size - 1)+1;
	int vertex2 = rand() % (this->size - 1)+1;
	while (vertex1 == vertex2)
	{
		vertex2 = rand() % (this->size - 1)+1;
	}
	int tmp = bestPath[vertex1];
	tmpPath = bestPath;
	tmpPath[vertex1] = tmpPath[vertex2];
	tmpPath[vertex2] = tmp;
}



std::vector<int> myGraph::getBestPath()
{
	return this->bestPath;
}
bool myGraph::isAlreadyThere(vector<int> v, int value)
{
	for (int x : v)
	{
		if (x == value) return true;
	}
	return false;
}

bool myGraph::czyMutowac()
{
	return wspMutacji > (float)((float)(rand()) / (float) RAND_MAX);
}

bool myGraph::czyKrzyzowac()
{
	return wspKrzyzowania > (float)((float)(rand()) / (float)RAND_MAX);
}

void myGraph::sortujOsobniki()
{
	std::sort(populacja.begin(), populacja.end(), doCompare(*this));
}



void myGraph::mutuj(double stopTime) 
{
	for (int i = 0 ; i < this->wielkoscPopulacji&& GetCounter() <= stopTime; i++)
	{
		if (czyMutowac())
		{//(rand()%(max-min +1) + min);
			int pointA = rand() % (this->size + 1);
			int pointB = rand() % (this->size - pointA + 1) + pointA;
			std::reverse(populacja[i].begin() + pointA, populacja[i].begin() + pointB);
		}
	}
}

void myGraph::krzyzuj(double stopTime)
{
	for (int father = 0; father < this->wielkoscPopulacji/3; father++)
	{

		for (int mother = 0; mother < this->wielkoscPopulacji/2; mother++)
		{
			if (czyKrzyzowac() && (mother!=father))
			{
				vector<int> child = vector<int>(this->size, -1);
				int pointA = rand() % (this->size);
				int pointB = rand() % (this->size - pointA ) + pointA;
				for (int i = pointA; i < pointB; i++) {
					child[i] = populacja[father][i];
				}
				int insertPosition = pointB;
				for (int i = pointB; i < this->size; i++) {
					if (!isAlreadyThere(child, populacja[mother][i])) {
						child[insertPosition] = populacja[mother][i];
						insertPosition = (insertPosition < this->size - 1) ? (insertPosition + 1) : 0;
					}
				}
				for (int i = 0 ; i < pointB; i++)
				{
					if (!isAlreadyThere(child, populacja[mother][i])) {
						child[insertPosition] = populacja[mother][i];
						insertPosition = (insertPosition < (this->size - 1)) ? (insertPosition + 1) : 0;
					}
				}
				populacja.push_back(child);
			}
		}
	}
}


void myGraph::TSP_GeneticAlgorithm(int popSize, int gen, float crossProp, float mutProp, double stopTime)
{
	StartCounter();
	this->wielkoscPopulacji = popSize;
	this->iloscPokolen = gen;
	this->wspKrzyzowania = crossProp;
	this->wspMutacji = mutProp;
	populacja.clear();

	srand(time(NULL)); 
	
	for (int i = 0 ; i < wielkoscPopulacji; i++) //populacja poczatkowa
	{
		vector<int> tmpPath = vector<int>(this->size);
		for (int i = 0 ; i < this->size; i++)
		{
			tmpPath[i] = i;
		}
		for (int i = 0 ; i < this->size; i++)
		{
			int r = rand() % (this->size);
			int temp = tmpPath[i];
			tmpPath[i] = tmpPath[r];
			tmpPath[r] = temp;
		}

		populacja.push_back(tmpPath);
	}

	bestPath = populacja[0];

	sortujOsobniki();

	if (pathLength(populacja[0]) < pathLength(bestPath))
	{
		bestPath = populacja[0];
	}
	for (int i=1 ; i < this->iloscPokolen && GetCounter() <= stopTime; i++)
	{
		
		populacja.resize(wielkoscPopulacji); //selekcja naturalna - zmniejeszenie populacji do wartosci 'wielkoscPopulacji'
		krzyzuj(stopTime);
		mutuj(stopTime);

		sortujOsobniki();
		if (pathLength(populacja[0]) < pathLength(bestPath))
		{
			bestPath = populacja[0];
		}
	}

	shortestPathWeight = pathLength(bestPath);
}