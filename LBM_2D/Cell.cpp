#include "Cell.h"
#include "constants.h"

/* Default consturctor */
Cell::Cell()
{
}

/* Destrcutor */
Cell::~Cell()
{
}

/* Custom consturctor */
Cell::Cell(int xpos, int ypos, int label, double gravity_lbm)
{
	// Assign position and label
	x = xpos;
	y = ypos;
	type = label;

	// Initialise the velocity to zero
	ux = 0.0;
	uy = 0.0;

	// Initialise density to one
	density = 1.0;

	// Intialise force
	forcex = density * gravity_lbm;

	// Initialise f values to their equilibrium value
	for (int v = 0; v < 9; v++)
	{
		f[v] = equilibrium(v);
		fnew[v] = equilibrium(v);
	}
}

/* Method to compute local equilibrium */
double Cell::equilibrium(int v)
{
	// Declare intermediate values A and B
	double A, B;

	// Compute the parts of the expansion for feq
	A = (c[v][0] * ux) + (c[v][1] * uy);

	B = ((c[v][0] * c[v][0]) - (cs * cs)) * (ux * ux) +
		((c[v][1] * c[v][1]) - (cs * cs)) * (uy * uy) +
		2 * c[v][0] * c[v][1] * ux * uy;

	// Compute f^eq
	return density * w[v] * (1.0 + (A / (cs * cs)) + (B / (2.0 * (cs * cs * cs * cs))));
}

/* Method to update macroscopic quantities */
void Cell::updateMacroscopic()
{
	// Reset macroscopic
	ux = 0.0;
	uy = 0.0;
	density = 0.0;
	
	// Loop over velocities
	for (int v = 0; v < 9; v++)
	{
		// Do summations required by moments
		density += fnew[v];
		ux += c[v][0] * fnew[v];
		uy += c[v][1] * fnew[v];
	}

	// Add forcing to X momentum
	ux += 0.5 * forcex;

	// Divide by density to get velocity
	ux /= density;
	uy /= density;
}

/* Method to perform BGK collision */
void Cell::collide(double tau)
{
	// Only collide if a fluid site
	if (type != 1) return;

	// Perform collision operation
	for (int v = 0; v < 9; v++)
	{
		fnew[v] += (1.0 / tau) * (equilibrium(v) - fnew[v]) + force(tau, v);
	}
}

/* Method to compute Guo forcing */
double Cell::force(double tau, int v)
{
	// Intermediate variables
	double beta_v = 0.0;

	// Compute the lattice forces based on Guo's forcing scheme
	double lambda_v = (1.0 - 0.5 * (1.0 / tau)) * (w[v] / (cs*cs));

	// Dot product (sum over d dimensions)
	beta_v = ((c[v][0] * ux) + (c[v][1] * uy)) * (1 / (cs*cs));

	// Compute force
	return (forcex * (c[v][0] * (1.0 + beta_v) - ux)) * lambda_v;
}
