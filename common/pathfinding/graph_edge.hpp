#ifndef GRAPH_EDGE_HPP
#define GRAPH_EDGE_HPP

#include <cstdint>

namespace Pathfinder {

template <class IdentifierType>
class Graph_Edge {
    private:
        IdentifierType identifier_start;
        IdentifierType identifier_destination;
        uint64_t cost;

    public:
        Graph_Edge() = default;
        ~Graph_Edge() = default;
        Graph_Edge(IdentifierType identifier_start,
            IdentifierType identifier_dest, int64_t cost) {
                this->identifier_start = identifier_start;
                this->identifier_destination = identifier_dest;
                this->cost = cost;
            };

        // Getters
        IdentifierType get_start();
        IdentifierType get_destination();
        uint64_t get_cost();
};

/**
 * Retrieve the name of the first node
 * 
 * @return Value which can be used to identify the node
*/
template <class IdentifierType>
IdentifierType Graph_Edge<IdentifierType>::get_start() {
    return this->identifier_start;
}

/**
 * Retrieve the name of the second node
 * 
 * @return Value which can be used to identify the node
*/
template <class IdentifierType>
IdentifierType Graph_Edge<IdentifierType>::get_destination() {
    return this->identifier_destination;
}

/**
 * Retrieve the cost of traveling from the first node to the second
 * 
 * @return Cost of travel
*/
template <class IdentifierType>
uint64_t Graph_Edge<IdentifierType>::get_cost() {
    return this->cost;
}

} // namespace Pathfinder

#endif