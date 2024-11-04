#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(0))); // Khởi tạo seed cho hàm rand

    vector<int> bullet;
    for (int i = 1; i < 7; i++) {
        bullet.push_back(i);
    }
    nhap:
    int bullet_position = rand() % bullet.size();
    int ans;
    cout << "Nhấn 1 để bắn ";
    cin >> ans;
    if (ans == 1)
    {
        int shot_position = rand() % bullet.size();
        if (shot_position == bullet_position)
        {
            cout << "Bạn đã trúng đạn và ra đi mãi mãi!\n";
        }
        else
        {
            cout << "Viên đạn rỗng" << endl;
            bullet.erase(bullet.begin() + shot_position);
        }
    }
}