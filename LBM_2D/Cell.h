#pragma once
class Cell
{
	friend class Grid;

public:
	Cell();				// Constructor
	~Cell();			// Destructor

	// Constructor providing position of cell and type
	Cell(int xpos, int ypos, int label, double gravity_lbm);

private:
	int type;			// Cell type 1 = fluid, 0 = solid
	double * f = new double[9];		// F values before streaming
	double * fnew = new double[9];	// F values after streaming
	int x;				// X position in the domain (lattice units)
	int y;				// Y position in the domain (lattice units)
	double ux;			// X velocity
	double uy;			// Y velocity
	double density;		// Density
	double forcex;		// Force in x direction

	// Equilibrium function to compute f^eq
	double equilibrium(int v);
	
	// Update macroscopic
	void updateMacroscopic();

	// Collide
	void collide(double tau);

	// Compute force
	double force(double tau, int v);
};

