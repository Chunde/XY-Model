#pragma once
#include <vector>
using namespace std;

class Model
{
private:
	int dimX;
	int dimY;
	vector<double> latticeData;
	vector<double> hMatrix;
	vector<double> energyHistory;
	double beta;
	double J;
	double totalEnergy;
protected:
	void InitLattice();
	void ComputeHMatrix();
	void Step();
	double ComputedEnergyChange(int x_, int y_, double v_);
	inline double ComputeSiteEnergy(int x, int y, double refAngel);
	inline double ComputeSiteEnergy(int x, int y);
	inline void UpdateHMatrix(int x, int y, double refAngel);
public:
	Model(int dimX_, int dimY_, double J_, double beta_);
	~Model();
	void Simulate(int steps, int iterationPerStep, bool verbal = false);
	void SaveState(string fileName);
};

