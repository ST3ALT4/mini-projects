#include <bits/stdc++.h>

void start(char[3][3]);
void print(char[3][3]);
void init(char[3][3]);
bool checkRow(char[3][3], int);
bool checkCols(char[3][3], int);
bool checkDia1(char[3][3]);
bool checkDia2(char[3][3]);

int main()
{
    char arr[3][3];
    init(arr);
    start(arr);
    return 0;
}

void init(char arr[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            arr[i][j] = ' ';
        }
    }
}

void start(char arr[3][3])
{
    int x = -1, y = -1;
    static int turn = 0;

    print(arr);

    if (turn == 9)
    {
        std::cout << "Tie" << std::endl;
        return;
    }

    std::cout << "Enter the pos (row and column): "<<std::endl;
    std::cin >> x >> y;

    if (x < 0 || x >= 3 || y < 0 || y >= 3)
    {
        std::cout << "Invalid position. Try again." << std::endl;
        start(arr);
        return;
    }

    if (arr[x][y] == ' ')
    {
        if(turn % 2 == 0){
            arr[x][y] = 'X';
        }else{
            arr[x][y] = 'O';
        }
        turn++;
    }
    else
    {
        std::cout << "Position already taken. Try again." << std::endl;
        start(arr);
        return;
    }

    if (checkCols(arr, y) || checkRow(arr, x) || checkDia1(arr) || checkDia2(arr))
    {
        print(arr);
        std::cout << arr[x][y] << " Wins!" << std::endl;
        return;
    }
    else
    {
        start(arr);
    }
}

void print(char arr[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool checkDia1(char arr[3][3])
{
    char first = arr[0][0];
    if (first == ' ')
        return false;
    
    for (int i = 1; i < 3; i++)
    {
        if (arr[i][i] != first)
            return false;
    }

    return true;
}

bool checkDia2(char arr[3][3])
{
    char first = arr[0][2];
    if (first == ' ')
        return false;
    
    for (int i = 1; i < 3; i++)
    {
        if (arr[i][2 - i] != first)
            return false;
    }

    return true;
}

bool checkRow(char arr[3][3], int x)
{
    char first = arr[x][0];
    if (first == ' ')
        return false;
    
    for (int j = 1; j < 3; j++)
    {
        if (arr[x][j] != first)
            return false;
    }

    return true;
}

bool checkCols(char arr[3][3], int y)
{
    char first = arr[0][y];
    if (first == ' ')
        return false;
    
    for (int j = 1; j < 3; j++)
    {
        if (arr[j][y] != first)
            return false;
    }

    return true;
}
