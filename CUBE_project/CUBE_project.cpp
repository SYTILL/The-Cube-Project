// HEAD

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>

#pragma warning (disable : 4996)

//ASKII CODE OF KEYS
#define MAP_ADJ_X 0
#define MAP_ADJ_Y 3

#define RED 4		
#define GREEN 2
#define YELLOW 14
#define BLUE 1
#define PINK 5 //instead of orange
#define WHITE 15

#define ESC 27
#define E 101 //1
#define R 114
#define A 97  //2
#define S 115
#define D 100 //3 
#define F 102
#define G 103 //4
#define H 104
#define T 116 //5
#define Y 121
#define C 99  //6
#define V 118
#define P 112 //reset
#define K 107 //auto solve


int colors[6] = { WHITE, PINK, GREEN, RED, BLUE, YELLOW };
int piece[6][9];
int edgeside[6][4] = { //u dont need to understand
	{4,3,2,1},
	{0,2,5,4},
	{0,3,5,1},
	{0,4,5,2},
	{0,1,5,3},
	{2,3,4,1} };
int edgenum[5][3] = { //u dont need to understand
	{0,1,2},
	{2,1,0},
	{0,3,6},
	{8,5,2},
	{6,7,8} };
int sidenum[6][4] = { //u dont need to understand
	{ 0,0,0,0 },
	{ 2,2,2,3 },
	{ 4,2,1,3 },
	{ 3,2,3,3 },
	{ 1,2,4,3 },
	{ 4,4,4,4 } };
int key,num;
int keystack[100]; 
int speed = 100; //kind of FPS
bool autoActive = false;


void gotoxy(double x, double y, const char * s) {
	COORD pos = { 2.0 * (x+MAP_ADJ_X), (y+MAP_ADJ_Y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
}

void drawcube(); //draw cube
void cursor(int n);
int tc(int n); //change text color
void reset(); // reset
void three(int n, int m); //textcolor,printf)x3 times
void sidechange(int n); //turn side clock dir
void edgechange(int n); //turn edge clock dir
void reverse(int n); //turn side clock-wise
void autosolve();
void status();

//main start
int main() {
	reset();
	cursor(0);

	while (1) {
		drawcube();
		status();
		Sleep(speed);
		if (autoActive) {
			key = keystack[--num];
			switch (key) {
			case E:key = R;break;//0
			case R:key = E;break;
			case A:key = S;break;//1
			case S:key = A;break;
			case D:key = F;break;//2
			case F:key = D;break;
			case G:key = H;break;//3
			case H:key = G;break;
			case T:key = Y;break;//4
			case Y:key = T;break;
			case C:key = V;break;//5
			case V:key = C;break;
			}
			if (num == 0) autoActive = false;
		} else {
			key = _getch();
			keystack[num++] = key;
		}

		switch (key) {
		case ESC: //exit
			exit(0);
		case P: //reset color
			reset();
			break;
		case K: //autosolve
			if(num != 1) autoActive = true;
			num--;
			break;

		case E:		// 0
			sidechange(0);
			edgechange(0);
			break;
		case R:
			reverse(0);
			break;

		case A:		//1
			sidechange(1);
			edgechange(1);
			break;
		case S:
			reverse(1);
			break;

		case D:		//2
			sidechange(2);
			edgechange(2);
			break;
		case F:
			reverse(2);
			break;

		case G:		//3
			sidechange(3);
			edgechange(3);
			break;
		case H:
			reverse(3);
			break;

		case T:		//4
			sidechange(4);
			edgechange(4);
			break;
		case Y:
			reverse(4);
			break;

		case C:		//5
			sidechange(5);
			edgechange(5);
			break;
		case V:
			reverse(5);
			break;
		default:
			num--;
		}
		key = 0; 
		while (_kbhit())_getch();
	}

	return 0;
}

void cursor(int n) {
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void reset() {
	int i, j;
	num = 0;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 9; j++) {
			piece[i][j] = colors[i];
		}
	}
}


void drawcube(void) {
	tc(WHITE); gotoxy(0, 0, "");
	printf("	                       ---  --  --- ---  --  ---\n");
	printf(" 	                     /"); three(0, 0); printf("/|"); three(4, 0); printf("| \n\n");

	printf("	                  /"); three(0, 3); printf("/"); tc(piece[3][2]); printf(" 0 "); tc(WHITE); printf("|"); three(4, 3); printf("| \n\n");

	printf("	               /"); three(0, 6); printf("/"); tc(piece[3][1]); printf(" 0 "); tc(piece[3][5]); printf(" 0 "); tc(WHITE); printf("|"); three(4, 6); printf("| \n");
	printf("	 ---  --  --- ---  --  ---          ---  --  ---\n");
	printf("	|"); three(1, 0); printf("|"); three(2, 0); printf("|"); tc(piece[3][0]); printf("0 "); tc(piece[3][4]); printf(" 0 "); tc(piece[3][8]); printf(" 0"); tc(WHITE); printf("/ \n\n");

	printf("	|"); three(1, 3); printf("|"); three(2, 3); printf("|"); tc(piece[3][3]); printf("0 "); tc(piece[3][7]); printf(" 0"); tc(WHITE); printf("/ \n\n");

	printf("	|"); three(1, 6); printf("|"); three(2, 6); printf("|"); tc(piece[3][6]); printf("0"); tc(WHITE); printf(" / \n");
	printf("	 ---  --  --- ---  --  ---  \n");
	printf("	             |"); three(5, 0); printf("|\n\n");

	printf("	             |"); three(5, 3); printf("|\n\n");

	printf("	             |"); three(5, 6); printf("|\n");
	printf("	              ---  --  ---\n");
	tc(WHITE); gotoxy(16, 3, "ER");
	tc(PINK); gotoxy(7, 9, "AS");
	tc(RED); gotoxy(19, 7, "GH");
	tc(GREEN); gotoxy(13.5, 9, "DF");
	tc(BLUE); gotoxy(24.5, 3, "TY");
	tc(YELLOW); gotoxy(13.5, 15, "CV");

	gotoxy(1, 1-MAP_ADJ_Y, "ㅁ");
	gotoxy(50, 1 - MAP_ADJ_Y, "ㅁ");
	gotoxy(1, 23 - MAP_ADJ_Y, "ㅁ");
	gotoxy(50, 23 - MAP_ADJ_Y, "ㅁ");
}

int tc(int n) {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, n);
	return n;
}

void three(int n, int m){
	int i;
	for (i = 0; i < 3; i++) {
		tc(piece[n][i+m]);
		printf(" ㅇ ");
	}
	tc(WHITE);
}

void sidechange(int n) {
	int imsi;
	imsi = piece[n][2]; piece[n][2] = piece[n][8]; piece[n][8] = piece[n][6]; piece[n][6] = piece[n][0]; piece[n][0] = imsi;
	imsi = piece[n][5]; piece[n][5] = piece[n][7]; piece[n][7] = piece[n][3]; piece[n][3] = piece[n][1]; piece[n][1] = imsi;
}

void reverse(int n) {
	int i;
	for (i = 0; i < 3; i++) {
		sidechange(n); 
		edgechange(n);
	}
}

void edgechange(int n) {
	int i, j, imsi;
	for (j = 0; j < 3; j++) {
		imsi = piece[edgeside[n][0]][edgenum[sidenum[n][0]][j]];
		for (i = 0; i < 3; i++) {
			piece[edgeside[n][i]][edgenum[sidenum[n][i]][j]] = piece[edgeside[n][i+1]][edgenum[sidenum[n][i+1]][j]];
		}
		piece[edgeside[n][3]][edgenum[sidenum[n][3]][j]] = imsi;
	}
}

void status() {
	gotoxy(30, 3, "Auto solve : K");
	gotoxy(30, 4, "Reset : P");

	gotoxy(30, 6, "keystack = ");
	printf("%2d, %3d", num, keystack[num-1]);
	gotoxy(30, 7, "autoActive = ");
	printf("%1d", autoActive);
}



