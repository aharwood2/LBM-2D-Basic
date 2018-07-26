#include "Grid.h"
#include <iostream>

// Entry point
int main()
{
	// Create a grid
	Grid grid(2.0, 1.0, 50, 0.001, 500.0, 0.00001);

	// Run simulation for a certain number of timesteps
	for (int t = 0; t < 10000; t++)
	{
		grid.timestep();
		std::cout << "t = " << t << '\r' << std::flush;
	}
		

	// Write out the results
	grid.writeOut();

	return 0;
}