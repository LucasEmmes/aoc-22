#include "../common/filereader.h"

#include <cmath>
#include <set>
#include <memory>

class Pos {
    public:
        int x = 0;
        int y = 0;
};

bool operator<(const Pos lhs, const Pos rhs) {
    return std::make_pair(lhs.x, lhs.y) > std::make_pair(rhs.x, rhs.y);
}

int main() {
    auto movement = aoc_read_input_singles({});

    std::shared_ptr<Pos> H;
    std::shared_ptr<Pos> T;
    std::vector<std::shared_ptr<Pos>> knots;
    for (size_t _ = 0; _ < 10; _++) knots.push_back(std::make_shared<Pos>());
    
    std::set<Pos> unique_positions_p1;
    std::set<Pos> unique_positions_p2;

    for (size_t i = 0; i < movement.size(); i+=2) {
        for (size_t j = 0; j < std::stoi(movement[i+1]); j++) {
            H = knots[0];
            switch (movement[i][0]) {
            case 'R': H->x++;
                break;
            case 'L': H->x--;
                break;
            case 'U': H->y++;
                break;
            case 'D': H->y--;
                break;
            default:
                break;
            }

            for (size_t k = 0; k < 9; k++) {
                H = knots[k];
                T = knots[k+1];
            
                int distance = std::abs(H->x - T->x) + std::abs(H->y - T->y);

                // Same row/col
                if (distance == 2 && (H->x == T->x || H->y == T->y)) {
                    T->x += (H->x - T->x) / 2;
                    T->y += (H->y - T->y) / 2;
                }
                // Need to move diagonally to keep up
                else if (distance >= 3) {
                    T->x += 1*(H->x > T->x) - 1*(H->x < T->x);
                    T->y += 1*(H->y > T->y) - 1*(H->y < T->y);
                }
                if (k==0) unique_positions_p1.insert(*T);
                else if (k==8) unique_positions_p2.insert(*T);
            }
        }
    }
    
    printf("%d\n", unique_positions_p1.size());
    printf("%d\n", unique_positions_p2.size());
}