#ifndef FILEHANDLING_HPP
#define FILEHANDLING_HPP

#include "graph_edge.hpp"
#include <type_traits>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Pathfinder {

    // Make template functions such that it can automatically convert the csv-
    // file into certain types 
    template <class... T>
    constexpr bool always_false = false;

    template<class T>
    Graph_Edge<T> make_edge(const std::vector<std::string>& line);

    template<>
    Graph_Edge<std::string> make_edge(const std::vector<std::string>& line) {
        return Graph_Edge<std::string>(line[0], line[1], std::stoull(line[2]));
    }

    template<>
    Graph_Edge<uint64_t> make_edge(const std::vector<std::string>& line) {
        return Graph_Edge<uint64_t>(std::stoull(line[0]), std::stoull(line[1]),
            std::stoull(line[2]));
    }
    
    template<>
    Graph_Edge<int> make_edge(const std::vector<std::string>& line) {
        return Graph_Edge<int>(std::stoi(line[0]), std::stoi(line[1]),
            std::stoi(line[2]));
    }

    /**
     * Reads a csv file and translates the data into graph edges
     * 
     * @param filename The relative path of the csv file to be read
     * @return A vector of graph-edges
    */
    template<class IdentifierType>
    std::vector<Graph_Edge<IdentifierType>> read_edges_from_csv(
        const std::string& filename) {
        // Set up collector and delimeter
        std::vector<std::vector<std::string>> csv_contents;
        char delimiter = ',';
        // Start reading
        auto ss = std::ostringstream{};
        std::ifstream input_file(filename);
        if (!input_file.is_open()) {
            std::cerr << "Could not open the file - '" << filename << "'" << std::endl;
            exit(EXIT_FAILURE);
        }
        ss << input_file.rdbuf();
        std::string file_contents = ss.str();
        std::istringstream sstream(file_contents);
        std::vector<std::string> items;
        std::string record;
        // Read and then split one line at a time
        while (std::getline(sstream, record)) {
            std::istringstream line(record);
            while (std::getline(line, record, delimiter))
                items.push_back(record);
            csv_contents.push_back(items);
            items.clear();
        }
        // 
        std::vector<Graph_Edge<IdentifierType>> edges;
        for(const auto& line : csv_contents){
            edges.push_back(make_edge<IdentifierType>(line));
        }

        return edges;
    };

} // namespace Pathfinder

#endif