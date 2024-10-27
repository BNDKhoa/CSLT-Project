#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <map>
#include <thread>
#include <chrono>
#include <random>
#include <limits>

using namespace std;

string suits[] = {"♠", "♥", "♦", "♣"};
string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

vector<string> createDeck() {
    vector<string> deck;
    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            deck.push_back(rank + suit);
        }
    }
    return deck;
}

void shuffleDeck(vector<string>& deck) {
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

vector<string> drawCards(vector<string>& deck, int numCards) {
    vector<string> hand;
    for (int i = 0; i < numCards; ++i) {
        hand.push_back(deck.back());
        deck.pop_back(); // Xóa lá bài đã rút khỏi bộ bài
    }
    return hand;
}

int getValidatedInput() {
    int input;
    while (true) {
        cout << "Please enter a number: ";
        string line;
        getline(cin, line); // Read the entire line

        try {
            input = stoi(line); // Try to convert to integer
            if (input < 0) { // Check for negative numbers if needed
                throw invalid_argument("Negative numbers are not allowed.");
            }
            return input; // Return the valid input
        } catch (const invalid_argument& e) {
            cout << "Invalid input. Please enter a valid number." << endl;
        } catch (const out_of_range& e) {
            cout << "Number is out of range. Please enter a valid number." << endl;
        }
    }
}

int isStraight(const vector<string>& hand) {
    vector<int> values;
    for (const auto& card : hand) {
        string rank = card.substr(0, card.size() - 1);
        if (rank == "A") values.push_back(14);
        else if (rank == "K") values.push_back(13);
        else if (rank == "Q") values.push_back(12);
        else if (rank == "J") values.push_back(11);
        else if (rank >= "2" && rank <= "10") values.push_back(stoi(rank)); // Chỉ chuyển đổi các giá trị hợp lệ
    }
    sort(values.begin(), values.end());
    int sanh = 0;
    int sanhthuong = 0;
    // Kiểm tra các giá trị có liên tiếp không
    for (size_t i = 1; i < values.size(); ++i) 
    {
        if (values[i] == values[i - 1] + 1) 
        {
            sanh++;
        }
        if (values[i] == 14)
        {
            sanhthuong++;
        }
    }
    if (sanh == 5)
    {
        if (sanhthuong > 0)
        {
            return 2; //Sảnh thượng
        }
        else
        {
            return 1; // Sảnh
        }
    }
    return 0;
}

int isFlush(const vector<string>& hand) {
    char suit = hand[0].back(); // Lấy bộ bài của lá đầu tiên
    for (const auto& card : hand) {
        if (card.back() != suit) return 0;
    }
    return 1; // Là thùng
}

int handValue(const vector<string>& hand) {
    map<string, int> rankCount;
    for (const auto& card : hand) {
        string rank = card.substr(0, card.size() - 1); // Lấy phần rank
        rankCount[rank]++;
    }

    bool hasFlush = isFlush(hand);
    bool hasStraight = isStraight(hand);
    int pairs = 0;
    bool hasThreeOfAKind = false;
    bool hasFourOfAKind = false;

    for (const auto& entry : rankCount) {
        if (entry.second == 2) pairs++;
        if (entry.second == 3) hasThreeOfAKind = true;
        if (entry.second == 4) hasFourOfAKind = true;
    }

    // Kiểm tra các hand
    if (hasFlush && (hasStraight == 2)) return 10;
    if (hasFlush && (hasStraight == 1)) return 9; // Thùng Phá Sảnh
    if (hasFourOfAKind) return 8; // Four of a Kind
    if (hasThreeOfAKind && pairs > 0) return 7; // Full House
    if (hasFlush) return 6; // Flush
    if (hasStraight) return 5; // Straight
    if (hasThreeOfAKind) return 4; // Three of a Kind
    if (pairs == 2) return 3; // Two Pair
    if (pairs == 1) return 2; // One Pair
    return 1; // High Card
}

void printHand(int n) {
    if (n == 1) cout << "High Card";
    else if (n == 2) cout << "One Pair";
    else if (n == 3) cout << "Two Pair";
    else if (n == 4) cout << "Three of a Kind";
    else if (n == 5) cout << "Straight";
    else if (n == 6) cout << "Flush";
    else if (n == 7) cout << "Full House";
    else if (n == 8) cout << "Four of a Kind";
    else if (n == 9) cout << "Straight Flush";
}

vector<int> determineWinner(const vector<int>& playerValues) {
    int maxValue = -1;
    vector<int> winners;

    for (size_t i = 0; i < playerValues.size(); ++i) {
        if (playerValues[i] > maxValue) {
            maxValue = playerValues[i];
            winners.clear();
            winners.push_back(i);
        } else if (playerValues[i] == maxValue) {
            winners.push_back(i);
        }
    }
    return winners;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Khởi tạo seed cho random

    cout << "Welcome to the game of Texas Hold'em Poker!!\n";

    // Chọn chế độ
    cout << "Select game mode (1. PvP / 2. PvE): ";
    int mode;
    entermode:
    cin >> mode;
    if (mode == 1)
    {
        cout << "---PvP---" << endl;
    }
    else if (mode == 2)
    {
        cout << "---PvE---" << endl;
    }
    else
    {
        cout << "Invalid mode. Please choose again: ";
        goto entermode;
    }
    cout << endl;

    // Nhập số lượng người chơi
    cout << "Enter the number of players (2-10 players): ";
    int numPlayers;
    enternumPlayers:
    cin >> numPlayers;
    if (numPlayers < 2 || numPlayers > 10)
    {
        cout << "Invalid number of players. Please choose again: ";
        goto enternumPlayers;
    }
    cin.ignore();
    cout << endl;

    // Chọn số vòng chơi
    cout << "Enter the number of rounds (1-10 rounds): ";
    int numRounds;
    enternumRounds:
    cin >> numRounds;
    if (numRounds < 1 || numRounds > 10)
    {
        cout << "Invalid number of rounds. Please choose again: ";
        goto enternumRounds;
    }
    cin.ignore();
    cout << endl;

    //PvP
    if (mode == 1) 
    {
        // Tạo tên người chơi
        vector<string> playerName(numPlayers);
        for (int i = 0; i < numPlayers; i++) {
            cout << "Please enter the name of the " << i+1 << " player: ";
            getline(cin, playerName[i]);
        }
        this_thread::sleep_for(chrono::milliseconds(250));
        cout << endl;

        map<int, int> playerScore;
        for (int i = 0; i < numPlayers; i++) {
            playerScore[i] = 0;
        }
        // Thực hiện chơi
        int current_round = 1;
        if (mode == 1)
        while (current_round <= numRounds) {
            cout << "---Round " << current_round << "---\n";
            // Tạo bộ bài
            vector<string> deck = createDeck();
            shuffleDeck(deck);

            // Rút bài cho các người chơi
            vector<vector<string>> playerHands(numPlayers);
            vector<int> playerValues(numPlayers);
            for (int i = 0; i < numPlayers; ++i) {
                playerHands[i] = drawCards(deck, 2); // Rút 2 lá bài tẩy
                cout << playerName[i][0] << " drew: ";
                    for (const auto& card : playerHands[i]) 
                    {
                        cout << card << " ";
                    }
                    cout << endl;
                    this_thread::sleep_for(chrono::milliseconds(250));
            }

            // Rút bài chung
            vector<string> communityCards = drawCards(deck, 5);
            cout << "Community Cards: ";
            for (const auto& card : communityCards) 
            {
                cout << card << " ";
            }
            cout << endl;
            this_thread::sleep_for(chrono::milliseconds(250));

            // Tính giá trị hand
            for (int i = 0; i < numPlayers; ++i) {
                vector<string> allCards = playerHands[i];
                allCards.insert(allCards.end(), communityCards.begin(), communityCards.end());
                vector<string> bestHand;
                int bestValue = 0;
                for (size_t j = 0; j < allCards.size(); ++j) {
                    for (size_t k = j + 1; k < allCards.size(); ++k) {
                        for (size_t l = k + 1; l < allCards.size(); ++l) {
                            for (size_t m = l + 1; m < allCards.size(); ++m) {
                                for (size_t n = m + 1; n < allCards.size(); ++n) {
                                    vector<string> hand = { allCards[j], allCards[k], allCards[l], allCards[m], allCards[n] };
                                    int value = handValue(hand);
                                    if (value > bestValue) {
                                        bestValue = value;
                                        bestHand = hand;
                                    }
                                }
                            }
                        }
                    }
                }
                cout << playerName[i] << "'s hand value is: " << bestValue << " (";
                printHand(bestValue);
                cout << ")" << endl;
                this_thread::sleep_for(chrono::milliseconds(250));
                playerValues[i] = bestValue;
            }

            // Xác định người thắng
            cout << "---Round result " << current_round << "---" << endl;
            cout << "Winner:\n";
            vector<int> result = determineWinner(playerValues);
            for (int i = 0; i < result.size(); i++) {
                cout << playerName[result[i]] << endl;
                playerScore[result[i]]++;
            }
            cout << endl;
            current_round++;
            this_thread::sleep_for(chrono::milliseconds(250));
        }
        cout << "---Game over---" << endl;
        // Kết quả chung cuộc
        // Tính điểm cao nhất
        int maxScore = playerScore[0];
        for (int i = 1; i < numPlayers; i++) {
            if (playerScore[i] > maxScore) {
                maxScore = playerScore[i];
            }
        }
        // Tìm ra người thắng cuộc
        cout << "The winner of the game is: ";
        for (int i = 0; i < numPlayers; i++) {
            if (playerScore[i] == maxScore) {
                cout << playerName[i] << ", ";
            }
        }
        cout << "with a total score of: " << maxScore << endl;
        cout << endl;

        // Bảng điểm
        cout << "---Leaderboard---" << endl;
        for (int i = 0; i < numPlayers; i++) {
            if (playerScore[i] < 2) {
                cout << playerName[i] << ": " << playerScore[i] << " score" << endl;
            } else {
                cout << playerName[i] << ": " << playerScore[i] << " scores" << endl;
            }
        }
    }
    else if (mode == 2)
    {
        //Tên người chơi
        vector<string> playerName2[numPlayers];
        cout << "Please enter the player's name: ";
        string name;
        getline(cin, name);
        playerName2[0].push_back(name);
        for (int i = 1; i < numPlayers; i++)
        {
            string May = "Bot ";
            string So = to_string(i);
            playerName2[i].push_back(May + So);
        }
        this_thread::sleep_for(chrono::milliseconds(250));
        cout << endl;

        map<int, int> playerScore2;
        for (int i = 0; i < numPlayers; i++) {
            playerScore2[i] = 0;
        }
        // Thực hiện chơi
        int current_round = 1;
        while (current_round <= numRounds) {
            cout << "---Round " << current_round << "---\n";
            // Tạo bộ bài
            vector<string> deck = createDeck();
            shuffleDeck(deck);

            // Rút bài cho các người chơi
            vector<vector<string>> playerHands2(numPlayers);
            vector<int> playerValues(numPlayers);
            for (int i = 0; i < numPlayers; ++i) {
                playerHands2[i] = drawCards(deck, 2); // Rút 2 lá bài tẩy
                cout << playerName2[i][0] << " drew: ";
                    for (const auto& card : playerHands2[i]) 
                    {
                        cout << card << " ";
                    }
                    cout << endl;
                    this_thread::sleep_for(chrono::milliseconds(250));
            }

            // Rút bài chung
            vector<string> communityCards = drawCards(deck, 5);
            cout << "Community Cards: ";
            for (const auto& card : communityCards) 
            {
                cout << card << " ";
            }
            cout << endl;
            this_thread::sleep_for(chrono::milliseconds(250));

            // Tính giá trị hand
            for (int i = 0; i < numPlayers; ++i) {
                vector<string> allCards = playerHands2[i];
                allCards.insert(allCards.end(), communityCards.begin(), communityCards.end());
                vector<string> bestHand;
                int bestValue = 0;
                for (size_t j = 0; j < allCards.size(); ++j) {
                    for (size_t k = j + 1; k < allCards.size(); ++k) {
                        for (size_t l = k + 1; l < allCards.size(); ++l) {
                            for (size_t m = l + 1; m < allCards.size(); ++m) {
                                for (size_t n = m + 1; n < allCards.size(); ++n) {
                                    vector<string> hand = { allCards[j], allCards[k], allCards[l], allCards[m], allCards[n] };
                                    int value = handValue(hand);
                                    if (value > bestValue) {
                                        bestValue = value;
                                        bestHand = hand;
                                    }
                                }
                            }
                        }
                    }
                }
                cout << playerName2[i][0] << "'s hand value is: " << bestValue << " (";
                printHand(bestValue);
                cout << ")" << endl;
                this_thread::sleep_for(chrono::milliseconds(250));
                playerValues[i] = bestValue;
            }

            // Xác định người thắng
            cout << "---Round result " << current_round << "---" << endl;
            cout << "Winner:\n";
            vector<int> result = determineWinner(playerValues);
            for (int i = 0; i < result.size(); i++) {
                cout << playerName2[result[i]][0] << endl;
                playerScore2[result[i]]++;
            }
            cout << endl;
            current_round++;
            this_thread::sleep_for(chrono::milliseconds(250));
        }
        cout << "---Game over---" << endl;
        // Kết quả chung cuộc
        // Tính điểm cao nhất
        int maxScore = playerScore2[0];
        for (int i = 1; i < numPlayers; i++) {
            if (playerScore2[i] > maxScore) {
                maxScore = playerScore2[i];
            }
        }
        // Tìm ra người thắng cuộc
        cout << "The winner of the game is: ";
        for (int i = 0; i < numPlayers; i++) {
            if (playerScore2[i] == maxScore) {
                cout << playerName2[i][0] << ", ";
            }
        }
        cout << "with a total score of: " << maxScore << endl;
        cout << endl;

        // Bảng điểm
        cout << "---Leaderboard---" << endl;
        for (int i = 0; i < numPlayers; i++) {
            if (playerScore2[i] < 2) {
                cout << playerName2[i][0] << ": " << playerScore2[i] << " score" << endl;
            } else {
                cout << playerName2[i][0] << ": " << playerScore2[i] << " scores" << endl;
            }
        }
    }
    return 0;
}