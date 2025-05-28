#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm> // Для std::max и std::min

using namespace std;

// Структура для представления клетки
struct Cell {
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int adjacentMines = 0;
};

// Класс игры
class Minesweeper {
private:
    int size;
    int mines;
    vector<vector<Cell>> board;

public:
    Minesweeper(int s, int m) : size(s), mines(m), board(s, vector<Cell>(s)) {}

    // Инициализация игрового поля
    void initializeBoard() {
        // Очистка поля
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                board[i][j] = Cell();
            }
        }

        // Размещение мин
        int minesPlaced = 0;
        while (minesPlaced < mines) {
            int x = rand() % size;
            int y = rand() % size;

            if (!board[x][y].isMine) {
                board[x][y].isMine = true;
                minesPlaced++;
            }
        }

        // Подсчет мин вокруг каждой клетки
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (!board[i][j].isMine) {
                    int count = 0;
                    for (int di = -1; di <= 1; ++di) {
                        for (int dj = -1; dj <= 1; ++dj) {
                            int ni = i + di;
                            int nj = j + dj;
                            if (ni >= 0 && ni < size && nj >= 0 && nj < size && board[ni][nj].isMine) {
                                count++;
                            }
                        }
                    }
                    board[i][j].adjacentMines = count;
                }
            }
        }
    }

    // Отображение игрового поля
    void displayBoard(bool showMines = false) {
        cout << "  ";
        for (int i = 0; i < size; ++i) {
            cout << i << " ";
        }
        cout << endl;

        for (int i = 0; i < size; ++i) {
            cout << i << " ";
            for (int j = 0; j < size; ++j) {
                if (board[i][j].isRevealed || showMines) {
                    if (board[i][j].isMine) {
                        cout << "* ";
                    }
                    else {
                        cout << board[i][j].adjacentMines << " ";
                    }
                }
                else if (board[i][j].isFlagged) {
                    cout << "F ";
                }
                else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }

    // Открытие клетки
    bool revealCell(int x, int y) {
        if (x < 0 || x >= size || y < 0 || y >= size || board[x][y].isRevealed) {
            return true;
        }

        if (board[x][y].isMine) {
            return false;
        }

        board[x][y].isRevealed = true;

        if (board[x][y].adjacentMines == 0) {
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    revealCell(x + di, y + dj);
                }
            }
        }

        return true;
    }

    // Установка/снятие флага
    void toggleFlag(int x, int y) {
        if (x >= 0 && x < size && y >= 0 && y < size && !board[x][y].isRevealed) {
            board[x][y].isFlagged = !board[x][y].isFlagged;
        }
    }

    // Проверка на победу
    bool checkWin() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (!board[i][j].isMine && !board[i][j].isRevealed) {
                    return false;
                }
            }
        }
        return true;
    }

    int getSize() const { return size; }
};

// Очистка ввода
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Отображение главного меню
int showMainMenu() {
    system("cls");
    cout << "============================\n";
    cout << "         *SAPER*\n";
    cout << "============================\n";
    cout << "1. New game\n";
    cout << "2. Exit\n";
    cout << "Choose an option: ";

    int choice;
    cin >> choice;
    clearInput();
    return choice;
}

// Отображение меню выбора сложности
void showDifficultyMenu(int& size, int& mines) {
    system("cls");
    cout << "============================\n";
    cout << "  CHOOSING THE DIFFICULTY\n";
    cout << "============================\n";
    cout << "1. Easy (8x8, 10 min)\n";
    cout << "2. Medium (12x12, 30 min)\n";
    cout << "3. Hard (16x16, 60 min)\n";
    cout << "4. user-defined\n";
    cout << "Select the options: ";

    int choice;
    cin >> choice;
    clearInput();

    switch (choice) {
    case 1:
        size = 8;
        mines = 10;
        break;
    case 2:
        size = 12;
        mines = 30;
        break;
    case 3:
        size = 16;
        mines = 60;
        break;
    case 4:
        system("cls");
        cout << "Enter the field size(5-20): ";
        cin >> size;
        size = max(5, min(size, 20));

        int maxMines = size * size / 3;
        cout << "Enter the number of mines (1-" << maxMines << "): ";
        cin >> mines;
        mines = max(1, min(mines, maxMines));
        break;
    }
}

// Основной игровой цикл
void playGame(int size, int mines) {
    Minesweeper game(size, mines);
    game.initializeBoard();

    bool gameOver = false;
    bool win = false;

    while (!gameOver && !win) {
        system("cls");
        cout << "Size: " << size << "x" << size << " | Mines: " << mines << endl;
        game.displayBoard();

        cout << "\nCommands:\n";
        cout << "r [row] [column] - open the cage\n";
        cout << "f [row] [column] - set/remove the flag\n";
        cout << "q - exit the menu\n";
        cout << "Enter the command: ";

        char command;
        int x, y;

        cin >> command;

        if (command == 'q' || command == 'Q') {
            return;
        }

        if (command == 'r' || command == 'R' || command == 'f' || command == 'F') {
            cin >> x >> y;

            if (cin.fail() || x < 0 || x >= size || y < 0 || y >= size) {
                clearInput();
                cout << "Incorrect input. Press /Enter/ to continue..";
                cin.get();
                continue;
            }

            switch (command) {
            case 'r':
            case 'R':
                if (!game.revealCell(x, y)) {
                    gameOver = true;
                }
                break;
            case 'f':
            case 'F':
                game.toggleFlag(x, y);
                break;
            }
        }
        else {
            clearInput();
            cout << "Unknown team. Press /Enter/ to continue...";
            cin.get();
            continue;
        }

        if (game.checkWin()) {
            win = true;
        }
    }

    system("cls");
    game.displayBoard(true);

    if (win) {
        cout << "\nCongratulations! You've won!\n";
    }
    else {
        cout << "\nThe game is over! You've hit a landmine!\n";
    }

    // Пауза перед возвратом в меню
    cout << "Press Enter to return to the menu...";
    clearInput(); 
    cin.get();
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int size = 10;
    int mines = 15;

    while (true) {
        int choice = showMainMenu();

        switch (choice) {
        case 1:
            showDifficultyMenu(size, mines);
            playGame(size, mines);
            break;
        case 2:
            return 0;
        default:
            cout << "Wrong choice. Try again.\n";
            cin.get();
        }
    }

    return 0;
}