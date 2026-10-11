#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

// ---------- Utility ----------

void clearScreen() {
    system("cls"); // Windows. Use system("clear") on Mac/Linux.
}

void pauseForEnter() {
    cout << "\nPress Enter to roll the dice...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int getValidatedInt(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal && value <= maxVal) {
            return value;
        }
        cout << "Invalid input. Enter a number between "
             << minVal << " and " << maxVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ---------- Player Class ----------

class Player {
private:
    string name;
    int position;

public:
    Player(string playerName) {
        name = playerName;
        position = 0; // 0 = not yet on the board
    }

    string getName() const { return name; }
    int getPosition() const { return position; }

    void setPosition(int newPos) {
        if (newPos <= 100) {
            position = newPos;
        }
    }

    bool hasWon() const { return position == 100; }
};

// ---------- Game Engine Class ----------

class SnakeAndLadderGame {
private:
    map<int, int> boardJumps;

    void setupBoard() {
        // --- LADDERS: [Start -> End] ---
        boardJumps[4] = 14;
        boardJumps[9] = 31;
        boardJumps[21] = 42;
        boardJumps[28] = 84;
        boardJumps[51] = 67;
        boardJumps[71] = 91;
        boardJumps[80] = 100;

        // --- SNAKES: [Head -> Tail] ---
        boardJumps[17] = 7;
        boardJumps[54] = 34;
        boardJumps[62] = 19;
        boardJumps[64] = 60;
        boardJumps[87] = 24;
        boardJumps[93] = 73;
        boardJumps[95] = 75;
        boardJumps[98] = 79;
    }

public:
    SnakeAndLadderGame() {
        setupBoard();
    }

    int rollDice() {
        return (rand() % 6) + 1;
    }

    int checkJump(int currentPos) {
        if (boardJumps.find(currentPos) != boardJumps.end()) {
            return boardJumps[currentPos];
        }
        return currentPos;
    }

    bool isLadder(int pos) const {
        auto it = boardJumps.find(pos);
        return it != boardJumps.end() && it->second > pos;
    }

    bool isSnake(int pos) const {
        auto it = boardJumps.find(pos);
        return it != boardJumps.end() && it->second < pos;
    }

    void printBoardLegend() const {
        cout << "-----------------------------------------\n";
        cout << " Board: 1 to 100 | Ladders take you UP    \n";
        cout << "        Snakes take you DOWN              \n";
        cout << "-----------------------------------------\n";
    }
};

// ---------- Main Program ----------

int main() {
    srand(static_cast<unsigned int>(time(0)));

    clearScreen();
    cout << "=========================================\n";
    cout << "        SNAKE AND LADDER GAME            \n";
    cout << "=========================================\n";

    int numPlayers = getValidatedInt("Enter number of players (2-4): ", 2, 4);

    vector<Player> players;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < numPlayers; i++) {
        string pname;
        cout << "Enter name for Player " << (i + 1) << ": ";
        getline(cin, pname);
        if (pname.empty()) pname = "Player" + to_string(i + 1);
        players.push_back(Player(pname));
    }

    SnakeAndLadderGame game;
    int currentPlayerIndex = 0;
    bool gameWon = false;

    while (!gameWon) {
        clearScreen();
        cout << "=========================================\n";
        cout << "        SNAKE AND LADDER GAME            \n";
        cout << "=========================================\n";
        game.printBoardLegend();

        for (const auto& p : players) {
            cout << "  " << p.getName() << " -> Position: " << p.getPosition() << "\n";
        }
        cout << "-----------------------------------------\n";

        Player& current = players[currentPlayerIndex];
        cout << "\n" << current.getName() << "'s turn.";
        pauseForEnter();

        int dice = game.rollDice();
        cout << "\n" << current.getName() << " rolled a " << dice << "!\n";

        int targetPos = current.getPosition() + dice;

        if (targetPos > 100) {
            cout << current.getName() << " needs exact roll to reach 100. Stays at "
                 << current.getPosition() << ".\n";
        } else {
            bool ladder = game.isLadder(targetPos);
            bool snake = game.isSnake(targetPos);
            int finalPos = game.checkJump(targetPos);

            if (ladder) {
                cout << "Landed on a ladder! Climbing from " << targetPos
                     << " to " << finalPos << "!\n";
            } else if (snake) {
                cout << "Oh no, a snake! Sliding down from " << targetPos
                     << " to " << finalPos << "!\n";
            } else {
                cout << "Moved to position " << finalPos << ".\n";
            }

            current.setPosition(finalPos);
        }

        if (current.hasWon()) {
            clearScreen();
            cout << "=========================================\n";
            cout << "              WE HAVE A WINNER!          \n";
            cout << "=========================================\n";
            cout << "\n*** " << current.getName() << " wins the game! ***\n\n";
            gameWon = true;
        } else {
            currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
            cout << "\nPress Enter for next player's turn...";
            cin.get();
        }
    }

    cout << "\nThanks for playing!\n";
    return 0;
}
