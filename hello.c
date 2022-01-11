#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

struct Game {
	int id;
	char fname[20];
	char lname[20];
};
void plane(int pos, int score) {
	//prints string that represents plane
	int i = 47-pos;
	char space[100] = "";
	while (pos > 0) {
		//append space pos times
		char c[] = " ";
		strcat(space, c);
		pos--;
	}
	printf("%s", space);
	printf("(|__.-. |");
	while (i > 0) {
		i--;
		printf(" ");
	}
	printf("Score:%d\n", score);
	printf("%s", space);
	printf("== ===_]+\n");
	printf("%s", space);
	printf("        |\n");
}

void air(int pos, int bomb, char* word) {

	char space[100] = "";
	int i = 0;
	while (pos > 1) {
		//append space pos times
		char c[] = " ";
		strcat(space, c);
		pos--;
	}
	while (bomb > 0) {
		//for the y-axis position of the bomb 
		bomb--;
		i++;
		printf("\n");
	}
	printf("%s<|>\n", space);
	printf(" %s|%s", space, word);
	printf(" %s*", space);
	i = 15 - i; //proper #of new lines
	while (i > 0) {
		i--;
		printf("\n");
	}
}
int isExplode(int y) {
	//check if bomb reach the city
	if (y >= 16) {
		system("cls");
		printf(" ______                    _____      _____            _____  ____\n");
		printf("|           /\\    |\\   /| |          |     | \\      / |      |    \\\n");
		printf("|   ___    /  \\   | \\_/ | |___       |     |  \\    /  |___   | ___|\n");
		printf("|  |   |  / -- \\  |     | |          |     |   \\  /   |      | \\\n");
		printf("|______| /      \\ |     | |_____     |_____|    \\/    |_____ |  \\_\n");
		return 1;
	}
	return 0;
}
void city(int pos) {
	//prints string that represents the city
	if (pos >= 15) { //bomb destroys city...
		printf("\n_________________________________________________________________________");
		Sleep(1000);
	}
	else {
		printf(" _____        _____    ________________ ________    _________\n|o o o|_______|    |__|              | | # # # |____|o o o o|\n|o o o|* * *|: ::|. . |              |o| # # # |. . |o o o o|\n|o o o|* * *|::  |. . |[]  []  []  []|o| # # # |. . |o o o o|\n|o o o|**** |:  :| . .|[]  []  []    |o| # # # |. . |o o o o|\n|_[]__|__[]_|_||_|__< |___________;;_|_|___[]__|_.|_|__[]___|\n");
	}
}

int rand_line(int start, int end) { //returns random int between [start,end]
	srand(time(NULL)); //change a different seed of random through srand
	return (rand() % (end - start + 1)) + start; 
}

char* codeword;
void rand_codeword(int length) {
	//returns random codeword from codewords.txt with length equals to parameter
	int start = 0;
	int end = 0;
	//here those if blocks are for set variables to specific line numbers. 
	if (length == 3) {
		start = 1;
		end = 4;
	}
	if (length == 4) {
		start = 5;
		end = 9;
	}
	if (length == 5) {
		start = 10;
		end = 14;
	}
	if (length == 6) {
		start = 15;
		end = 18;
	}
	if (length == 7) {
		start = 19;
		end = 24;
	}
	if (length == 8) {
		start = 25;
		end = 30;
	}
	if (length == 9) {
		start = 31;
		end = 35;
	}
	if (length == 10) {
		start = 36;
		end = 39;
	}
	FILE *fp = fopen("codewords.txt", "r"); //open file in reading mode
	static char line[20];
	int i = 1;
	int random = rand_line(start, end);
	while (fgets(line, sizeof(line), fp)) { //for each line
		if (i == random) { //then we reached the line that we randomly determined  
			codeword = line; //set codeword to this line 
			break;
		}
		i++;
	}
	fclose(fp); //close file
}
void print_menu() {
	printf("1. New Game\n2. Load a Saved Game\n3. Save Current Game\n4. Return to Game\n5. Exit\n");
}

int main() {
	int plane_pos; //horizontal position of the plane
	int bomb; //boolean value to check there exist at most one bomb.
	int bomb_pos_x; //vertical where the bomb is dropped
	int bomb_pos_y; //bomb position on y axis. (increase as bomb going down)
	int score;//total score
	int gain; //to calcualte points earned from each word 
	int word_length;
	int level;
	int div_5;
	int milisecond; //setting game speed
	char do_u_want = 'y';
	while (do_u_want == 'y') {
		system("cls");
		print_menu();
		int option;
		scanf("%d", &option);
		printf("\n");
		switch (option) {
		case 1: //New Game
			system("cls");
			//set variables for new game
			plane_pos = 0;
			bomb = 1;
			bomb_pos_x = 0;
			bomb_pos_y = 0;
			score = 0;
			gain = 0;
			word_length = 3;
			level = 3;
			div_5 = 0;
			milisecond = 100;
			rand_codeword(level);
			while (1) {

				if (isExplode(bomb_pos_y)) {
					do  {
						printf("\nDo you want to continue? y/n\n");
						scanf(" %c", &do_u_want);
					} while (!(do_u_want == 'y' || do_u_want == 'n'));
					break;
				}

				if (plane_pos == 47) { //if the plane came to the far right, go back to far left
					plane_pos = 0;
				}
				plane(plane_pos, score);
				if (bomb) { //check if new bomb required
					bomb = 0;
					bomb_pos_x = plane_pos; //drop from plane's position
					bomb_pos_y = 0; //drop from highest level
				}
				air(bomb_pos_x, bomb_pos_y, codeword);
				city(bomb_pos_y);
				while (_kbhit()) {
					char c = _getch();
					if (c == codeword[0]) {
						gain++;
						word_length--;
						memmove(codeword, codeword + 1, strlen(codeword));
						if (word_length == 0) {
							bomb = 1;
							div_5++;
							if (div_5 % 5 == 0) {
								level++;
								milisecond /= 5; //increase bomb drop speed by decreasing sleeping seconds 
								if (level == 11) {
									level = 10;
								}
							}
							word_length = level;
							rand_codeword(level);
							score += gain;
							gain = 0;
						}
					}
					else {
						gain--;
					}
				}
				Sleep(milisecond);
				system("cls");
				plane_pos++;
				bomb_pos_y++;
			}
			break;
		case 2: //Load Game
			printf("Value is 2");
			break;
		case 3: //Save Game
			printf("Value is 3");
			break;
		case 4: //Return to Game
			printf("Value is 4");
			break;
		case 5: //Exit
			printf("Value is 5");
			break;
		default:
			printf("%d is not a valid option. Try again by re-running the program\n", option);
			exit(0);
			break;
		}
	}
	system("cls");
	printf("Bye Bye!\n");
	return 0;
}