#pragma once
#include <vector>
#include "Cell.h"

class Grid
{
public:
	Grid();						// Constructor
	~Grid();					// Destructor
	
	// Overloaded constructor with simulation setup parameters
	Grid(double width, double height, int resolution, double timestep,
			double re, double gravity);

	void writeOut();			// Method for writing out the velocity
	void timestep();			// Method for performing a single timestep

private:
	double dx;					// Lattice spacing (dimensionless units)
	double dt;					// Timestep (dimensionless units)
	double tau;					// Relaxation time
	double nu;					// Viscosity
	double reynolds;			// Reynolds number
	std::vector<Cell*> cells;	// Array of cells representing the grid
	int nx;						// Number of cells in the X direction
	int ny;						// Number of cells in the Y direction

	// LBM methods
	void stream();
};

