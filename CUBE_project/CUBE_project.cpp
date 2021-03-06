﻿#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include <stack>
#include <iostream>

#pragma warning (disable : 4996)

//ASKII CODE OF KEYS
#define MAP_ADJ_X 0
#define MAP_ADJ_Y 3

#define RED 4		
#define GREEN 2
#define YELLOW 14
#define BLUE 1
#define PINK 13 //instead of orange
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
#define M 109 //mix cube

using namespace std;

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
int keyArray[12] = { E,R,A,S,D,F,G,H,T,Y,C,V };
int key;
int speed = 100; 
bool autoActive = false;
bool mixActive = false;
bool devtoolOn = false;
stack<int> keystack; //for autosolve
stack<int> mixstack; //for mix

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
void move(int n,int m); //turn cube
void status();
void mix();

//main start
int main() {
	int i;
	reset();
	cursor(0);

	while (1) {
		drawcube();
		status();
		Sleep(speed);
		if (autoActive) { // active auto solve
			key = keystack.top();
			keystack.pop();
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
			if (keystack.empty()) autoActive = false;
		} else if (mixActive) { //active auto mix
			key = mixstack.top();
			keystack.push(mixstack.top());
			mixstack.pop();
			if (mixstack.empty()) mixActive = false;
		} /*else if (devtoolOn){
			
		}*/ else { //get key
			key = _getch();
			keystack.push(key);
		}

		switch (key) {
		case ESC:exit(0); //exit
		case P:  reset(); //set color
			break;
		case K: //autosolve
			if(!keystack.empty()) autoActive = true; //can't use autosolve at first
			break;
		case M:
			mix();
			break;

		case E:move(0, 1);break; //0
		case R:move(0, 3);break;

		case A:move(1, 1);break; //1
		case S:move(1, 3);break;

		case D:move(2, 1);break; //2
		case F:move(2, 3);break;

		case G:move(3, 1);break; //3
		case H:move(3, 3);break;

		case T:move(4, 1);break; //4
		case Y:move(4, 3);break;

		case C:move(5, 1);break; //5
		case V:move(5, 3);break;
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
	while (!keystack.empty()) keystack.pop();
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 9; j++) {
			piece[i][j] = colors[i];
		}
	}
}


void drawcube(void) { //*just skip this function LOL
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

	gotoxy(1, 1-MAP_ADJ_Y, "□");
	gotoxy(42, 1 - MAP_ADJ_Y, "□");
	gotoxy(1, 23 - MAP_ADJ_Y, "□");
	gotoxy(42, 23 - MAP_ADJ_Y, "□");
}

int tc(int n) { //textcolor set
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, n);
	return n;
}

void three(int n, int m){ //only for drawcube();
	int i;
	for (i = 0; i < 3; i++) {
		tc(piece[n][i+m]);
		printf(" ㅇ "); 
	}
	tc(WHITE);
}

void move(int n,int m) { // 1-clockwise / 3-clock
	int i;
	for (i = 0; i < m; i++) {
		sidechange(n); 
		edgechange(n);
	}
}

void sidechange(int n) {
	int imsi;
	imsi = piece[n][2]; piece[n][2] = piece[n][8]; piece[n][8] = piece[n][6]; piece[n][6] = piece[n][0]; piece[n][0] = imsi;
	imsi = piece[n][5]; piece[n][5] = piece[n][7]; piece[n][7] = piece[n][3]; piece[n][3] = piece[n][1]; piece[n][1] = imsi;
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
	gotoxy(30, 3, "Mix Cube : M");
	gotoxy(30, 4, "Auto solve : K");
	gotoxy(30, 5, "Reset : P");



	gotoxy(30, 9, "keystack = ");
	if (!keystack.empty()) printf("%2d ,%3d", keystack.size(), keystack.top());
	else printf(" 0 ,  0");
	gotoxy(30, 10, "autoActive = ");
	printf("%1d", autoActive);
	gotoxy(30, 11, "mixActive =");
	printf("%1d", mixActive);

}

void mix() {
	int i; 
	mixActive = true;
	keystack.pop();
	srand(time(NULL));
	int random = rand() % 10 + 15;
	for (i = 0; i < random; i++) {
		mixstack.push(keyArray[rand() % 12]);
	}
}

