/* LBM-2D-Basic created by Adrian Harwood, The University of Manchester, UK.
* Use of this software is covered by the Apache 2.0 License. */
#include "Grid.h"
#include "constants.h"
#include <iostream>
#include <fstream>

/* Default consturctor */
Grid::Grid()
{
}

/* Destrutor */
Grid::~Grid()
{
}

/* Custom consturctor */
Grid::Grid(double width, double height, int resolution, double timestep, double re, double gravity)
{
	// Populate the member variables from dimensionless data
	reynolds = re;
	dx = 1.0 / static_cast<double>(resolution);
	dt = timestep;

	// Work out number of cells
	nx = static_cast<int>(std::floor(static_cast<double>(resolution) * width));
	ny = static_cast<int>(std::floor(static_cast<double>(resolution) * height));

	// Work out the viscosity in LBM units
	nu = (1.0 / reynolds) * dt / (dx * dx);

	// Work out the relaxation time
	tau = (nu / (cs * cs)) + 0.5;

	// Work out gravity force in LBM units
	double grav_lbm = gravity * dt * dt / dx;

	// Now create cells
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			// Create solid walls and an object using labels
			if (j == 0 || j == ny - 1)
			{
				// Wall
				cells.push_back(new Cell(i, j, 0, 0.0));
			}
			else
			{
				// Fluid
				cells.push_back(new Cell(i, j, 1, grav_lbm));
			}			
		}
	}
}

/* Method to write velocity to file */
void Grid::writeOut()
{
	// Create a CSV file
	std::ofstream file;
	file.open("./velocity.csv", std::ios::out);
	if (file.is_open())
	{
		// Line 1 = nx
		file << nx << ',';

		// Line 2 = ny
		file << ny;

		// Line 3+ = velocity
		for (size_t idx = 0; idx < cells.size(); idx++)
			file << ',' << std::sqrt(cells[idx]->ux * cells[idx]->ux + cells[idx]->uy * cells[idx]->uy);

		// Close file
		file.close();
	}
}

/* Method to perform a timestep */
void Grid::timestep()
{
	// Stream calls collide()
	stream();

	// Last step is to swap f and fnew pointers for next time step
	for (int idx = 0; idx < nx * ny; idx++)
	{
		double * tmp = cells[idx]->f;
		cells[idx]->f = cells[idx]->fnew;
		cells[idx]->fnew = tmp;		
	}
}

/* Stream-Collide method */
void Grid::stream()
{
	// Loop over the cells
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			// Compute 1D index of current cell
			int currentCell = j + i * ny;

			// Loop over each lattice velocity
			for (int v = 0; v < 9; v++)
			{
				// Get coordinates of cell from where f value will be pulled
				int i_adj = i - c[v][0];
				int j_adj = j - c[v][1];

				// Apply periodic boundary condtions
				if (i_adj < 0) i_adj = nx - 1;
				if (j_adj < 0) j_adj = ny - 1;
				if (i_adj > nx - 1) i_adj = 0;
				if (j_adj > ny - 1) j_adj = 0;

				// Compute 1D index into array of cells
				int adjacentCell = j_adj + i_adj * ny;

				// Only need to change fluid cells as solid cells are not simulated
				if (cells[currentCell]->type == 1)
				{
					// Read value of f from adjacent cell and write it to the 
					// fnew array of the current cell. Unless adjacent cell is 
					// a solid cell in which case apply no-slip BC.
					if (cells[adjacentCell]->type == 0)
					{
						// Apply no-slip
						cells[currentCell]->fnew[v] = cells[currentCell]->f[vopp[v]];
					}
					else
					{
						// Assume adjacent cell is a fluid cell
						cells[currentCell]->fnew[v] = cells[adjacentCell]->f[v];
					}					
				}
			}

			// Update the macroscopic values for this cell
			cells[currentCell]->updateMacroscopic();

			// Now collide in the cell
			cells[currentCell]->collide(tau);
		}
	}
}
