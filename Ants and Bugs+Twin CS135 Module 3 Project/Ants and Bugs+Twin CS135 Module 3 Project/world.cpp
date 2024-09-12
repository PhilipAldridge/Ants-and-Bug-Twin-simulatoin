#include <iostream>
#include <cstdlib>  // for rand
#include "World.h"
#include "Ant.h"
#include "Bug.h"
#include "TWINBUG.h"
#include "Windows.h"

using namespace std;

extern Position Blast_Radius[18]; // blast radious finds all cells impacted by the blast and removies them from the grid
extern World myWorld; // myworld is the variable of class World
extern bool Twins_Touched; // determins if Two TWINBUGS touch
extern void displaytitle(); // displays title

///////////////////
// Public functions
///////////////////

// Default constructor: creates and initializes the world
// the seed is used for seeding the random behaviour.
World::World(unsigned int seed) {
	// seed the random generator
	srand(seed);
	// Create an empty world
	for (int i = 0; i < WORLDSIZE; i++) {
		for (int j = 0; j < WORLDSIZE; j++) {
			grid[i][j] = NULL;
		}
	}
	// creates the ants = 100
	createOrganisms(ANT, INITIAL_ANTS);
	// creates the bugs = 5
	createOrganisms(BUG, INITIAL_BUGS);
	// creates the bugs = 2
	createOrganisms(TWINBUG, INITIAL_TWINBUG);
	for (int i = 0; i < 18; i++) { Blast_Radius[i].x = -1; Blast_Radius[i].y = -1; } // initialize values outside of world size
}

// Deallocate memory allocated to organisms in this world.
World::~World() {
	for (int i = 0; i < WORLDSIZE; i++) {
		for (int j = 0; j < WORLDSIZE; j++) {
			if (grid[i][j] != NULL) {
				delete grid[i][j];
			}
		}
	}
}

// Return the organism at the given coordinates
// If the coordinates are not valid, returns NULL
Organism* World::getAt(int x, int y) const {
	if ((x >= 0) && (x < WORLDSIZE) && (y >= 0) && (y < WORLDSIZE)) {
		return grid[x][y];
	}
	else {
		return NULL;
	}
}

// Sets the entry at x,y to the value passed in.
void World::setAt(int x, int y, Organism* org) {
	if ((x >= 0) && (x < WORLDSIZE) && (y >= 0) && (y < WORLDSIZE)) {
		grid[x][y] = org;
	}
}

// Kill the organism at x,y due to explosion
void World::killAt(int x, int y) 
{
	if ((x >= 0) && (x < WORLDSIZE) && (y >= 0) && (y < WORLDSIZE) && grid[x][y] != NULL)
	{
		delete grid[x][y];
		grid[x][y] = NULL;
	}
}

// checks if cordinates are in the blast radius so display can change them to red stars
bool bombradius(int x, int y)
{
	for (int i = 0; i < 18; i++)
		if (x == Blast_Radius[i].x && y == Blast_Radius[i].y)
			return true;

	return false;
}



// Displays the world in ASCII.
void World::display() const {
	int numAnts = 0;
	int numBugs = 0;
	int numTwinbug = 0;
	cout << endl << endl;
	for (int j = 0; j < WORLDSIZE; j++) {
		cout << "                                                 ";
		for (int i = 0; i < WORLDSIZE; i++) {
			if (bombradius(i, j))
			{
				cout << "\033[7;31m\033[31m" << '*' << "\033[m"; // changes bugs in blast radius to red stars
			}
			else
				if (grid[i][j] == NULL) {
					cout << ".";
				}
			else {
				if (grid[i][j]->getType() == ANT) {
					numAnts++;
				}
				else if (grid[i][j]->getType() == BUG) {
					numBugs++;
				}
				else if (grid[i][j]->getType() == TWINBUG) { // twinbug
					numTwinbug++;
				}
				cout << grid[i][j]->representation();
			}
		}
		cout << endl;
	}
	cout << "\n                                              ";
	cout << "Ants: " << numAnts << " Bugs: " << numBugs << " Twinbug: " << numTwinbug << endl;
}

void World::simulateOneStep() {
	// The main routine that simulates one turn in the world:
	// 1. move bugs
	// 2. move ants
	// 3. make bugs starve (which happends under a certain condition)
	// 4. make the organisms breed (again which happens under a certain
	// condition).

	// Reset all organisms to not moved
	resetOrganisms();

	// Move the Twinbug twice
	moveOrganism(TWINBUG);
	resetOrganisms();
	moveOrganism(TWINBUG);

	// Move the bugs
	moveOrganism(BUG);

	// Move the ants
	moveOrganism(ANT);

	// Make the bugs starve
	cleanup();

	//Make them breed
	breedOrganisms();
}

Position World::randomPosition() const {    // returns a random number in the range 0 to MAX-1
	Position p;
	p.x = rand() % WORLDSIZE;
	p.y = rand() % WORLDSIZE;
	return p;
}


Move World::randomMove() const {
	return static_cast<Move>(rand() % 4);
}

////////////////////
// Private functions
////////////////////

void World::createOrganisms(OrganismType orgType, int count) {
	int orgCount = 0;
	while (orgCount < count) {
		Position p = randomPosition();

		// Only put ant in empty spot
		if (grid[p.x][p.y] == NULL) {
			orgCount++;
			if (orgType == ANT) {
				new Ant(this, p.x, p.y);   // Create an Ant and put it into the world
			}
			else if (orgType == BUG) {
				new Bug(this, p.x, p.y);   // Create a Bug and put it into the world
			}
			else if (orgType == TWINBUG) { // twinbug
				new Twinbug(this, p.x, p.y);   // Create a Bug and put it into the world
			}
		}
	}
}

// Reset all organisms to not moved
void World::resetOrganisms() {
	for (int i = 0; i < WORLDSIZE; i++)
	{
		for (int j = 0; j < WORLDSIZE; j++)
		{
			if (grid[i][j] != NULL)
			{
				grid[i][j]->setMoved(false);
			}
		}
	}
}

// Move all organisms of type aType
void World::moveOrganism(OrganismType aType) {
	for (int i = 0; i < WORLDSIZE; i++)
	{
		for (int j = 0; j < WORLDSIZE; j++)
		{
			if (grid[i][j] != NULL)
			{
				if (grid[i][j]->getType() == aType && !(grid[i][j]->hasMoved()))
				{
					grid[i][j]->move();
					// If TWINBUG is moving and they are touching, process explosion for this pair of TWINBUGS
					if (aType == TWINBUG && Twins_Touched)
					{
						TWINBUG_explosion();
					}
				}
			}
		}
	}
}

// Remove all dead organisms from this world.
void World::cleanup() {
	for (int i = 0; i < WORLDSIZE; i++) {
		for (int j = 0; j < WORLDSIZE; j++) {
			// Kill off any organisms that haven't eaten recently
			if ((grid[i][j] != NULL) && grid[i][j]->isDead()) {
				delete grid[i][j];
				grid[i][j] = NULL;
			}
		}
	}
}

// Make the organisms breed
void World::breedOrganisms() {
	for (int i = 0; i < WORLDSIZE; i++)
	{
		for (int j = 0; j < WORLDSIZE; j++)
		{
			if (grid[i][j] != NULL)
			{
				grid[i][j]->breed();
			}
		}
	}
}

// process TWINBUG touch explosion, world maintenance(deletes all bugs blast raidous), screen simulation
void World::TWINBUG_explosion()
{
	wstring my_str = L"..\\Explosion.wav";
	LPCWSTR wide_string; //define an array with size of my_str + 1
	wide_string = my_str.c_str();

	system("cls");
	displaytitle();
	cout << "                                                  By Philip Aldridge" << endl;
	myWorld.display(); // updates simulation explosion in the middle of one cycle of simulation
	PlaySound(wide_string, NULL, SND_FILENAME);

	for (int i = 0; i < 18; i++) // blast radious finds all cells impacted by hte blast and removies them from the grid
		if (Blast_Radius[i].x >= 0 && Blast_Radius[i].x < WORLDSIZE &&
			Blast_Radius[i].y >= 0 && Blast_Radius[i].y < WORLDSIZE)
			killAt(Blast_Radius[i].x, Blast_Radius[i].y);

	for (int i = 0; i < 18; i++) { Blast_Radius[i].x = -1; Blast_Radius[i].y = -1; }
	Twins_Touched = false; // true when two TWINBUGS touch
	Sleep(1000);
	PlaySound(NULL, 0, 0); // STOP ANY PLAYING SOUND
}
