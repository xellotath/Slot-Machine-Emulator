#include <iostream>
#include <stdlib.h>
#include "slotinternals.h"
#include "sqlite3.h"
#include <Windows.h>
#include <conio.h>
#include "db.h"


using namespace std;



int main(int argc, char* argv[]) {



	//Create a database to store data
	sqlite3* db_fair;
	sqlite3* db_unfair;
	char* zErrMsg = 0;
	int rc_fair;
	int rc_unfair;
	int menu = 0;
	rc_fair = sqlite3_open("fair.db", &db_fair);
	char ch;

	if (rc_fair) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db_fair));
		return(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}
	sqlite3_close(db_fair);


	rc_unfair = sqlite3_open("unfair.db", &db_unfair);

	if (rc_unfair) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db_unfair));
		return(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}
	sqlite3_close(db_unfair);



	// Create our slot machine
	Slot* slotmachine = new Slot();
	int lines;


	printf("_______________\n"
		"|             |"
		"|  Main Menu  |\n"
		"|_____________|\n\n"
		"1.Play our unfair and rigged slot game\n"
		"2.Play the \"fair\" and not so much rigged game\n"
		"3.Play but don't actually play(Autoplay Mode)\n"
		"4.Exit\n\n"
		"Your Choice: ");



	for (;;) {

		if (menu == 0) {
			system("color 70");
			system("cls");
			printf("               _______________\n"
				"               |             |\n"
				"               |  Main Menu  |\n"
				"               |_____________|\n\n"
				"1.Play our unfair and rigged slot game\n"
				"2.Play the \"fair\" and not so much rigged game\n"
				"3.Play but don't actually play(Autoplay Mode)\n"
				"4.Exit\n\n"
				"Your Choice: ");


			cin >> menu;
		}


		else if (menu == 1) {
			slotmachine->insertmoney(0);

			int i = 0;
			lines = 5;

			for (;;) {

				for (;;) {

					_delay(500);

					if (_kbhit()) {

						// Stores the pressed key in ch
						ch = _getch();

						// Terminates the loop
						// when escape is pressed
						if (int(ch) == 27)
							break;

						cout << "\nKey pressed= " << ch;
					}


					cout << "How many credits would you like to bet? (100/200/300/400/500):" << endl;

					cin >> lines;

					if (slotmachine->credits >= lines) {

						if ((lines < 100) || (lines > 500) || ((lines % 100) != 0)) {

							if ((lines >= 1) && (lines <= 5)) {
								break;
							}


							cout << "You may only bet between 100 and 500 credits. Please try again." << endl;

							continue;

						}
						break;
					}
					else if (slotmachine->credits < lines) {

						cout << "You don't have enough credits! :(" << endl;

					}

				}




				if (slotmachine->credits > 0) {

					if ((lines >= 1) && (lines <= 5)) {
						slotmachine->betlines(lines*100);
					}
					else {

						slotmachine->betlines(lines);
					}

					slotmachine->spin();

				}


				if (slotmachine->credits == 0) {

					break;

				}


			}


		}

		else if (menu == 2) {
			slotmachine->insertmoney(2);

			int i = 0;
			lines = 5;

			for (;;) {

				for (;;) {

					_delay(500);

					if (_kbhit()) {

						// Stores the pressed key in ch
						ch = _getch();

						// Terminates the loop
						// when escape is pressed
						if (int(ch) == 27)
							break;

						cout << "\nKey pressed= " << ch;
					}


					cout << "How many credits would you like to bet? (100/200/300/400/500):" << endl;

					cin >> lines;

					if (slotmachine->credits >= lines) {

						if ((lines < 100) || (lines > 500) || ((lines % 100) != 0)) {

							if ((lines >= 1) && (lines <= 5)) {
								break;
							}


							cout << "You may only bet between 100 and 500 credits. Please try again." << endl;

							continue;

						}
						break;
					}
					else if (slotmachine->credits < lines) {

						cout << "You don't have enough credits! :(" << endl;

					}

				}




				if (slotmachine->credits > 0) {

					if ((lines >= 1) && (lines <= 5)) {
						slotmachine->betlines(lines * 100);
					}
					else {

						slotmachine->betlines(lines);
					}

					slotmachine->spin();

				}


				if (slotmachine->credits == 0) {

					break;

				}


			}


		}

		else if (menu == 3) {
			slotmachine->insertmoney(1);

			int i = 0;
			lines = 5;

			for (;;) {




				if (slotmachine->credits > 0) {



					slotmachine->betlines(300);

					slotmachine->spin();

				}


				if (slotmachine->credits == 0) {

					break;

				}


			}
		}

		else if (menu == 4) {
			printf("Thanks for Playing!");
			getchar();
			return 0;
		}
	}




}
