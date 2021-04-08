/*
 * BaseBoard.h
 *
 *  Created on: 21/02/2021
 *      Author: Dongmo
 */

#ifndef BOARD_H_
#define BOARD_H_

#include<vector>
#include<stack>
#include<algorithm>
#include <windows.h>

using namespace std;


class Board
{
private:
    int boardSize;
    int turn;
    vector<int> emptyCells;
    vector<int> allCells;
public:
    Board(int bs)
    {
        boardSize = bs;
        grid = new int *[boardSize];
        for (int i = 0; i < boardSize; i++)
            grid[i] = new int[boardSize];

        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
            {
                grid[i][j] = 0;
            }
        turn = 1;
    }

    virtual ~Board()
    {

        for (int i = 0; i < boardSize; i++)
            delete[] grid[i];

        delete[] grid;
    }

    Board(Board &cboard)
    {
        boardSize = cboard.getBoardSize();

        grid = new int *[boardSize];
        for (int i = 0; i < boardSize; i++)
            grid[i] = new int[boardSize];

        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
                grid[i][j] = 0;

        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
            {
                grid[i][j] = cboard.getGridVal(i, j);
            }

        turn = cboard.getTurn();
    }

    //region Spots
    void addCells();

    void printCells(vector<int> spots);

    //region Free Spots
    vector<int> getFreeCells()
    {
        return emptyCells;
    }

    int freeCellsSize()
    {
        return emptyCells.size();
    }

    void removeFreeCell(int x, int y);
    //endregion

    //region All Spots
    vector<int> getAllCells()
    {
        return allCells;
    }

    int allCellsSize()
    {
        return allCells.size();
    }
    //endregion

    //endregion

    int **grid;

    int getBoardSize()
    {
        return boardSize;
    }

    int getGridVal(int x, int y)
    {
        return grid[x][y];
    }

    int **getGrid()
    {
        return grid;
    }

    int getTurn()
    {
        return turn;
    }

    bool validInput(int, int);

    bool addMove(int playerIndex, int x, int y);

    int checkWinningStatus(int playerType, int x, int y);

    bool lineWin(int playerType);

    bool checkWinDFS(int playerType, int x, int y);

    void printBoard();

    bool isBoardFull();

    bool isFullThisTurn();

    stack<int> checkNeighbours(int player, int x, int y);

    void printNeighbours(stack<int> s);

    void printCoord(int x, int y, bool el);

    int CoordToInt(int x, int y)
    {
        return (x * boardSize + y);
    }
};

void Board::addCells()
{
    for (int x = 0; x < boardSize; x++)
        for (int y = 0; y < boardSize; y++)
        {
            int value = x * boardSize + y;
            emptyCells.push_back(value);
            allCells.push_back(value);
        }
}

void Board::printCells(vector<int> spots)
{
    int x, y;
    int tally = 0;
    for (int i = 0; i < spots.size(); i++)
    {
        x = (spots[i] / boardSize) + 1, y = (spots[i] % boardSize) + 1;
        if (x > tally)
        {
            tally = x;
            cout << endl;
        }
        else
        {
            cout << ", ";
        }
        printCoord(x, y, false);
    }

    cout << endl;
}

void Board::removeFreeCell(int x, int y)
{
    int value = x * boardSize + y;

    for (int i = 0; i < emptyCells.size(); i++)
    {
        if (emptyCells[i] == value)
        {
            emptyCells.erase(emptyCells.begin() + i);
            break;
        }
    }
}

bool Board::validInput(int x, int y)
{
    if (x < 0 || y < 0 || x >= boardSize || y >= boardSize)
    {
        cout << "Node (" << x + 1 << ", " << y + 1 << ") out of range!" << endl;
        return false;
    }

    if (grid[x][y] != 0)
    {
        cout << "Invalid move. The cell has been occupied." << endl;
        return false;
    }

    return true;
}

bool Board::addMove(int playerIndex, int x, int y)
{
    if (playerIndex != turn)
    {
        cout << "It is not the player's turn!" << endl;
        return false;
    }

    if (grid[x][y] != 0)
    {
        cout << "Invalid move. The cell has been occupied." << endl;
        return false;
    }

    grid[x][y] = playerIndex;
    removeFreeCell(x, y);

    turn = -1 * turn;
    return true;
}

void Board::printBoard()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << endl;

    //SECTION - Top numbers (X)
    cout << "    ";
    for (int j = 0; j < boardSize; j++)
    {
        if (j < 9)
        {
            cout << "_";
        }
        SetConsoleTextAttribute(hConsole, 12);//RED
        cout << j + 1;
        SetConsoleTextAttribute(hConsole, 15);//WHITE
        cout << "_.";
    }
    cout << endl;

    //SECTION - GRID
    for (int i = 0; i < boardSize; i++)
    {
        //SECTION - Left Spacing
        for (int k = 0; k < i; k++)
            cout << "  ";

        //SECTION - Left numbers ())
        if (i < 9)
            cout << " ";
        SetConsoleTextAttribute(hConsole, 9);//BLUE
        cout << i + 1;
        SetConsoleTextAttribute(hConsole, 15);//WHITE
        cout << " ";

        //SECTION - Loop through all cells
        for (int j = 0; j < boardSize; j++)
        {
            //SECTION - Empty Cells
            if (grid[i][j] == 0)
            {
                if (j == 0)
                {
                    cout << "|___|";
                }
                else
                {
                    cout << "___|";
                }
            }
                //SECTION - X Cells
            else if (grid[i][j] == 1)
            {
                //First cell
                if (j == 0)
                {
                    cout << "|_";
                    SetConsoleTextAttribute(hConsole, 12);//RED
                    cout << "X";
                    SetConsoleTextAttribute(hConsole, 15);//WHITE
                    cout << "_|";
                }
                    //All other Cells
                else
                {
                    cout << "_";
                    SetConsoleTextAttribute(hConsole, 12);//RED
                    cout << "X";
                    SetConsoleTextAttribute(hConsole, 15);//WHITE
                    cout << "_|";
                }
            }
                //SECTION - O Cells
            else
            {
                //First cell
                if (j == 0)
                {
                    cout << "|_";
                    SetConsoleTextAttribute(hConsole, 9);//BLUE
                    cout << "O";
                    SetConsoleTextAttribute(hConsole, 15);//WHITE
                    cout << "_|";
                }
                    //All other Cells
                else
                {
                    cout << "_";
                    SetConsoleTextAttribute(hConsole, 9);//BLUE
                    cout << "O";
                    SetConsoleTextAttribute(hConsole, 15);//WHITE
                    cout << "_|";
                }
            }
        }

        //SECTION - Close End Boxes & Right Numbers (O)
        if (i < boardSize - 1)
        {
            cout << "_";
            SetConsoleTextAttribute(hConsole, 9);//BLUE
            cout << i + 1;
            SetConsoleTextAttribute(hConsole, 15);//WHITE
        }
        else
        {
            cout << " ";
            SetConsoleTextAttribute(hConsole, 9);//BLUE
            cout << i + 1;
            SetConsoleTextAttribute(hConsole, 15);//WHITE
        }
        cout << endl;
    }

    //SECTION - Bottom Numbers
    for (int s = 0; s < boardSize; s++)
        cout << "  ";
    cout << "  ";
    for (int e = 0; e < boardSize; e++)
    {
        if (e < 9)
        {
            cout << " ";
        }
        SetConsoleTextAttribute(hConsole, 12);//RED
        cout << e + 1;
        SetConsoleTextAttribute(hConsole, 15);//WHITE
        cout << "  ";
    }
    cout << endl;
}

bool Board::isBoardFull()
{
    if (emptyCells.size() > 0)
        return false;

    cout << "Woops! The board is full and no one has won, looks like it's game over." << endl;
    return true;
}

bool Board::isFullThisTurn()
{
    int count = 0;
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (grid[i][j] == 0)
                ++count;
        }
    }

    if (count == 1)
    {
        cout << "Yikes, looks like there's only one free spot. Better make this move count or its game over." << endl;
        return true;
    }
    else
        return false;
}

int Board::checkWinningStatus(int playerType, int x, int y)
{
    if ((emptyCells.size() + (boardSize * 2 - 1)) <= (boardSize * boardSize))
    {
        if (lineWin(playerType) || checkWinDFS(playerType, x, y))
            return playerType;
    }
    return 0;
}

bool Board::lineWin(int playerType)
{
    bool line;

    //Check if a whole row is complete (for O)
    if (playerType == -1)
        for (int r = 0; r < boardSize; r++)
        {
            line = true;

            for (int c = 0; c < boardSize - 1; c++)
            {
                if (grid[r][c] != grid[r][c + 1] || grid[r][c] != playerType)
                {
                    line = false;
                    break;
                }
            }
            if (line)
                return true;
        }

    //Check if a whole column is complete (for X)
    if (playerType == 1)
        for (int c = 0; c < boardSize; c++)
        {
            line = true;

            for (int r = 0; r < boardSize - 1; r++)
            {
                if (grid[r][c] != grid[r + 1][c] || grid[r][c] != playerType)
                {
                    line = false;
                    break;
                }
            }
            if (line)
                return true;
        }

    line = true;
    for (int i = 0; i < boardSize - 1; i++)
    {
        if (grid[i][boardSize - i] != grid[i + 1][boardSize - i - 1] || grid[i][boardSize - i] != playerType)
        {
            line = false;
            break;
        }
    }

    return line;
}

bool Board::checkWinDFS(int playerType, int x, int y) //DFS
{
    stack<int> trackStack = checkNeighbours(playerType, x, y);
    vector<int> visitedStack;
    if (trackStack.empty())
        return false;

    bool start = false, finish = false;
    int startGoal = 0, endGoal = boardSize - 1;

    while (!trackStack.empty())
    {
        int s = trackStack.top();
        trackStack.pop();
        visitedStack.push_back(s);

        int sX = s / boardSize;
        int sY = s % boardSize;

        if (playerType == -1)
        {
            if (sY == startGoal)
                start = true;
            else if (sY == endGoal)
                finish = true;
        }
        else if (playerType == 1)
        {
            if (sX == startGoal)
                start = true;
            else if (sX == endGoal)
                finish = true;
        }

        if (start && finish)
        {
            return true;
        }
        else
        {
            stack<int> children = checkNeighbours(playerType, sX, sY);
            while (!children.empty())
            {
                if (find(visitedStack.begin(), visitedStack.end(), children.top()) !=
                    visitedStack.end())
                    children.pop();
                else
                {
                    trackStack.push(children.top());
                    children.pop();
                }
            }
        }
    }
    return false;
}

stack<int> Board::checkNeighbours(int player, int x, int y)
{
    stack<int> neighbours;
    int value;

    //Left
    if ((y - 1) >= 0)
        if (grid[x][y - 1] == player)
        {
            value = CoordToInt(x, (y - 1));
            neighbours.push(value);
        }

    //Right
    if ((y + 1) < boardSize)
        if (grid[x][y + 1] == player)
        {
            value = CoordToInt(x, (y + 1));
            neighbours.push(value);
        }

    //Up-Left
    if ((x - 1) >= 0)
        if (grid[x - 1][y] == player)
        {
            value = CoordToInt((x - 1), y);
            neighbours.push(value);
        }

    //Up-Right
    if ((x - 1) >= 0 && (y + 1) < boardSize)
        if (grid[x - 1][y + 1] == player)
        {
            value = CoordToInt((x - 1), (y + 1));
            neighbours.push(value);
        }

    //Down-Left
    if ((x + 1) < boardSize && (y - 1) >= 0)
        if (grid[x + 1][y - 1] == player)
        {
            value = CoordToInt((x + 1), (y - 1));
            neighbours.push(value);
        }

    //Down-Right
    if ((x + 1) < boardSize)
        if (grid[x + 1][y] == player)
        {
            value = CoordToInt((x + 1), y);
            neighbours.push(value);
        }

    return neighbours;
}

void Board::printNeighbours(stack<int> s)
{
    if (s.empty())
        return;

    int t = s.top();
    s.pop();

    printNeighbours(s);

    int x = t / boardSize;
    int y = t % boardSize;
    printCoord(x + 1, y + 1, false);
    cout << ", ";
}

void Board::printCoord(int x, int y, bool el)
{
    cout << "(" << x << ", " << y << ")";
    if (el)
        cout << endl;
}

#endif /* BOARD_H_ */