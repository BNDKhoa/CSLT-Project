#include <iostream>
#include <fstream>

using namespace std;

struct Player {
    string name;
    string id;
    int score;
    Player (){}
    Player( string _name, string _id, int _score)
    {
        name = _name;
        id = _id;
        score = _score;
    }
};

int main() 
{
    Player *player = new Player[3];
    fstream file;
    file.open("input.txt", ios::out);
    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return 1;
    }
    for (int i = 0; i < 3; i++) {
        cout << "Enter name: ";
        getline(cin, player[i].name);
        cout << "Enter ID: ";
        getline(cin, player[i].id);
        cout << "Enter score: ";
        cin >> player[i].score;
        cin.ignore();
    }
    for (int i = 0; i < 3; i++) {
        file << player[i].name << " ";
        file << player[i].id << " ";
        file << player[i].score << endl;
    }

    for (int i = 0; i < 3; i++)
    {
        cout << player[i].name << " " << player[i].id << " " << player[i].score << endl;
    }
    file.close();
    return 0;
}