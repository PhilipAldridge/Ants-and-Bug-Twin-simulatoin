// ants_and_bugs CS M135 module 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>    // for time
#include "World.h"
#include <windows.h>
#include <conio.h>
#include <unordered_map>
using namespace std;
extern void displaytitle();
/*
define the new3 bug (class)
modify the "world" calss

*/
	World myWorld(time(0)); 

int main()
{

	myWorld.display();

	char ch;

	//while (cin.get(ch) && ch != 'q') {   // q for quit
	while (1){
		
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 'b' || ch == 'B')
			{
				// add BUG
				myWorld.createOrganisms(BUG, 1);
			}
			else if (ch == 'a' || ch == 'A')
			{
				// add Ant
				myWorld.createOrganisms(ANT, 1);
			}
			else if (ch == 't' || ch == 'T')
			{
				// add TWINBUG
				myWorld.createOrganisms(TWINBUG, 1); // twinbug
			}
			else if (ch == 'n')
			{
				 // reset everything
			}
		}

		system("cls");
		displaytitle();
		cout << "                                                  By Philip Aldridge" << endl;
		myWorld.display();
		Sleep(1000);
		myWorld.simulateOneStep();
	}

	return 0;
}

void displaytitle()
{
	std::unordered_map < int, const char* > char_map =
	{
		{
			int('A'),

			 R"(
__________               __________                                            
\______   \__ __  ____   \______   \_______  ____   ________________    _____  
 |    |  _/  |  \/ ___\   |     ___/\_  __ \/  _ \ / ___\_  __ \__  \  /     \ 
 |    |   \  |  / /_/  >  |    |     |  | \(  <_> ) /_/  >  | \// __ \|  Y Y  \
 |______  /____/\___  /   |____|     |__|   \____/\___  /|__|  (____  /__|_|  /
        \/     /_____/                           /_____/            \/      \/
                            )"
		},
	};
	cout << char_map['A'] << '\n';
}
