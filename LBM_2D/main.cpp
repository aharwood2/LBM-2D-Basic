/* LBM-2D-Basic created by Adrian Harwood, The University of Manchester, UK.
* Use of this software is covered by the Apache 2.0 License. */
#include "Grid.h"
#include <iostream>

// Entry point
int main()
{
	// Create a grid
	Grid grid(2.0, 1.0, 30, 0.001, 100, 0.08);

	// Run simulation for a certain number of timesteps
	for (int t = 0; t < 1000000; t++)
	{
		grid.timestep();
		std::cout << "t = " << t << '\r' << std::flush;
	}
		

	// Write out the results
	grid.writeOut();

	return 0;
}