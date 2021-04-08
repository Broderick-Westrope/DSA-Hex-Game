/*
 * HumanPlayer.h
 *
 *  Created on: 21/02/2021
 *      Author: dongmo
 */

#ifndef HUMANPLAYER_H_
#define HUMANPLAYER_H_

using namespace std;

class HumanPlayer : public Player
{
public:
    HumanPlayer(int t, string name = "Human") :
            Player(t, name)
    {
    }

    bool getMove(Board *, int &, int &);
};

bool HumanPlayer::getMove(Board *board, int &x, int &y)
{
    bool flag = false;
    bool lastTurn = false;
    int bs = board->getBoardSize();

    lastTurn = board->isFullThisTurn();

    do
    {
        cout << "Input row and column (x, y) between 1 to " << bs << " for " << name << ":";
        board->printCells(board->getFreeCells());
        int row, col;
        cin >> row >> col;
        x = row - 1;
        y = col - 1;
        flag = !board->validInput(x, y); //Flag invalid input if given coordinates are invalid
        if (flag)
            cout << "Invalid input! Please input again." << endl;
    } while (flag);

    system("CLS");
    return true;
}

#endif /* HUMANPLAYER_H_ */