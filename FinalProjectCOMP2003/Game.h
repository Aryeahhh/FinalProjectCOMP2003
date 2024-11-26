#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

class LotteryGame {
private:
    int numLines;                          // Number of lines chosen by the user
    bool encore;                           // Whether the user opted for Encore
    int encoreNumber;                      // User's chosen or generated Encore number
    double totalCost;                      // Total cost of the ticket
    std::vector<std::vector<int>> ticket;  // Ticket lines

    // Helper functions
    std::vector<int> generateLine();
    int countMatches(const std::vector<int>& ticketLine);
    double calculateMainPrize(int matches, bool bonusMatch);
    int countEncoreMatches(int userEncore, int winningEncore);
    double calculateEncorePrize(int matches);

public:
    LotteryGame();  // Constructor

    // Game functions
    void displayRules();
    void generateTicket();
    void saveTicketToFile(const std::string& filename);
    void saveResultsToFile(const std::string& filename);
    void play();
};

#endif
