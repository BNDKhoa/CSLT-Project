#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <map>
#include <thread>
#include <chrono>
#include <random>

using namespace std;

//Cờ bạc bịp
// vector<string> createDeck() {
//     vector<string> deck;
//     // Thêm các lá bài từ 2 đến 10
//     for (int i = 2; i <= 10; ++i) {
//         for (char suit : {'H', 'D', 'C', 'S'}) {
//             deck.push_back(to_string(i) + suit);
//         }
//     }
//     // Thêm các lá bài J, Q, K, A
//     for (char face : {'J', 'Q', 'K', 'A'}) {
//         for (char suit : {'H', 'D', 'C', 'S'}) {
//             deck.push_back(face + string(1, suit));
//         }
//     }
//     return deck;
// }

// // Hàm trộn bộ bài
// void shuffleDeck(vector<string>& deck) {
//     random_device rd;
//     mt19937 g(rd());
//     shuffle(deck.begin(), deck.end(), g);
// }

// // Hàm chia bài cho người chơi
// vector<string> drawCards(vector<string>& deck, int handSize) {
//     vector<string> hand(deck.end() - handSize, deck.end());
//     deck.erase(deck.end() - handSize, deck.end());
//     return hand;
// }

//Cờ bạc liêm chính
// Tạo bộ bài
vector<string> createDeck() {
    vector<string> deck;
    //Bài bình thường
    string suits[] = {"♠", "♥", "♦", "♣"};
    string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

    for (const auto& suit : suits) {
        for (const auto& rank : ranks) {
            deck.push_back(rank + suit);
        }
    }

    return deck;
}

// Rút bài
vector<string> drawCards(vector<string>& deck, int numCards) {
    vector<string> hand;
    for (int i = 0; i < numCards; ++i) {
        int index = rand() % deck.size();
        hand.push_back(deck[index]);
        deck.erase(deck.begin() + index); // Xóa lá bài đã rút khỏi bộ bài
    }
    return hand;
}

// Kiểm tra xem có Sảnh (Straight)
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
            return 1; //Sảnh
        }
        
    }
    return 0;
}

// Kiểm tra xem có Thùng (Flush)
int isFlush(const vector<string>& hand) {
    char suit = hand[0].back(); // Lấy bộ bài của lá đầu tiên
    for (const auto& card : hand) {
        if (card.back() != suit) return 0;
    }
    return 1;
}

// Tính giá trị của một hand
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
    if (hasFlush && (hasStraight == 2)) return 10; // Thùng phá sảnh thượng
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

//In ra loại hạng bài của người chơi
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
    else if (n == 10) cout << "Royal Flush";
}

// Xác định người thắng
vector<int> determineWinner(const vector<int>& playerValues) {
    int maxValue = -1;
    vector<int> winners;

    for (size_t i = 0; i < playerValues.size(); ++i) 
    {
        if (playerValues[i] > maxValue) 
        {
            maxValue = playerValues[i];
            winners.clear();
            winners.push_back(i);
        } 
        else if (playerValues[i] == maxValue) 
        {
            winners.push_back(i);
        }
    }
    return winners;
}

// Chương trình chính
int main() {
    srand(static_cast<unsigned int>(time(0))); // Khởi tạo seed cho random

    cout << "Chào mừng đến với trò chơi Stud Poker!\n";

    //Chọn chế độ
    cout << "Chọn chế độ chơi (1.PvP/2.PvE): ";
    inputmode:
    int mode;
    cin >> mode;
    if (mode == 1)
    {
        cout << "---Chế độ PvP---\n";
    }
    else if (mode == 2)
    {
        cout << "---Chế độ PvE---\n";
    }
    else if (mode < 1 || mode > 2)
    {
        cout << "Vui lòng chọn chế độ phù hợp: ";
        goto inputmode;
    }
    else if (cin.bad()) {
        cout << "Dữ liệu nhập vào không hợp lệ.\n";
        cout << "Vui lòng nhập lại: ";
        goto inputmode;
    }
    else if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Dữ liệu nhập vào không hợp lệ.\n";
        cout << "Vui lòng nhập lại: ";
        goto inputmode;
    }

    //Nhập số lượng người chơi
    cout << "Nhập số lượng người chơi (2-10 người): ";
    inputname:
    int numPlayers;
    cin >> numPlayers;
    cin.ignore();
    if (numPlayers < 2 || numPlayers > 10)
    {
        cout << "Số lượng người chơi phải trong khoảng từ 2 đến 9.\n";
        cout << "Vui lòng nhập lại: ";
        goto inputname;
    }
    else if (cin.bad())
    {
        cout << "Dữ liệu nhập vào không hợp lệ.\n";
        cout << "Vui lòng nhập lại: ";
        goto inputname;
    }
    else if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Dữ liệu nhập vào không hợp lệ.\n";
        cout << "Vui lòng nhập lại: ";
        goto inputname;
    }
    cout << endl;

    //Chọn số vòng chơi
    cout << "Nhập số vòng chơi (1-10 vòng): ";
    inputrounds:
    int numRounds;
    cin >> numRounds;
    cin.ignore();
    if (numRounds < 1 || numRounds > 10)
    {
        cout << "Số vòng chơi phải trong khoảng từ 1 đến 9.\n";
        cout << "Vui lòng nhập lại: ";
        goto inputrounds;
    }
    else if (cin.bad())
    {
        cout << "Dữ liệu nhập vào không hợp lệ.\n";
        cout << "Vui lòng nhập lại: ";
        goto inputrounds;
    }
    else if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Dữ liệu nhập vào không hợp lệ.\n";
        cout << "Vui lòng nhập lại: ";
        goto inputrounds;
    }
    int current_round = 1;
    cout << endl;

    //Chế độ PVP 
    if (mode == 1)
    {
        // Tạo tên người chơi
        vector<string> playerName[numPlayers];
        for (int i = 0; i < numPlayers; i++)
        {
            cout << "Mời nhập tên người chơi " << i+1 << ": ";
            string name;
            getline(cin, name);
            playerName[i].push_back(name);
        }
        this_thread::sleep_for(chrono::milliseconds(250));
        cout << endl;

        map<int, int> playerScore;
        for (int i = 0; i < numPlayers; i++)
        {
            playerScore[i] = 0;
        }

        // Thực hiện chơi
        while (current_round <= numRounds) {
            cout << "---Vòng chơi " << current_round << "---\n";
            // Tạo bộ bài
            vector<string> deck = createDeck();
            // Rút bài cho các người chơi
            vector<vector<string>> playerHands(numPlayers);
            vector<int> playerValues(numPlayers);
            for (int i = 0; i < numPlayers; ++i) 
            {
                playerHands[i] = drawCards(deck, 5);
                cout << "Người chơi " << playerName[i][0] << " rút được: ";
                for (const auto& card : playerHands[i]) 
                {
                    cout << card << " ";
                }
                cout << endl;
                this_thread::sleep_for(chrono::milliseconds(250));

                // Tính giá trị hand
                playerValues[i] = handValue(playerHands[i]);
            }

            cout << endl;
            // Hiển thị giá trị hand
            for (int i = 0; i < numPlayers; ++i) {
                cout << "Giá trị hand của người chơi " << playerName[i][0] << ": " << playerValues[i] << " (";
                printHand(playerValues[i]);
                cout << ")" << endl;
                this_thread::sleep_for(chrono::milliseconds(250));
            }
            cout << endl;

            // Xác định người thắng
            cout << "---Kết quả vòng " << current_round << "---" << endl;
            cout << "Người chơi thắng:\n";
            vector<int> result = determineWinner(playerValues);
            for (int i = 0; i < result.size(); i++)
            {
                cout << playerName[result[i]][0] << endl;
                playerScore[result[i]]++;
            }
            cout << endl;
            current_round++;
            this_thread::sleep_for(chrono::milliseconds(250));
        }
        cout << "---Kết thúc trò chơi---" << endl;
        // Kết quả chung cuộc
        // Tính điểm cao nhất
        int maxScore = playerScore[0];
        for (int i = 1; i < numPlayers; i++)
        {
            if (playerScore[i] > maxScore)
            {
                maxScore = playerScore[i];
            }
        }
        // Tìm ra người thắng cuộc
        cout << "Người chiến thắng là: ";
        for (int i = 0; i < numPlayers; i++)
        {
            if (playerScore[i] == maxScore)
            {
                cout << playerName[i][0] << ", ";
            }
        }
        cout << "với tổng số điểm là " << maxScore << endl;
        cout << endl;

        // Bảng điểm
        cout << "---Bảng điểm---" << endl;
        for (int i = 0; i < numPlayers; i++)
        {
            cout << playerName[i][0] << ": " << playerScore[i] << " điểm" << endl;
        }
    }
    //Chế độ PvE
    else if (mode == 2)
    {
        //Tên người chơi
        vector<string> playerName2[numPlayers];
        cout << "Mời nhập tên người chơi " << ": ";
        string name;
        getline(cin, name);
        playerName2[0].push_back(name);
        for (int i = 1; i < numPlayers; i++)
        {
            string May = "Máy ";
            string So = to_string(i);
            playerName2[i].push_back(May + So);
        }
        this_thread::sleep_for(chrono::milliseconds(250));
        cout << endl;

        map<int, int> playerScore2;
        for (int i = 0; i < numPlayers; i++)
        {
            playerScore2[i] = 0;
        }
        // Thực hiện chơi

        while (current_round <= numRounds) {
            cout << "---Vòng chơi " << current_round << "---\n";
            // Tạo bộ bài
            vector<string> deck = createDeck();
            // Rút bài cho các người chơi
            vector<vector<string>> playerHands2(numPlayers);
            vector<int> playerValues(numPlayers);
            for (int i = 0; i < numPlayers; ++i) 
            {
                playerHands2[i] = drawCards(deck, 5);
                cout << playerName2[i][0] << " rút được: ";
                for (const auto& card : playerHands2[i]) 
                {
                    cout << card << " ";
                }
                cout << endl;
                this_thread::sleep_for(chrono::milliseconds(250));

                // Tính giá trị hand
                playerValues[i] = handValue(playerHands2[i]);
            }

            cout << endl;
            // Hiển thị giá trị hand
            for (int i = 0; i < numPlayers; ++i) {
                cout << "Giá trị hand của " << playerName2[i][0] << ": " << playerValues[i] << " (";
                printHand(playerValues[i]);
                cout << ")" << endl;
                this_thread::sleep_for(chrono::milliseconds(250));
            }

            cout << endl;
            // Xác định người thắng
            cout << "---Kết quả vòng " << current_round << "---" << endl;
            cout << "Người chơi thắng:\n";
            vector<int> result = determineWinner(playerValues);
            for (int i = 0; i < result.size(); i++)
            {
                cout << playerName2[result[i]][0] << endl;
                playerScore2[result[i]]++;
            }
            cout << endl;
            current_round++;
            this_thread::sleep_for(chrono::milliseconds(250));
        }
        cout << "---Kết thúc trò chơi---" << endl;
        //Kết quả chung cuộc
        //Tính điểm cao nhất
        int maxScore2 = playerScore2[0];
        for (int i = 1; i < numPlayers; i++)
        {
            if (playerScore2[i] > maxScore2)
            {
                maxScore2 = playerScore2[i];
            }
        }
        //Tìm ra người thắng cuộc
        cout << "Người chiến thắng là: ";
        for (int i = 0; i < numPlayers; i++)
        {
            if (playerScore2[i] == maxScore2)
            {
                    cout << playerName2[i][0] << ", ";
            }
        }
        cout << "với tổng số điểm là " << maxScore2 << endl;
        cout << endl;

        //Bảng điểm
        cout << "---Bảng điểm---" << endl;
        for (int i = 0; i < numPlayers; i++)
        {
            cout << playerName2[i][0] << ": " << playerScore2[i] << " điểm" << endl;
        }
    }
    return 0;
}