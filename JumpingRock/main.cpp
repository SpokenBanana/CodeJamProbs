#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

int checkDirection(string player, int xDiff, int yDiff, int currentpos[2], string board[4][4], int &spaces) {
    if (currentpos[0] >= 4 || currentpos[1] >= 4 || currentpos[0] < 0 || currentpos[1] < 0) {
        return 0;
    }
    if (board[currentpos[0]][currentpos[1]] == ".") {
        spaces++;
        return 0;
    }
    if (board[currentpos[0]][currentpos[1]] == player || board[currentpos[0]][currentpos[1]] == "T") {
        // we move along the same direction until we can't no longer (run into the different character, out of boundary)
        int nextpos[2] = {currentpos[0] + xDiff, currentpos[1] + yDiff};
        // 1 for confirming this space is the player's character, and we move along
        return 1 + checkDirection(player, xDiff, yDiff, nextpos, board, spaces);
    }
    else {
        return 0;
    }
}

int getRowAndColumnScore(string player, int current[], string b[4][4], int &blankSpaces) {
    // we keep track of the blank spaces to make it easier to determine if it was draw or non-complete game
    //the 0,1 literals are the direction we move to find a winning move.
    int column = checkDirection(player, 0, 1, current, b, blankSpaces) + checkDirection(player, 0,-1, current, b, blankSpaces) - 1;
    int rows = checkDirection(player, 1,0, current, b, blankSpaces) + checkDirection(player, -1,0, current, b, blankSpaces) - 1;
    //int diagonals = checkDirection(player, 1,1, current, b, blankSpaces) + checkDirection(player, -1,-1, current, b, blankSpaces) - 1;
    return max(column, rows);
}

int getScore(int current[], string b[4][4], int&blankSpaces, string & winner) {
    string player = b[current[0]][current[1]];
    if (player == "T") {
        // if this is true then we set player as T, and we cannot say "T won" so we check which one actually won
        int xScore = getRowAndColumnScore("X", current, b, blankSpaces);
        int oScore = getRowAndColumnScore("0", current, b, blankSpaces);
        winner = (xScore > oScore) ? "X" : "0";
        return max(xScore, oScore);
    }
    else {// otherwise we just go along with the X or O we got from b[i][i]
        winner = player;
        return getRowAndColumnScore(player, current, b, blankSpaces);
    }
}
int getScore(int current[], string b[4][4], int increments[], int&blankSpaces, string & winner) {
    // This is for diagonal checks, same concept as the other getScore
    string player = b[current[0]][current[1]];
    if (b[3][0] == "T") {
        int xScore = checkDirection("X", increments[0], increments[1], current, b, blankSpaces);
        int oScore = checkDirection("O", increments[0], increments[1], current, b, blankSpaces);
        winner = (xScore > oScore) ? "X" : "0";
        return max(xScore, oScore);
    }
    else {
        winner = player;
        return checkDirection(player, increments[0], increments[1], current, b, blankSpaces);
    }
}

int main() {
    int n;
    cin >> n;
    string line;
    getline(cin, line);
    for (int i = 0; i < n; i++) {
        string b[4][4];
        getline(cin, line);
        int j = 0;
        while (line != "") {
            for (int i = 0; i < 4; i++)
                b[j][i] = line[i];
            j++;
            getline(cin, line);
        }
        cout << "Case #" << i+1 << ": ";
        bool won = false;
        int blankSpaces = 0;
        // we check diagonals first, starting at the top left (0,0) down and another starting at the bottom left (3,0) up
        int start[2][2] = {{0,0}, {3,0}};
        // to move down from (0,0) we use (1,1) to go to the bottom right corner, same idea with the other increments
        int increments[2][2] = {{1,1}, {-1,1}};
        string win; //get the player who won
        if (getScore(start[0], b, increments[0], blankSpaces, win) == 4 || getScore(start[1], b, increments[1], blankSpaces, win) == 4) {
            cout << win << " won" << endl;
            continue; // we got the outcome, no need to go forward
        }
        // we are going to travel diagonally down, starting at the top left corner and ending at the bottom left.
        // this way we avoid doing too many checks that are unnecessary.
        for (int i = 0; i < 4; i++) {
            int current[2] = {i, i};
            string player;
            int score = getScore(current, b, blankSpaces, player);
            if (score == 4) { //4 in a row wins the game!
                cout << player << " won" << endl;
                // we have this flag to avoid unnecessary checks
                won = true;
                break;
            }
        }
        if (!won) //this is why we kept track of blank spaces (.) if there is none, then we know it was a draw.
            (blankSpaces > 0) ? cout << "Game has not completed" << endl : cout << "Draw" << endl;
    }
    return 0;
}