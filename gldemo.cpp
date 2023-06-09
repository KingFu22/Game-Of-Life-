//
// Array Visualisation for CIS6007 WRIT1
//

#include <iostream>
#include <string>
#include <random>
#include "gl_stuff.h"
#include "Array2D.h"
#include <tbb/tbb.h>
#include <chrono>
#include <thread>


using namespace std;
using namespace tbb;


#pragma region Function Prototypes

void initPattern(Array2D<int>& array); // Initialise grid with "alive" and "dead" cell patterns
void update(void); // Main update function - apply Game of Life rules here

#pragma endregion


//
// Example array
//
Array2D<int>						myArray = Array2D<int>(128, 128);


// Starting point for our program...
int main(int argc, char* argv[])
{
	// Initialise OpenGL window...
	gl_init(argc, argv, update, "CIS6007 WRIT1 - Game of Life Demo", true);

	// Initialise our Game of Life starting pattern...
	initPattern(myArray);

	// Start the main loop - this calls our update function (below) to update our game of life generations.
	// Note: We never return from glutMainLoop until we quit the application
	glutMainLoop();

	return 0;
}


// Initialise grid with "alive" and "dead" cell patterns
void initPattern(Array2D<int>& array) {

	// 1. First clear the array (set all cells to "dead" ie. 0)
	parallel_for(
		0,
		array.w * array.h,
		[&](int i) {

			int x = i % array.w;
	int y = i / array.h;
	array(x, y) = 0;
		}
	);

	// Setup some cells to be "alive" - this is the initial state
	for (int i = 0; i < array.w && i < array.h; i++) {
		myArray(10, 2 + i) = 1;
		myArray(10, 3 + i) = 1;
		myArray(11, 2 + i) = 1;
		myArray(11, 3 + i) = 1;
		myArray(20, 2 + i) = 1;
		myArray(20, 3 + i) = 1;
		myArray(20, 4 + i) = 1;
		myArray(21, 1 + i) = 1;
		myArray(21, 5 + i) = 1;
		myArray(22, 0 + i) = 1;
		myArray(22, 6 + i) = 1;
		myArray(23, 0 + i) = 1;
		myArray(23, 6 + i) = 1;
		myArray(24, 3 + i) = 1;
		myArray(25, 1 + i) = 1;
		myArray(25, 5 + i) = 1;
		myArray(26, 2 + i) = 1;
		myArray(26, 3 + i) = 1;
		myArray(26, 4 + i) = 1;
		myArray(27, 3 + i) = 1;
		myArray(30, 2 + i) = 1;
		myArray(30, 1 + i) = 1;
		myArray(30, 0 + i) = 1;
		myArray(31, 2 + i) = 1;
		myArray(31, 1 + i) = 1;
		myArray(31, 0 + i) = 1;
		myArray(32, 3 + i) = 1;
		myArray(32, 4 + i) = 1;
		myArray(32, 5 + i) = 1;
		myArray(33, 3 + i) = 1;
		myArray(33, 4 + i) = 1;
		myArray(33, 5 + i) = 1;
		myArray(34, 2 + i) = 1;
		myArray(34, 6 + i) = 1;
		myArray(36, 1 + i) = 1;
		myArray(36, 2 + i) = 1;
		myArray(36, 6 + i) = 1;
		myArray(36, 7 + i) = 1;
		myArray(46, 3 + i) = 1;
		myArray(46, 4 + i) = 1;
		myArray(47, 3 + i) = 1;
		myArray(47, 4 + i) = 1;
		
	}
}
// NOTE: *** REPLACE THIS WITH YOUR OWN CODE TO SETUP THE LIVE CELL PATTERNS




// Main update function - apply Game of Life rules here
// Note: This is called by FreeGLUT's main event loop - YOU DON'T NEED TO CALL THIS - IT'S CALLED FOR YOU!!!
void update(void) {

	//
	// *** ADD SEQUENTIAL OR PARALLEL CODE HERE TO UPDATE GAME OF LIFE ***
	//

	// Create a new array to hold the next generation
	Array2D<int> nextGeneration = Array2D<int>(myArray.w, myArray.h);

	// Apply the rules of Conway's Game of Life to update the next generation
	/**auto startTime = chrono::system_clock::now();
	for (int y = 0; y < myArray.h; y++) {
		for (int x = 0; x < myArray.w; x++) {
			int neighbors = 0;
			for (int dy = -1; dy <= 1; dy++) {
				for (int dx = -1; dx <= 1; dx++) {
					if (dx == 0 && dy == 0) {
						continue;
					}
					int nx = x + dx;
					int ny = y + dy;
					if (nx >= 0 && nx < myArray.w && ny >= 0 && ny < myArray.h && myArray(nx, ny) == 1) {
						neighbors++;
					}
				}
			}
			if (myArray(x, y) == 1 && (neighbors == 2 || neighbors == 3)) {
				nextGeneration(x, y) = 1;
			}
			else if (myArray(x, y) == 0 && neighbors == 3) {
				nextGeneration(x, y) = 1;
			}
		}
		int totalIterations = 100;
		int iterationsBy10 = totalIterations / 10;
	}

	auto endTime = chrono::system_clock::now();

	chrono::duration<double> duration = endTime - startTime;
	cout << "Time taken = " << duration.count() << "seconds" << endl;*/

	auto startTime = chrono::system_clock::now();
	parallel_for(
		0,
		myArray.w * myArray.h,
		[&](int i) {

			int x = i % myArray.w;
	int y = i / myArray.h;

	// Count the number of live neighbors
	int liveNeighbors = 0;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			// Skip the center cell
			if (dx == 0 && dy == 0) {
				continue;
			}
			// Calculate the neighbor's coordinates
			int nx = x + dx;
			int ny = y + dy;
			// Make sure the neighbor is within the grid bounds
			if (nx >= 0 && nx < myArray.w && ny >= 0 && ny < myArray.h) {
				// Count the live neighbor
				if (myArray(nx, ny) == 1) {
					liveNeighbors++;
				}
			}
		}
	}
	// Apply the rules of Conway's Game of Life
	if (myArray(x, y) == 1 && (liveNeighbors < 2 || liveNeighbors > 3)) {
		// Any live cell with fewer than two live neighbors dies, as if caused by underpopulation.
		// Any live cell with more than three live neighbors dies, as if by overpopulation.
		nextGeneration(x, y) = 0;
	}
	else if (myArray(x, y) == 0 && liveNeighbors == 3) {
		// Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
		nextGeneration(x, y) = 1;
	}
	else {
		// Any live cell with two or three live neighbors lives on to the next generation.
		nextGeneration(x, y) = myArray(x, y);
	}
		}
	);
	auto endTime = chrono::system_clock::now();

	chrono::duration<double> duration = endTime - startTime;
	cout << "Time taken = " << duration.count() << "seconds" << endl;

	// Update myArray to the next generation
	myArray = nextGeneration;

	// Redraw the screen to show latest generation
	renderLatestGeneration(myArray);
}