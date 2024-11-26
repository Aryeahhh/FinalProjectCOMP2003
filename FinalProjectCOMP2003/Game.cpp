#include "Game.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constants
const int MIN_NUM = 1;
const int MAX_NUM = 49;
const int NUMBERS_PER_LINE = 6;
const int LINE_COST = 3;

const vector<int> WINNING_NUMBERS = { 2, 6, 19, 25, 32, 33 };
const int BONUS_NUMBER = 42;
const int ENCORE_NUMBER = 4445337;

struct MainPrize {
    int matches;
    bool bonusMatch;
    double prizeAmount;
};

const vector<MainPrize> MAIN_PRIZES = {
    {6, false, 2000000.00},
    {5, true, 50000.00},
    {5, false, 500.00},
    {4, false, 50.00},
    {3, false, 10.00},
    {2, true, 3.00},
    {2, false, 0.00} // Free play
};

const vector<double> ENCORE_PRIZES = { 0.00, 0.00, 10.00, 100.00, 1000.00, 10000.00, 100000.00, 1000000.00 };

// Constructor
LotteryGame::LotteryGame() : numLines(0), encore(false), encoreNumber(0), totalCost(0) {}

// Display Rules
void LotteryGame::displayRules() {
    cout << "Welcome to the 49 Lottery Game!\n";
    cout << "Rules:\n";
    cout << "- Choose up to 30 lines to play.\n";
    cout << "- Each line costs " << LINE_COST << " CAD.\n";
    cout << "- A bonus line is added for each purchased line.\n";
    cout << "- Encore is optional for 1 CAD.\n";
    cout << "- Winning numbers are predefined.\n\n";
}

// Generate Random Line
vector<int> LotteryGame::generateLine() {
    vector<int> line;
    while (line.size() < NUMBERS_PER_LINE) {
        int num = (rand() % MAX_NUM) + MIN_NUM;
        if (find(line.begin(), line.end(), num) == line.end()) {
            line.push_back(num);
        }
    }
    return line;
}

// Generate Ticket
void LotteryGame::generateTicket() {
    for (int i = 0; i < numLines; ++i) {
        ticket.push_back(generateLine());
        ticket.push_back(generateLine()); // Bonus line
    }
}

// Count Matches
int LotteryGame::countMatches(const vector<int>& ticketLine) {
    int matches = 0;
    for (int num : ticketLine) {
        if (find(WINNING_NUMBERS.begin(), WINNING_NUMBERS.end(), num) != WINNING_NUMBERS.end()) {
            matches++;
        }
    }
    return matches;
}

// Calculate Main Prize
double LotteryGame::calculateMainPrize(int matches, bool bonusMatch) {
    for (const auto& prize : MAIN_PRIZES) {
        if (prize.matches == matches && prize.bonusMatch == bonusMatch) {
            return prize.prizeAmount;
        }
    }
    return 0.0;
}

// Count Encore Matches
int LotteryGame::countEncoreMatches(int userEncore, int winningEncore) {
    int matches = 0;
    for (int i = 6; i >= 0; --i) {
        if (userEncore % 10 == winningEncore % 10) {
            matches++;
        }
        userEncore /= 10;
        winningEncore /= 10;
    }
    return matches;
}

// Calculate Encore Prize
double LotteryGame::calculateEncorePrize(int matches) {
    return matches >= 2 ? ENCORE_PRIZES[matches] : 0.0;
}

// Save Ticket to File
void LotteryGame::saveTicketToFile(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "49 Lottery Ticket\n";
        file << "Encore Number: " << setw(7) << setfill('0') << encoreNumber << "\n\n";
        for (const auto& line : ticket) {
            for (int num : line) {
                file << setw(2) << num << " ";
            }
            file << "\n";
        }
        file << "Total cost: $" << fixed << setprecision(2) << totalCost << "\n";
        file.close();
    }
}

// Save Results to File
// Save Results to File
void LotteryGame::saveResultsToFile(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "Lottery Results\n";
        file << "Winning Numbers: ";
        for (int num : WINNING_NUMBERS) {
            file << setw(2) << num << " ";
        }
        file << "| Bonus: " << BONUS_NUMBER << "\n\n";

        int lineNumber = 1;
        double totalWinnings = 0.0; // Track total winnings for the lottery
        cout << "\nLottery Results:\n";
        for (const auto& line : ticket) {
            int matches = countMatches(line);
            bool bonusMatch = find(line.begin(), line.end(), BONUS_NUMBER) != line.end();
            double prize = calculateMainPrize(matches, bonusMatch);
            totalWinnings += prize;

            file << "Line " << lineNumber << ": ";
            for (int num : line) {
                file << setw(2) << num << " ";
            }
            file << " | Matches: " << matches;
            if (bonusMatch) file << " + Bonus";
            file << " | Prize: $" << fixed << setprecision(2) << prize << "\n";

            cout << "Line " << lineNumber++ << ": ";
            for (int num : line) {
                cout << setw(2) << num << " ";
            }
            cout << " | Matches: " << matches;
            if (bonusMatch) cout << " + Bonus";
            cout << " | Prize: $" << fixed << setprecision(2) << prize << "\n";
        }

        // Output total winnings for the normal lottery
        file << "\nTotal Lottery Winnings: $" << fixed << setprecision(2) << totalWinnings << "\n";
        cout << "\nTotal Lottery Winnings: $" << fixed << setprecision(2) << totalWinnings << "\n";

        // Add Encore results if the user played Encore
        if (encore) {
            file << "\nEncore Results:\n";
            int encoreMatches = countEncoreMatches(encoreNumber, ENCORE_NUMBER);
            double encorePrize = calculateEncorePrize(encoreMatches);
            file << "Encore Number: " << setw(7) << setfill('0') << encoreNumber << "\n";
            file << "Winning Encore: " << setw(7) << setfill('0') << ENCORE_NUMBER << "\n";
            file << "Encore Matches: " << encoreMatches << " | Prize: $"
                << fixed << setprecision(2) << encorePrize << "\n";

            cout << "\nEncore Results:\n";
            cout << "Encore Matches: " << encoreMatches << " | Prize: $"
                << fixed << setprecision(2) << encorePrize << "\n";
        }

        file.close();
    }
}



// Play Game
void LotteryGame::play() {
    displayRules();
    cout << "Enter number of lines to play (1-30): ";
    Utils::validateInput(numLines, 1, 30, "Please enter a valid number of lines (1-30): ");

    char encoreChoice;
    cout << "Would you like to play Encore for $1? (y/n): ";
    Utils::validateCharInput(encoreChoice, "Please enter 'y' or 'n': ");
    encore = (encoreChoice == 'y' || encoreChoice == 'Y');

    if (encore) {
        cout << "Enter Encore number manually or generate randomly? (m/r): ";
        char manualOrRandom;
        cin >> manualOrRandom;

        if (manualOrRandom == 'm') {
            cout << "Enter a 7-digit Encore number: ";
            Utils::validateInput(encoreNumber, 1000000, 9999999, "Please enter a valid 7-digit number: ");
        }
        else if(manualOrRandom == 'r') {
            encoreNumber = rand() % 9000000 + 1000000; // Generate 7-digit number
            cout << "Generated Encore Number: " << setw(7) << setfill('0') << encoreNumber << "\n";
        }
    }

    totalCost = numLines * LINE_COST + (encore ? 1 : 0);
    cout << "Total cost: $" << fixed << setprecision(2) << totalCost << "\n";

    generateTicket();
    saveTicketToFile(Utils::generateFilename("Ticket"));
    saveResultsToFile(Utils::generateFilename("Results"));
}
