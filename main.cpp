#include <iostream>
#include <array>
#include <memory>
#include <vector>
#include <random>
#include <thread>
#include <chrono>


class Board {
private:
    using array_2d = std::vector<std::vector<int>>;
    array_2d grid_cur;
    array_2d grid_prev;
    int width;
    int height;

    const void cleaScreen() const {
        std::cout << "\033[2J\033[1;1H";
    }

    const void print() const {
        cleaScreen();
        for (int ii = 0; ii < height; ii++) {
            for(int jj = 0; jj < width; jj++) {
                if (grid_cur[ii][jj])
                    std::cout << "\u2588";
                else
                    std::cout << " ";
            }
            std::cout << std::endl;
        }
    }

    void copyState() {
        for (int ii = 0; ii < height; ii++) {
            for(int jj = 0; jj < width; jj++) {
                grid_prev[ii][jj] = grid_cur[ii][jj];
            }
        }
    }

public:
    Board(int width, int height) : width(width), height(height) {
        grid_cur.resize(height);
        for (auto &xx: grid_cur) {
            xx.resize(width);
        }

        grid_prev.resize(height);
        for (auto &xx: grid_prev) {
            xx.resize(width);
        }
    }

    void random() {
        for (int yy = 0; yy < height; yy++) {
            for (int xx = 0; xx < width; xx++) {
                if (xx == 0 || yy == 0 || xx == width - 1 || yy == height - 1) {
                    grid_cur[yy][xx] = 0;
                    continue;
                }

                grid_cur[yy][xx] = std::round(static_cast<float>(std::rand()) / RAND_MAX);
            }
        }
    }

    void update() {
        copyState();
        for (int yy = 1; yy < height-1; yy++) {
            for (int xx = 1; xx < width-1; xx++) {
                // calculate neighbors for cell
                int neighbors = 0;
                for (int ii = -1; ii < 2; ii++) {
                    for (int jj = -1; jj < 2; jj++) {
                        if (ii == 0 && jj == 0) continue;
                        if (grid_prev[yy+ii][xx+jj])
                            neighbors++;
                    }
                }

                if (neighbors < 2 || neighbors > 3)
                    grid_cur[yy][xx] = 0;
                else if (neighbors == 3)
                    grid_cur[yy][xx] = 1;
            }
        }
    }

    void play() {
        while(true) {
            update();
            print();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
};

int main() {
    Board board(100,50);
    board.random();
    board.play();
    std::cout << "Hello World!" << std::endl;
}
