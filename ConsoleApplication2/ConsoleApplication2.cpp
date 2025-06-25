#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <chrono>
#include <random>
#include <mutex>
#include <iomanip>
#include <sstream>

using namespace std;

const double PI = 3.14159265358979323846;
mutex coutMutex;

const double V = 1.0;      
const int N = 3;           // N секунд
const double R = 5.0;      
const int DURATION = 10;   

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> angle_dist(0, 2 * PI);

struct Position {
    double x = 0.0;
    double y = 0.0;
};

string intToStr(int value) {
    stringstream ss;
    ss << value;
    return ss.str();
}

void student_thread(int id) {
    Position pos;
    double angle = angle_dist(gen);

    for (int t = 1; t <= DURATION; ++t) {
        if ((t - 1) % N == 0) {
            angle = angle_dist(gen);
        }

        pos.x += V * cos(angle);
        pos.y += V * sin(angle);

        {
            lock_guard<mutex> lock(coutMutex);
            cout << left << setw(12) << "Student " + intToStr(id)
                << setw(8) << "t=" + intToStr(t)
                << fixed << setprecision(2)
                << "x=" << setw(8) << pos.x
                << "y=" << pos.y << "\n";
        }

        this_thread::sleep_for(chrono::milliseconds(500)); 
    }
}

void studentka_thread(int id) {
    double angle = 0.0;
    Position pos;

    for (int t = 1; t <= DURATION; ++t) {
        angle += V / R;

        pos.x = R * cos(angle);
        pos.y = R * sin(angle);

        {
            lock_guard<mutex> lock(coutMutex);
            cout << left << setw(12) << "Studentka " + intToStr(id)
                << setw(8) << "t=" + intToStr(t)
                << fixed << setprecision(2)
                << "x=" << setw(8) << pos.x
                << "y=" << pos.y << "\n";
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main() {
    vector<thread> threads;

    cout << "===== Multithreaded Simulation Start =====\n\n";
    cout << left << setw(12) << "Object"
        << setw(8) << "Time"
        << setw(10) << "X"
        << "Y\n";
    cout << "---------------------------------------------\n";

    for (int i = 0; i < 2; ++i) {
        threads.emplace_back(student_thread, i + 1);
        threads.emplace_back(studentka_thread, i + 1);
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "\n===== Simulation Complete =====\n";
    return 0;
}
