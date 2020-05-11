#include <iostream>
#include <conio.h>
#include <windows.h>

#define fieldHeight 25
#define fieldWidth 50

using namespace std;

int score = 0;
int difficulty = 50;
bool gameIsOver = false;
char field[fieldHeight][fieldWidth];
int playerPosition = 9;
int boardLength = 6, boardLevel = fieldHeight-4;
char command;
int HorizontalDirection = 1, VerticalDirection = 1;
int Xposition = playerPosition+2, Yposition = boardLevel-1;
int aimArray[fieldHeight][fieldWidth];
int aimLevel = fieldHeight / 3;

void CreateField();
void Draw();
void PlayerMove(char command);
void BallLogic();
void fillAimArray();

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
	fillAimArray();
	system("pause");
	while (!gameIsOver)
	{
		Sleep(difficulty);
		if (_kbhit()) command = _getch();
		PlayerMove(command);
		BallLogic();
		CreateField();
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), start);
		Draw();
	}
}

void CreateField()
{
	for (int i = 0; i < fieldHeight; i++)
	{
			for (int j = 0; j < fieldWidth; j++)
			{
				if (aimArray[i][j] == 1) field[i][j] = '-';
				else field[i][j] = ' ';

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

void BallLogic()
{
	// Отталкивание от платформы
	if (Yposition == boardLevel - 1 && Xposition >= playerPosition - 1 && Xposition <= playerPosition + boardLength) VerticalDirection = -1;
	if (Yposition == boardLevel + 1 && Xposition >= playerPosition - 1 && Xposition <= playerPosition + boardLength) VerticalDirection = 1;
	if (Yposition == boardLevel && Xposition == playerPosition + boardLength) VerticalDirection = 1;
	if (Yposition == boardLevel && Xposition == playerPosition) VerticalDirection = -1;
	if (Yposition == boardLevel - 1 && Xposition == playerPosition - 1) {
		VerticalDirection = -1; HorizontalDirection = -1;
	}
	if (Yposition == boardLevel - 1 && Xposition == playerPosition + boardLength) {
		VerticalDirection = -1; HorizontalDirection = 1;
	}
	if (Yposition == boardLevel + 1 && Xposition == playerPosition - 1) {
		VerticalDirection = 1; HorizontalDirection = -1;
	}
	if (Yposition == boardLevel + 1 && Xposition == playerPosition + boardLength) {
		VerticalDirection = 1; HorizontalDirection = 1;
	}

	// Взаимодействие с целью
	if (aimArray[Yposition - 1][Xposition] == 1)
	{
		VerticalDirection = 1;
		aimArray[Yposition - 1][Xposition] = 0;
		score++;
	}
	if (aimArray[Yposition + 1][Xposition] == 1)
	{
		VerticalDirection = -1;
		aimArray[Yposition + 1][Xposition] = 0;
		score++;
	}
	if (aimArray[Yposition][Xposition - 1] == 1)
	{
		HorizontalDirection = 1;
		aimArray[Yposition][Xposition - 1] = 0;
		score++;
	}
	if (aimArray[Yposition][Xposition + 1] == 1)
	{
		HorizontalDirection = -1;
		aimArray[Yposition][Xposition + 1] = 0;
		score++;
	}
	if (aimArray[Yposition + 1][Xposition + 1] == 1)
	{
		HorizontalDirection = -1;
		VerticalDirection = -1;
		aimArray[Yposition + 1][Xposition + 1] = 0;
		score++;
	}
	if (aimArray[Yposition - 1][Xposition - 1] == 1)
	{
		HorizontalDirection = 1;
		VerticalDirection = 1;
		aimArray[Yposition - 1][Xposition - 1] = 0;
		score++;
	}

	// Движение мячика
	if (Xposition < fieldWidth && HorizontalDirection == 1)	Xposition++;
	if (Xposition > 0 && HorizontalDirection == -1)	Xposition--;
	if (Xposition == fieldWidth) HorizontalDirection = -1;
	if (Xposition == 0) HorizontalDirection = 1;

	if (Yposition < fieldHeight && VerticalDirection == 1)	Yposition++;
	if (Yposition > 0 && VerticalDirection == -1)	Yposition--;
	if (Yposition == fieldHeight) VerticalDirection = -1;
	if (Yposition == 0) VerticalDirection = 1;
}

void fillAimArray()
{
	for (int i = 0; i < fieldHeight; i++)
	{
		for (int j = 0; j < fieldWidth; j++)
		{
			if (i < aimLevel) aimArray[i][j] = 1;
			else aimArray[i][j] = 0;
		}
	}
}