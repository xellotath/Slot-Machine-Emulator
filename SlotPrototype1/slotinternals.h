#pragma once
#include <iostream>
#include <fstream>
#include "reel.h"
#include "sqlite3.h"
#include <iomanip>
#include "db.h""

#define MAX_INT 236870911




//TODO: modify values of winnings
//		modify weights
//		modify rpt function
//		


// Class declaration for our slot Machine.
class Slot {

public:
	
	//Functions
	Slot();

	int insertmoney(int);

	void betlines(int);
	
	void spin();

	void display();

	char numbers[10][4][4] = {

	{{"   "},
	 {"  |"},
	 {"  |"},},

	{{"__ "},
	 {" _|"},
	 {"|__"}},

	{{"__ "},
	 {"__|"},
	 {"__|"}},

	{{"   "},
	 {"|_|"},
	 {"  |"}},

	{{" __"},
	 {"|_ "},
	 {"__|"}},

	{{" _ "},
	 {"|_ "},
	 {"|_|"}},

	{{" _ "},
	 {"| |"},
	 {"  |"}},

	{{" _ "},
	 {"|_|"},
	 {"|_|"}},

	{{" _ "},
	 {"|_|"},
	 {" _|"}},
	};
	
	//Enum - Reel Values
	enum Results {
		derp = 1, 
		derpina = 2, 
		FFFFFFFUUUUUUUUUUUU = 3, 
		foreveralone = 4, 
		ilied = 5, 
		trollface = 6, 
		trolldad = 7, 
		megusta = 8, 
		okayguy = 9
	};
	
	//Variables
	
	int credits;

	int autoplay;

	//Reel Object
	Reel* reels[5];



	//Functions
	void load(int, int*);

	int checkwinnings();

	int checkline(int line[5]);

	void RTP(int, int);

	

	//Variables
	int screen[3][5];
	int betAmount;
	int weights[6][8] = { { 27500 ,45000 ,55000 ,64500 ,73000 ,82000 ,90000 ,95000 } ,
		{ 27500 ,45000 ,55000 ,64500 ,73000 ,82000 ,90000 ,95000 } ,
		{ 27500 ,45000 ,55000 ,64500 ,73000 ,82000 ,90000 ,95000 } ,
		{ 27500 ,45000 ,55000 ,64500 ,73000 ,82000 ,90000 ,95000 } ,
		{ 27500 ,45000 ,55000 ,64500 ,73000 ,82000 ,90000 ,95000 } ,
		{ 27500 ,45000 ,55000 ,64500 ,73000 ,82000 ,90000 ,95000 } };
	ofstream myfile;

	int jackpotflag = 0;

	int isfair;

	int rounds[5] = { 1,1,1,1,1 };

};


//Create the Slot Machine
Slot::Slot() {

	//Set credits to 0 on creation
	credits = 0;

	//Set bet amount to 0 on creation
	betAmount = 0;


	reels[0] = new Reel(weights[5]);
	reels[1] = new Reel(weights[5]);
	reels[2] = new Reel(weights[5]);
	reels[3] = new Reel(weights[5]);
	reels[4] = new Reel(weights[5]);

	//Initialize the screen 2D array
	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 5; j++) {

			screen[i][j] = 0;

		}

	}

}



//The main game mechanic at this point.
//"Spins" the "Reels" and loads the results on our 2D array "screen"
void Slot::spin() {

	if (betAmount > 0) {

		reels[0]->reroll(weights[(betAmount / 100) - 1]);
		reels[1]->reroll(weights[(betAmount / 100) - 1]);
		reels[2]->reroll(weights[(betAmount / 100) - 1]);
		reels[3]->reroll(weights[(betAmount / 100) - 1]);
		reels[4]->reroll(weights[(betAmount / 100) - 1]);



		//Calls rspin in reel.h
		reels[0]->rspin();
		reels[1]->rspin();
		reels[2]->rspin();
		reels[3]->rspin();
		reels[4]->rspin();

		//Gets results from reels' spin and then loads it for printing.
		int* c1 = reels[0]->getvalue();
		int* c2 = reels[1]->getvalue();
		int* c3 = reels[2]->getvalue();
		int* c4 = reels[3]->getvalue();
		int* c5 = reels[4]->getvalue();

		load(0, c1);
		load(1, c2);
		load(2, c3);
		load(3, c4);
		load(4, c5);

		//Prints all reel values on our screen
		display();

		//Did we win?
		
		RTP(checkwinnings(), betAmount);

		betAmount = 0;


		delete[] c1;
		delete[] c2;
		delete[] c3;
		delete[] c4;
		delete[] c5;

		c1 = c2 = c3 = c4 = c5 = NULL;

	}

	else { 

		cout << "Can't spin without your money." << endl; 

	}



	rounds[(betAmount/100)-1]++;



	writedb(betAmount/100);

}


void Slot::RTP(int win, int bet) {
	float rtp = (float)win / (float)bet;

	dbcollectrtp(rtp);

	myfile.open("zrtp.txt", std::ios_base::app);
	myfile << std::fixed << std::setprecision(8) << rtp << endl;
	myfile.close();

	
	if (isfair == 0) {
		if ((float) rtp > (float) 0.85 * bet) {
			myfile.open("zweights.txt", std::ios_base::app);
			for (int i = 0; i < 8; i++) {
				weights[(bet / 100) - 1][i] += 5;

				myfile << weights[(bet / 100) - 1][i] << " ";

				dbcollectweights(weights[(bet / 100) - 1][i], i);

			}
			myfile << endl;
			myfile.close();
		}
		else if ((float)rtp < (float)0.85*bet) {
			myfile.open("zweights.txt", std::ios_base::app);
			for (int i = 0; i < 8; i++) {
				weights[(bet / 100) - 1][i] -= 5;
				myfile << weights[(bet / 100) - 1][i] << " ";
				dbcollectweights(weights[(bet / 100) - 1][i], i);

			}
			myfile << endl;
			myfile.close();
		}
	}
	else if (isfair == 1) {
		if (rtp > bet) {
			myfile.open("zweights.txt", std::ios_base::app);
			for (int i = 0; i < 8; i++) {
				weights[(bet / 100) - 1][i] += 5;

				myfile << weights[(bet / 100) - 1][i] << " ";
				dbcollectweights(weights[(bet / 100) - 1][i], i);

			}
			myfile << endl;
			myfile.close();
		}
		else if (rtp < bet) {
			myfile.open("zweights.txt", std::ios_base::app);
			for (int i = 0; i < 8; i++) {
				weights[(bet / 100) - 1][i] -= 5;
				myfile << weights[(bet / 100) - 1][i] << " ";
				dbcollectweights(weights[(bet / 100) - 1][i], i);

			}
			myfile << endl;
			myfile.close();
		}
	}
}

//Loads the reel column to be correctly printed on our screen
void Slot::load(int col, int* reelcolumn) {

	for (int i = 0; i < 3; i++) {

		screen[i][col] = reelcolumn[i];

	}

}


//Beautifully prints all reel values on our screen
void Slot::display() {


	if (autoplay == 0) {
		cout << "___________________________________________" << endl;
		cout << "|  " << "_____" << "   " << "_____" << "   " << "_____" << "   " << "_____" << "   " << "_____" << "  |" << endl;
		//cout << "| | " << "    " << "| | " << "    " << "| | " << "    " << "| | " << "    " << "| | " << "    " << "| |" << endl;

		for (int i = 0; i < 3; i++) {

			for (int k = 0; k < 3; k++) {

				for (int j = 0; j < 5; j++) {


					cout << "| | " << numbers[screen[i][j] - 1][k] << " ";
				}
				cout << "| | " << endl;
			}



			cout << "| |" << "_____" << "| |" << "_____" << "| |" << "_____" << "| |" << "_____" << "| |" << "_____" << "| |" << endl;
			
		}
		cout << "|_________________________________________|" << endl;
	}	
	
	else if (autoplay == 1) {
		myfile.open("zslot.txt", std::ios_base::app);

		myfile << "   " << "_____" << "   " << "_____" << "   " << "_____" << "   " << "_____" << "   " << "_____" << "  " << endl;
		//cout << "| | " << "    " << "| | " << "    " << "| | " << "    " << "| | " << "    " << "| | " << "    " << "| |" << endl;

		for (int i = 0; i < 3; i++) {

			for (int k = 0; k < 3; k++) {

				for (int j = 0; j < 5; j++) {


					myfile << "| | " << numbers[screen[i][j] - 1][k] << " ";
				}
				myfile << "| | " << endl;
			}



			myfile << "| |" << "_____" << "| |" << "_____" << "| |" << "_____" << "| |" << "_____" << "| |" << "_____" << "| |" << endl;

		}
		myfile << endl;
		myfile.close();
	}
}


//Betlines function takes the number of lines the user wants to bet on. Currently equivalent to a credit
void Slot::betlines(int lines) {

		betAmount = lines;
		credits -= betAmount;

}





//Adds money and converts it to credits.
int Slot::insertmoney(int mode) {

	if (mode == 1) {

		autoplay = 1;

		int tempcredits = (int)(MAX_INT * 4);

		credits += tempcredits;

		cout << "You now have " << credits << " credits." << endl;

		getchar();

		return credits;

		dbcollectrounds(rounds[betAmount/100]);

	}

	if (mode == 0) {
		autoplay = 0;
		isfair = 0;
	}

	if (mode == 2) {
		isfair = 1;
		autoplay = 0;
	}

	double moneyvalue;

	cout << "Enter the amount of money to be deposited in the slot machine: ";

	cin >> moneyvalue;


	for (;;) {
		if (moneyvalue >= 1.00) {


			int tempcredits = (int)(moneyvalue * 100);

			credits += tempcredits;

			cout << "You now have " << credits << " credits." << endl;

			return credits;

		}
		else if ((moneyvalue > 0.00) && (moneyvalue < 1.00)) {

			cout << "I know this is a virtual slot machine but let's keep it classy." << endl;
			cin >> moneyvalue;

		}
		else if (moneyvalue == 0.00) {

			cout << "Entering 0 dollars is very meaningful." << endl;

			cin >> moneyvalue;

		}
		else {

			cout << "Cool so you don't want to play." << endl;
			cin >> moneyvalue;

		}
	}
}



//Payouts: 
int Slot::checkline(int line[5]) {


	//HARD VICTORIES

	if (line[0] == this->okayguy
		&& line[1] == this->okayguy
		&& line[2] == this->okayguy
		&& line[3] == this->okayguy
		&& line[4] == this->okayguy) {

		system("color 47");

		for (int q = 0; q < 10; q++) {
			cout << endl << "JACKPPOOOOOOOTOOTOTOOTOOTO" << endl;

			_delay(500);
		}
		return 10000;

	}

	if (line[0] == this->megusta
		&& line[1] == this->megusta
		&& line[2] == this->megusta
		&& line[3] == this->megusta
		&& line[4] == this->megusta) {

		return 5000;

	}



	if (line[0] == this->trolldad
		&& line[1] == this->trolldad
		&& line[2] == this->trolldad
		&& line[3] == this->trolldad
		&& line[4] == this->trolldad) {

		return 2000;

	}

	if (line[0] == this->trollface
		&& line[1] == this->trollface
			&& line[2] == this->trollface
		&& line[3] == this->trollface
			&& line[4] == this->trollface) {

		return 99;

	}

	if (line[0] == this->ilied
		&& line[1] == this->ilied
		&& line[2] == this->ilied
		&& line[3] == this->ilied
		&& line[4] == this->ilied) {

		return 80;

	}

	if (line[0] == this->foreveralone
		&& line[1] == this->foreveralone
		&& line[2] == this->foreveralone
		&& line[3] == this->foreveralone
		&& line[4] == this->foreveralone) {

		return 69;

	}

	if		(line[0] == this->FFFFFFFUUUUUUUUUUUU 
		&& line[1] == this->FFFFFFFUUUUUUUUUUUU 
		&& line[2] == this->FFFFFFFUUUUUUUUUUUU
		&& line[3] == this->FFFFFFFUUUUUUUUUUUU
		&& line[4] == this->FFFFFFFUUUUUUUUUUUU) {

		return 50; 

	}

	if		(line[0] == this->derpina 
		&& line[1] == this->derpina 
		&& line[2] == this->derpina
		&& line[3] == this->derpina
		&& line[4] == this->derpina) {

		return 35; 

	}

	if (line[0] == this->derp
		&& line[1] == this->derp
		&& line[2] == this->derp
		&& line[3] == this->derp
		&& line[4] == this->derp) {

		return 30;

	}



	//SOFT VICTORIES


	if (line[0] == this->okayguy
		&& (line[1] == this->okayguy
			|| line[2] == this->okayguy)
		&& (line[3] == this->okayguy
			|| line[4] == this->okayguy)) {

		return 100;

	}


	if (line[0] == this->megusta
		&& (line[1] == this->megusta
			|| line[2] == this->megusta)
		&& (line[3] == this->megusta
			|| line[4] == this->megusta)) {

		return 100;

	}

	if (line[0] == this->trolldad
		&& (line[1] == this->trolldad
			|| line[2] == this->trolldad)
		&& (line[3] == this->trolldad
			|| line[4] == this->trolldad)) {

		return 100;

	}

	if (line[0] == this->trollface
		&& (line[1] == this->trollface
			|| line[2] == this->trollface)
		&& (line[3] == this->trollface
			|| line[4] == this->trollface)) {

		return 20;

	}
	if (line[0] == this->ilied
		&& (line[1] == this->ilied
			|| line[2] == this->ilied)
		&& (line[3] == this->ilied
			|| line[4] == this->ilied)) {

		return 15;

	}

	if (line[0] == this->foreveralone
		&& (line[1] == this->foreveralone
			|| line[2] == this->foreveralone)
		&& (line[3] == this->foreveralone
			|| line[4] == this->foreveralone)) {

		return 10;

	}

	if		(line[0] == this->FFFFFFFUUUUUUUUUUUU 
		&&( line[1] == this->FFFFFFFUUUUUUUUUUUU
		|| line[2] == this->FFFFFFFUUUUUUUUUUUU)
		&& (line[3] == this->FFFFFFFUUUUUUUUUUUU
		|| line[4] == this->FFFFFFFUUUUUUUUUUUU)) {

		return 10; 

	}

	if (line[0] == this->derpina
		&& (line[1] == this->derpina
			|| line[2] == this->derpina)
		&& (line[3] == this->derpina
			|| line[4] == this->derpina)) {

		return 5;

	}


	if (line[0] == this->derp
		&& (line[1] == this->derp
			|| line[2] == this->derp)
		&& (line[3] == this->derp
			|| line[4] == this->derp)) {

		return 10;

	}





	return 0;
}


//Checks winnings against the line results, for the lines that were played
int Slot::checkwinnings() {

	int val[5];

	int winnings = 0;

	int tempwinnings = 0;

	// Check middle line


		for (int i = 0; i < 5; i++) {

			val[i] = screen[1][i];

		}

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 1 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;



	// Check top line


		for (int i = 0; i < 5; i++) {

			val[i] = screen[0][i];

		}

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 2 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;


	// Check bottom line


		for (int i = 0; i < 5; i++) {

			val[i] = screen[2][i];

		}

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 3 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;


	// Check main diagonal


		for (int i = 0; i < 3; i++) {

			val[i] = screen[i][i];

		}

		val[3] = screen[2][3];
		val[4] = screen[2][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 4 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;


	// Check secondary diagonal



		val[0] = screen[2][0];
		val[1] = screen[1][1];
		val[2] = screen[0][2];
		val[3] = screen[0][3];
		val[4] = screen[0][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 5 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;	




	//diagram line 4
	if (betAmount < 500) {


		val[0] = screen[0][0];
		val[1] = screen[1][1];
		val[2] = screen[2][2];
		val[3] = screen[1][3];
		val[4] = screen[0][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 6 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;




		//diagram line 5

		val[0] = screen[2][0];
		val[1] = screen[1][1];
		val[2] = screen[0][2];
		val[3] = screen[1][3];
		val[4] = screen[2][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 7 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;





		//diagram line 6

		val[0] = screen[0][0];
		val[1] = screen[0][1];
		val[2] = screen[1][2];
		val[3] = screen[2][3];
		val[4] = screen[2][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 8 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;


	}

	//diagram line 7
	if (betAmount < 400) {


		val[0] = screen[2][0];
		val[1] = screen[2][1];
		val[2] = screen[1][2];
		val[3] = screen[0][3];
		val[4] = screen[0][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 9 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;



		//diagram line 8

		val[0] = screen[1][0];
		val[1] = screen[0][1];
		val[2] = screen[1][2];
		val[3] = screen[2][3];
		val[4] = screen[1][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 10 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;


	}

	//diagram line 9
	if (betAmount < 300) {


		val[0] = screen[1][0];
		val[1] = screen[2][1];
		val[2] = screen[1][2];
		val[3] = screen[0][3];
		val[4] = screen[1][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 11 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;



		//diagram line 10

		val[0] = screen[0][0];
		val[1] = screen[1][1];
		val[2] = screen[1][2];
		val[3] = screen[1][3];
		val[4] = screen[2][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 12 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;



		//diagram line 11

		val[0] = screen[2][0];
		val[1] = screen[1][1];
		val[2] = screen[1][2];
		val[3] = screen[1][3];
		val[4] = screen[0][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 13 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;


	}

	//diagram line 12
	if (betAmount < 200) {


		val[0] = screen[1][0];
		val[1] = screen[0][1];
		val[2] = screen[0][2];
		val[3] = screen[1][3];
		val[4] = screen[2][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 14 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;


		//diagram line 13

		val[0] = screen[1][0];
		val[1] = screen[2][1];
		val[2] = screen[2][2];
		val[3] = screen[1][3];
		val[4] = screen[0][4];

		tempwinnings = checkline(val);

		if (tempwinnings > 0) {

			cout << "Line 15 wins " << tempwinnings << " credits!" << endl;
			_delay(150);
		}

		winnings += tempwinnings;


	}

	if (winnings > 0) {

		cout << "You won a total of " << (betAmount / 100) * winnings << " credits!" << endl;

		credits += (betAmount / 100) * winnings;




	}
	else {

		cout << "Thank you dear customer!" << endl;

	}

	cout << "You have " << credits << " credits left." << endl << endl;

	dbcollectwin((betAmount / 100)* winnings);

	myfile.open("zwin.txt", std::ios_base::app);
	myfile << (betAmount / 100)*winnings << endl;
	myfile.close();

	return winnings;

}


// rounds, winnings, rtp, weights