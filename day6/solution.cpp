#include "../common/filereader.h"

bool is_start(char* buffer, int n) {
    uint32_t mask = 0;
    uint32_t newmask;
    for (size_t i = 0; i < n; i++) {
        newmask = mask | 1 << (buffer[i]-97);
        if (mask == newmask) return false;
        mask = newmask;
    }
    return true;
}

int main() {
    std::string line = aoc_read_lines_raw()[0];

    // Initiate buffers
    char buffer_p1[4], buffer_p2[14];
    int index_p1 = 4;
    int index_p2 = 14;
    for (size_t i = 0; i < 4; i++) buffer_p1[i] = line[i];
    for (size_t i = 0; i < 14; i++) buffer_p2[i] = line[i];
    
    // P1
    while (index_p1++) {
        if (is_start(buffer_p1, 4)) break;
        buffer_p1[index_p1%4] = line[index_p1];
    }
    printf("%d\n", index_p1);
    
    // P2
    while (index_p2++) {
        if (is_start(buffer_p2, 14)) break;
        buffer_p2[index_p2%14] = line[index_p2];
    }
    printf("%d\n", index_p2);
}