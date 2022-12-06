#include <iostream>
#include <fstream>

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
    std::ifstream file("input.txt");
    std::string line;
    std::getline(file, line);
    
    int vals[] = {4,14};
    for (size_t i = 0; i < 2; i++) {
        int n = vals[i];
        char buffer[n];
        int index = n;
        for (size_t j = 0; j < n; j++) buffer[j] = line[j];
        while (index++) {
            if (is_start(buffer, n)) break;
            buffer[index%n] = line[index];
        }
        printf("%d\n", index);
    }
}