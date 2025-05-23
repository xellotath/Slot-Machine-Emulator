#pragma once
#include <iostream>
#include <fstream>
#include "reel.h"
#include "sqlite3.h"
#include <iomanip>
#include <time.h>
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>

void dbcollectrtp(float);
void dbcollectrounds(int);
void dbcollectwin(int);
void dbcollectweights(int, int);
void writedb(int);


int rounds;

int winnings;

float rtp;

int weights[9];





void writedb(int betlevel) {



}








void dbcollectrtp(float value) {
	rtp = value;
}

void dbcollectwin(int value) {
	winnings = value;
}

void dbcollectrounds(int value) {
	rounds = value;
}

void dbcollectweights(int value, int index) {
	weights[index] = value;
}