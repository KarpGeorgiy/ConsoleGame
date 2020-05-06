#include <iostream>
#include <conio.h>
#include <windows.h>

#define fieldHeight 40
#define fieldWidth 80

using namespace std;

int score = 0;
int difficulty = 0;
bool gameIsOver = false;
char field[fieldHeight][fieldWidth];
int playerPosition = 9;
int boardLength = 6, boardLevel = fieldHeight-4;
char command;
int HorizontalDirection = 1, VerticalDirection = 1;
int Xposition = 5, Yposition = 5;

void CreateField();
void Draw();
void PlayerMove(char command);
void BallMove();
void BallLogic();

int main()
{
	HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD crd = { fieldWidth, fieldHeight+3 };
	SMALL_RECT src = { 0, 0, crd.X - 1, crd.Y - 1 };
	SetConsoleWindowInfo(out_handle, true, &src);
	SetConsoleScreenBufferSize(out_handle, crd);
	CONSOLE_CURSOR_INFO curs;
	curs.dwSize = sizeof(curs);
	curs.bVisible = FALSE;
	::SetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &curs);
	COORD start;
	start.X = 0;
	start.Y = 0;
	system("pause");
	while (!gameIsOver)
	{
		Sleep(difficulty);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), start);
		CreateField();
		Draw();
		if (_kbhit()) command = _getch();
		PlayerMove(command);
		//command = ' ';
		BallMove();
	}
}

void CreateField()
{
	for (int i = 0; i < fieldHeight; i++)
	{
		for (int j = 0; j < fieldWidth; j++)
		{
			field[i][j] = ' ';
		}
	}

	for (int i = playerPosition; i <= playerPosition + boardLength - 1; i++)	field[boardLevel][i] = '=';
	field[Yposition][Xposition] = '+';
}

void Draw()
{
	for (int i = 0; i < fieldWidth; i++) cout << "_";
	cout << endl;
	for (int i = 0; i < fieldHeight; i++)
	{
		for (int j = 0; j < fieldWidth; j++)
		{
			cout << field[i][j];
		}
		cout << endl;
	}
	for (int i = 0; i < fieldWidth; i++) cout << "^";
	cout << endl;
	cout << "Score: " << score;
}

void PlayerMove(char command)
{
	switch (command)
	{
	case 'z':
		if (!(playerPosition == 0)) playerPosition--;
		for (int i = playerPosition; i <= playerPosition + boardLength - 1; i++) field[boardLevel][i] = '=';
		break;
	case 'x':
		if (!(playerPosition + boardLength == fieldWidth)) playerPosition++;
		for (int i = playerPosition; i <= playerPosition + boardLength - 1; i++) field[boardLevel][i] = '=';
		break;
	}
}

void BallMove()
{
	if (Yposition == boardLevel - 1 && Xposition >= playerPosition-1 && Xposition <= playerPosition + boardLength) VerticalDirection = -1;
	if (Yposition == boardLevel + 1 && Xposition >= playerPosition-1 && Xposition <= playerPosition + boardLength) VerticalDirection = 1;

	if (Xposition < fieldWidth && HorizontalDirection == 1)	Xposition++;
	if (Xposition > 0 && HorizontalDirection == -1)	Xposition--;
	if (Xposition == fieldWidth) HorizontalDirection = -1;
	if (Xposition == 0) HorizontalDirection = 1;

	if (Yposition < fieldHeight && VerticalDirection == 1)	Yposition++;
	if (Yposition > 0 && VerticalDirection == -1)	Yposition--;
	if (Yposition == fieldHeight) VerticalDirection = -1;
	if (Yposition == 0) VerticalDirection = 1;
}
