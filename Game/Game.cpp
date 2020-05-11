#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define fieldHeight 25
#define fieldWidth 50

using namespace std;

int score = 0;
int maxScore = 0;
int difficulty = 0;
bool gameIsOver = false;
char field[fieldHeight][fieldWidth];
int playerPosition = 9;
int boardLength = 6, boardLevel = fieldHeight-4;
char command;
int HorizontalDirection = 1, VerticalDirection = 1;
int Xposition = playerPosition+2, Yposition = boardLevel-1;
int aimArray[fieldHeight][fieldWidth];
int aimLevel = fieldHeight / 3;

time_t start_while, end_while;

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
		time(&start_while);
		if (score >= maxScore) break;
		Sleep(difficulty);
		if (_kbhit()) command = _getch();
		PlayerMove(command);
		BallLogic();
		Draw();
		time(&end_while);
		cout << "   Time of execute " << difftime(end_while, start_while);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), start);
	}
	cout << "You Win!!!";
	system("pause");
}

void Draw()
{
	for (int i = 0; i < fieldWidth; i++) cout << "_";
	cout << endl;
	for (int i = 0; i < fieldHeight; i++)
	{
		if (i == Yposition || i == Yposition-1 || i == Yposition+1 || i <= boardLevel)
		{
			for (int j = 0; j < fieldWidth; j++)
			{
				if (aimArray[i][j] == 1) field[i][j] = '-';
				else if (i == Yposition && j == Xposition) field[i][j] = '+';
				else if (i == boardLevel && j >= playerPosition && j <= playerPosition + boardLength - 1) field[i][j] = '=';
				else field[i][j] = ' ';
				cout << field[i][j];
			}
		}
		cout << endl;
	}
	for (int i = 0; i < fieldWidth; i++) cout << "^";
	cout << endl;
	cout << "Score: " << score << "   Aim: " << maxScore;
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
			if (i < aimLevel) {
				aimArray[i][j] = 1;
				maxScore++;
			}
			else aimArray[i][j] = 0;
		}
	}
}