#ifndef OUTGOING_EDGE_HPP
#define OUTGOING_EDGE_HPP

#include <memory>
#include <functional>
#include <cstdint>

namespace Pathfinder {

class BaseNode;

class Outgoing_Edge {
    private:
        uint64_t cost = UINT64_MAX;
        std::shared_ptr<BaseNode> destination;        

    public:
        Outgoing_Edge() = default;
        ~Outgoing_Edge() = default;
        Outgoing_Edge(uint64_t cost, std::shared_ptr<BaseNode> destination) :
            cost(cost), destination(destination) {};

        // Getters
        uint64_t get_cost();
        std::shared_ptr<BaseNode> get_destination();
};

/**
 * Retrieves the cost to traverse this edge
 * 
 * @return Cost of traversing this edge
*/
uint64_t Outgoing_Edge::get_cost() {
    return this->cost;
}

/**
 * Retrieves the destination of the edge
 * 
 * @return Pointer to the next node
*/
std::shared_ptr<BaseNode> Outgoing_Edge::get_destination() {
    return this->destination;
}

} // namespace Pathfinder

#endif