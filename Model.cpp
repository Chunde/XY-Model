#include "Model.h"
#include <fstream>
#include <string>

#include <iostream>
#define PI2 6.283185307179586476925286766559
inline double RandomAngel()
{
	double angle = static_cast <double> (rand()) * PI2 / (static_cast <double> (RAND_MAX));
	return angle;
}
inline double RandomUniform()
{
	double v = static_cast <double> (rand())  / (static_cast <double> (RAND_MAX));
	return v;
}
Model::Model(int dimX_, int dimY_, double J_, double beta_)
{
	dimX = dimX_;
	dimY = dimY_;
	J = J_;
	beta = beta_;
	latticeData.resize(dimX * dimY);
	hMatrix.resize(dimX * dimY);
	InitLattice();
	ComputeHMatrix();
}

Model::~Model()
{
}

void Model::InitLattice()
{
	for (int i = 0; i < dimX * dimY; i++)
		latticeData[i] = RandomAngel();
}

void Model::ComputeHMatrix()
{
	int id = 0;
	for (int y = 0; y < dimY; y++)
	{
		for (int x = 0; x < dimX; x++)
		{
			hMatrix[id] = ComputeSiteEnergy(x, y, latticeData[id]);
			id++;
		}
	}
	totalEnergy = 0;
	for (auto& e : hMatrix)
		totalEnergy += e;
}

void Model::Step()
{
	int x = rand() % dimX;
	int y = rand() % dimY;
	double changedValue = RandomAngel();
	double deltaH = ComputedEnergyChange(x, y, changedValue);
	if (deltaH > 0)
	{
		if (RandomUniform() < exp(-beta * deltaH)) //update the lattice and its energy
		{
			latticeData[y * dimX + x] = changedValue;
			ComputeHMatrix();
			UpdateHMatrix(x, y, changedValue);
		}
	}
	else
	{
		latticeData[y * dimX + x] = changedValue;
		ComputeHMatrix();
		UpdateHMatrix(x, y, changedValue);
	}
}

double Model::ComputedEnergyChange(int x, int y, double v)
{
	double newSiteEnergy = ComputeSiteEnergy(x, y, v);
	double delta = newSiteEnergy - hMatrix[y * dimX + x];
	return delta * 2.0; // its 4 nearest neighbors' energy change sum up to the same change too.
}

double Model::ComputeSiteEnergy(int x, int y, double v)
{
	double angleRight = latticeData[y * dimX + (x + 1) % dimX];
	double angleTop = latticeData[((y + 1) % dimY) * dimX + x];
	double angleLeft = latticeData[y * dimX + (x - 1 + dimX) % dimX];
	double angleButton = latticeData[((y - 1 + dimY) % dimY) * dimX + x];
	double siteEnergy = -cos(v - angleButton) - cos(v - angleLeft)
		- cos(v - angleRight) - cos(v - angleTop);
	siteEnergy *= J;
	return siteEnergy * 0.5;
}

inline double Model::ComputeSiteEnergy(int x, int y)
{
	x = (x + dimX) % dimX;
	y = (y + dimY) % dimY;
	return ComputeSiteEnergy(x, y, latticeData[y * dimX + x]);
}

inline void Model::UpdateHMatrix(int x, int y, double newAngle)
{
	double oldAngle = latticeData[y * dimX + x];
	double angleRight = latticeData[y * dimX + (x + 1) % dimX];
	double angleTop = latticeData[((y + 1) % dimY) * dimX + x];
	double angleLeft = latticeData[y * dimX + (x - 1 + dimX) % dimX];
	double angleButton = latticeData[((y - 1 + dimY) % dimY) * dimX + x];
	double dR = -0.5 * J * (cos(newAngle - angleRight) - cos(oldAngle - angleRight));
	double dT = -0.5 * J * (cos(newAngle - angleTop) - cos(oldAngle - angleTop));
	double dL = -0.5 * J * (cos(newAngle - angleLeft) - cos(oldAngle - angleLeft));
	double dB = -0.5 * J * (cos(newAngle - angleButton) - cos(oldAngle - angleButton));
	//need to update five sites
	hMatrix[y * dimX + (x + 1) % dimX] += dR;
	hMatrix[((y + 1) % dimY) * dimX + x] += dT;
	hMatrix[((y + 1) % dimY) * dimX + (x - 1 + dimX) % dimX] += dL;
	hMatrix[((y - 1 + dimY) % dimY) * dimX + x] += dB;
	hMatrix[y * dimX + x] += dR + dT + dL + dB;
	totalEnergy += 2 * (dR + dT + dL + dB); //update the total energy now is simple and fast
}


void Model::Simulate(int steps, int iterationPerStep, bool verbal)
{
	energyHistory.resize(steps);
	for (int step = 0; step < steps; step++)
	{
		for (int i = 0; i < iterationPerStep; i++)
			Step();
		energyHistory[step] = totalEnergy;
		if(verbal)
			std::cout << totalEnergy <<endl;
	}
}

void Model::SaveState(string fileName)
{
	ofstream file;
	file.open(fileName, std::ios::out | std::ios::binary);
	if (file.is_open())
	{
		file.write((char*)&dimX, sizeof(dimX));
		file.write((char*)&dimY, sizeof(dimY));
		file.write((char*)&beta, sizeof(beta));
		file.write((char*)&J, sizeof(J));
		file.write((char*)&totalEnergy, sizeof(totalEnergy));
		file.write((char*)latticeData.data(), sizeof(double) * latticeData.size());
		//int size = energyHistory.size();
		//file.write((char*)&size, sizeof(size));
		//file.write((char*)energyHistory.data(), sizeof(double) * energyHistory.size());
		file.close();
	}
	else
	{
		std::cout << "Error: failed to open " << fileName << endl;
	}

}
