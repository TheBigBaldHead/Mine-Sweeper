#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <windows.h>
#include <iomanip>

using namespace std;

// Function to generate random mines
vector<vector<bool>> generateMines(int n, int numMines) {
    vector<vector<bool>> IsMine(n, vector<bool>(n, false));

    srand(time(NULL));

    while (numMines > 0) {

        int randRow = rand() % n;
        int randCol = rand() % n;

        if (IsMine[randRow][randCol] != '*') {
            IsMine[randRow][randCol] = '*';
            numMines--;
        }
    }

    return IsMine;
}

int MinesFound(vector<vector<char>> Board, vector<vector<bool>> IsMine) {
    int count = 0;
    for (int i = 0; i < Board.size(); i++) {
        for (int j = 0; j < IsMine.size(); j++) {
            if (IsMine[i][j] && Board[i][j] == 'f') {
                count++;
            }
        }
    }
    return count;
}

// Function to display the board
void displayBoard(const vector<vector<char>> board, const vector<vector<bool>> IsMine) {
    int rows = board.size();
    int cols = board[0].size();

    cout << "      ";
    for (int col = 1; col <= cols; col++) {
        if (col < 10)
            cout << "  " << col << "   ";
        else 
            cout << "  " << col << "  ";
    }
    cout << endl;
    cout << "     +";
    for (int col = 0; col < cols; col++) {
        cout << "-----+";
    }
    cout << endl;
    
    for (int row = 0; row < rows; row++) {
        if (row + 1 < 10)
            cout << "  " << row + 1 << "  |  ";
        else 
            cout << "  " << row + 1 << " |  ";

        for (int col = 0; col < cols; col++) {

            if (board[row][col] == ' ')
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            else if (board[row][col] == 'f') 
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            else if (board[row][col] == '7' || board[row][col] == '8') 
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            else
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), board[row][col] - '0' + 8);
            
            cout << board[row][col];
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            cout << "  |  ";
        }
        cout << endl << "     +";
        for (int col = 0; col < cols; col++) {
            cout << "-----+";
        }
        cout << endl;
    }
}

// Function to check if a cell is valid
bool isValidCell(int row, int col, int n) {
    return row >= 0 && row < n && col >= 0 && col < n;
}

// Function to count the number of neighboring mines
int countNeighboringMines(const vector<vector<bool>> IsMine, int row, int col) {
    int count = 0;
    int n = IsMine.size();

    // Define all the possible directions
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Check each neighbor
    for (int i = 0; i < 8; i++) {
        int newRow = row + dx[i];
        int newCol = col + dy[i];

        if (isValidCell(newRow, newCol, n) && IsMine[newRow][newCol] == true) {
            count++;
        }
    }

    return count;
}

// Function to reveal a cell
void revealCell(vector<vector<bool>> IsMine, vector<vector<char>>& board, vector<vector<bool>>& visited, int row, int col) {
    int n = board.size();
    board[row][col] = '0' + countNeighboringMines(IsMine, row, col);
    visited[row][col] = true;

    if (board[row][col] == '0') {
        // Define all the possible directions
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        // Check each neighbor
        for (int i = 0; i < 8; i++) {
            int newRow = row + dx[i];
            int newCol = col + dy[i];

            if (isValidCell(newRow, newCol, n) && !visited[newRow][newCol]) {
                visited[newRow][newCol] = true;
                revealCell(IsMine, board, visited, newRow, newCol);
            }
        }
    }
}

bool GameEnded(vector<vector<char>> Board, vector<vector<bool>> IsMine) {
    for (int i = 0; i < Board.size(); i++) {
        for (int j = 0; j < IsMine.size(); j++) {
            if (IsMine[i][j] && Board[i][j] != 'f') 
                return false;
            if (!IsMine[i][j] && Board[i][j] == 'f')
                return false;
        }
    }
    return true;
}

int main() {
    int n, numMines;
    int level;
    cout << "Choose the hardness level: 1 or 2? ";
    cin >> level;
    while (level != 1 && level != 2) {
        cout << "Choose the hardness level: 1 or 2?";
        cin >> level;
    }
    if (level == 1) {
        n = 10;
        numMines = 10;
    }
    else if (level == 2) {
        n = 16;
        numMines = 40;
    }
    vector<vector<bool>> IsMine = generateMines(n, numMines);
    vector<vector<char>> Board(n, vector<char>(n, ' '));
    vector<vector<bool>> Visited(n, vector<bool>(n, false));
    while (true) {
        int mode;
        cout << endl << "      Enter 1 if you want to flag, Enter 2 if you want to reveal: ";
        cin >> mode;
        if (mode != 1 && mode != 2) {
            cout << "Invalid input!" << endl;
            continue;
        }
        
        int row, col;
        cout << "      Enter the column and the row: ";
        cin >> col >> row;
        row--;
        col--;
        if (isValidCell(row, col, n)) {
            if (mode == 1) {
                if (!Visited[row][col]) {
                    if (Board[row][col] == 'f')
                        Board[row][col] = ' ';
                    else 
                        Board[row][col] = 'f';
                }
                else {
                    cout << "You can't flag this place!";
                    continue;
                }
            }
            else if (mode == 2) {
                if (!Visited[row][col]) {
                    if (IsMine[row][col]) {
                        if (Board[row][col] != 'f') {
                            cout << "You revealed a mine! Boom. You lost!" << endl;
                            cin >> mode;
                            return 0;
                        }
                        else {
                            cout << "You have flagged this place!" << endl;
                            continue;
                        }
                    }
                    revealCell(IsMine, Board, Visited, row, col);
                }
                else {
                    cout << "You can't reveal this place!";
                    continue;
                }
            }
        }
        system("CLS");
        cout << "Mines found: " << MinesFound(Board, IsMine) << "/" << numMines << endl;
        displayBoard(Board, IsMine);
        if (GameEnded(Board, IsMine)) {
            cout << "Game ended. You won!" << endl;
            cin >> mode;
            return 1;
        }
    }
}