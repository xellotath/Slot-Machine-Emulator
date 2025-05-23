#pragma once
#include <iostream>
#include <time.h>
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>

void _delay(unsigned int ms);

using namespace std;

class Reel {

public:

	Reel(int[]);

	void rspin();


	int* getvalue();

	int slot[9];

	//int spanslot[9];

	int temp;
	int temp2=-1;

	void reroll(int[]);

	
	int tester = 0;


};


//Create Reel
Reel::Reel(int weights[8]){



	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distrib(0, 100000);

	for (int i = 0; i < 9; i++) {
		
		temp = distrib(gen);
		


		temp2 = temp;

		for (int j = 0; j < 8; j++) {

			if (temp < weights[j]) {
				slot[i] = j + 1;
				break;
			} 
			if ((temp > weights[j]) && (j == 7)) {
				slot[i] = 9;
			}

		}

		

	}

	shuffle(begin(slot), end(slot), std::default_random_engine(temp));


}

void Reel::reroll(int weights[8]) {


	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distrib(0, 100000);


	for (int i = 0; i < 9; i++) {

		temp = distrib(gen);


		temp2 = temp;

		for (int j = 0; j < 8; j++) {

			if (temp < weights[j]) {
				slot[i] = j + 1;
				break;
			}
			if ((temp > weights[j]) && (j == 7)) {
				slot[i] = 9;
			}

		}


		
	}

	shuffle(begin(slot), end(slot), std::default_random_engine(temp));

}




//Create Reel
/*Reel::Reel() {

	srand((int)5);

	temp = (rand() % 9);

	for (int i = 0; i < 9; i++) {

		slot[i] = i + 1;

		spanslot[i] = i + 1;

	}

}*/

//"Spin" the reels by circling around the array.
void Reel::rspin() {

	int spins = (rand() % 50) + 10;

	for (int i = 0; i < spins; i++) {

		temp++;

		if (temp > 8) { 

			temp = 0; 

		}
	}
}

/*/"Spin" the reels by randomizing the array elements' order.
void Reel::rspin() {

	int spins = (rand() % 50) + 10;

	for (int i = 0; i < spins; i++) {

		temp++;

		if (temp > 8) {

			temp = 0;

		}
	}
}*/



//Get reel values and return them. Temp is middle row. If overshot, then circle around.
int* Reel::getvalue() {

	int a, b = temp, c;

	int* values = new int[3];
	//Middle Row
	values[1] = slot[b];

	if (b == 0) {

		a = 8;

	} else {

		a = b - 1;

	}
	//Top row
	values[0] = slot[a];

	if (b == 8) {

		c = 0;

	} else {

		c = b + 1;

	}
	//Bottom Row
	values[2] = slot[c];

	return values;
}


void _delay(unsigned int mseconds) //whenever this function is called it will cause a delay of "mseconds" miliseconds
{
	//clock_t time = mseconds + clock();	//time to campare against current time
	//while (time > clock());				//when current time is target time, stop
}