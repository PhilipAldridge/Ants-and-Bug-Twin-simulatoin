//
//  Bug.cpp
//  INHERITANCE_AND_POLYMORPHISM
//
//  Created by Kristjan Thorsteinsson on 01/04/14.
//  Copyright (c) 2014 Kristjan Thorsteinsson. All rights reserved.
//

#include "TWINBUG.h"
#include "Organism.h"
using namespace std;

Position Blast_Radius[18]; // blast radious finds all cells impacted by the blast and removies them from the grid
bool Twins_Touched = false; // determins if Two TWINBUGS touch
extern void set_blast_radius(int x, int y, int x1, int y1);  // blast radious finds all organisms impacted by the blast and removies them from the grid

Twinbug::Twinbug(World* aWorld, int xcoord, int ycoord) : Organism(aWorld, xcoord, ycoord)
{
	death_tik = 0;
}

void Twinbug::move()
{

	if (world->getAt(x, y + 1) != NULL && world->getAt(x, y + 1)->getType() == TWINBUG) // looking for 2 TWINBUGS in a vertical for horizontal position
	{
		set_blast_radius(x, y, x, y + 1); // saves cordinates of all affected organisms inorder to process explosion later
	}
	else
		if (world->getAt(x, y - 1) != NULL && world->getAt(x, y - 1)->getType() == TWINBUG)
		{
			set_blast_radius(x, y, x, y - 1);
		}
		else
			if (world->getAt(x - 1, y) != NULL && world->getAt(x - 1, y)->getType() == TWINBUG)
			{
				set_blast_radius(x, y, x - 1, y);

			}
			else
				if (world->getAt(x + 1, y) != NULL && world->getAt(x + 1, y)->getType() == TWINBUG)
				{
					set_blast_radius(x, y, x + 1, y);
				}
				else
				{// no TWINBUGS were found touching, perform normal ramdom move
				Move mover = world->randomMove(); // normal random move of all orgnaisms
				switch (mover) {
				case UP:
					if (world->getAt(x, y + 1) == NULL && in_range(x, y + 1))
					{
						movesTo(x, y + 1);
					}
					break;
				case DOWN:
					if (world->getAt(x, y - 1) == NULL && in_range(x, y - 1))
					{
						movesTo(x, y - 1);
					}
					break;
				case LEFT:
					if (world->getAt(x - 1, y) == NULL && in_range(x - 1, y))
					{
						movesTo(x - 1, y);
					}
					break;
				case RIGHT:
					if (world->getAt(x + 1, y) == NULL && in_range(x + 1, y))
					{
						movesTo(x + 1, y);
					}
					break;
				default:
					break;
				}
			}
}


void Twinbug::generateOffspring(int whereX, int whereY)
{
	//new Twinbug(this->world, whereX, whereY);
	//breedTicks = 0;
}


void Twinbug::breed()
{
	/*
	if (breedTicks >= BREED_BUGS)
	{
		breedAtAdjacentCell();
	}
	*/
	

}

bool Twinbug::isDead() const
{
	/*
	if (death_tik >= STARVE_BUGS)
	{
		return true;
	}
	else
	{
		return false;
	}
	*/
	return false;
}

OrganismType Twinbug::getType() const
{
	return TWINBUG; // Twinbug
}


char Twinbug::representation()const
{
	return 'T';
}

int Twinbug::size() const
{
	return 30;
}

bool Twinbug::in_range(int xx, int yy)
{
	return (xx >= 0) && (xx < WORLDSIZE) && (yy >= 0) && (yy < WORLDSIZE);
}

// blast radious finds all organisms impacted by the blast and removies them from the grid
void set_blast_radius(int x, int y, int x1, int y1)
{
	// saves all posable postions of 2 TWINBUGS touching
	Twins_Touched = true;
	Blast_Radius[0].x = x;
	Blast_Radius[0].y = y;

	Blast_Radius[1].x = x;
	Blast_Radius[1].y = y + 1;

	Blast_Radius[2].x = x;
	Blast_Radius[2].y = y - 1;

	Blast_Radius[3].x = x - 1;
	Blast_Radius[3].y = y;

	Blast_Radius[4].x = x + 1;
	Blast_Radius[4].y = y;

	Blast_Radius[5].x = x + 1;
	Blast_Radius[5].y = y + 1;

	Blast_Radius[6].x = x + 1;
	Blast_Radius[6].y = y - 1;

	Blast_Radius[7].x = x - 1;
	Blast_Radius[7].y = y + 1;

	Blast_Radius[8].x = x - 1;
	Blast_Radius[8].y = y - 1;

	Blast_Radius[9].x = x1;
	Blast_Radius[9].y = y1;

	Blast_Radius[10].x = x1;
	Blast_Radius[10].y = y1 + 1;

	Blast_Radius[11].x = x1;
	Blast_Radius[11].y = y1 - 1;

	Blast_Radius[12].x = x1 - 1;
	Blast_Radius[12].y = y1;

	Blast_Radius[13].x = x1 + 1;
	Blast_Radius[13].y = y1;

	Blast_Radius[14].x = x1 + 1;
	Blast_Radius[14].y = y1 + 1;

	Blast_Radius[15].x = x1 + 1;
	Blast_Radius[15].y = y1 - 1;

	Blast_Radius[16].x = x1 - 1;
	Blast_Radius[16].y = y1 + 1;

	Blast_Radius[17].x = x1 - 1;
	Blast_Radius[17].y = y1 - 1;
}
