#include "../common/filereader.h"

int main() {
    auto lines = aoc_read_lines_raw();

    std::vector<std::vector<int8_t>> forest(lines.size(), std::vector<int8_t>(lines[0].length()));
    for (size_t i = 0; i < lines.size(); i++)
        for (size_t j = 0; j < lines[0].size(); j++)
            forest[i][j] = lines[i][j]-48;

    	// IF A SCNEIC SCORE IS 0 IT MEANS IT IS VISIBLE FROM OUTSID
	// 	// IF A SCNEIC SCORE IS 0 IT MEANS IT IS VISIBLE FROM OUTSIDEE
    
}
