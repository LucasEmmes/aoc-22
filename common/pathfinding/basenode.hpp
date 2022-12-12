#ifndef BASE_NODE_HPP
#define BASE_NODE_HPP

#include "outgoing_edge.hpp"
#include <vector>

namespace Pathfinder {

class BaseNode {
    protected:
        uint64_t current_cost = INT64_MAX;
        uint64_t heuristic_value = UINT64_MAX;
        uint64_t best_estimate = UINT64_MAX;
        std::weak_ptr<BaseNode> best_parent = std::weak_ptr<BaseNode>();
        std::vector<Outgoing_Edge> edges;
        std::weak_ptr<BaseNode> self_ptr;

    public:
        BaseNode() = default;
        ~BaseNode() = default;

        // Getters
        uint64_t get_current_cost();
        uint64_t get_heuristic();
        uint64_t get_current_estimate();
        std::weak_ptr<BaseNode> get_best_parent();

        // Setters
        void set_current_cost(uint64_t new_cost);
        void set_heuristic(uint64_t heuristic);
        void set_current_estimate(uint64_t estimate);
        void set_best_parent(std::weak_ptr<BaseNode> new_parent);
        void set_as_start();
        void set_ptr(std::weak_ptr<BaseNode> self_ptr);

        // Other
        void reset();
        void add_edge(Outgoing_Edge edge);
        void update_edges(std::vector<std::shared_ptr<BaseNode>>& updated_nodes);
};

/**
 * Returns the current lowest known cost to reach this node
 * 
 * @return Lowest known cost to reach this node
*/
uint64_t BaseNode::get_current_cost() {
    return this->current_cost;    
}

/**
 * Returns the heuristic of this node
 * 
 * @return Estimated cost to reach goal from this node
*/
uint64_t BaseNode::get_current_estimate() {
    return this->best_estimate;    
}

/**
 * Returns the current lowest estimate of cost to reach goal via this ndoe
 * 
 * @return Lowest estimated cost to reach goal through this node
*/
uint64_t BaseNode::get_heuristic() {
    return this->heuristic_value;    
}

/**
 * Returns the node through which the shortest known
 * 
 * @return Weak pointer to best known parent to reach this node
*/
std::weak_ptr<BaseNode> BaseNode::get_best_parent() {
    return this->best_parent;
}

/**
 * Sets the current lowest known cost of the node
 *   
 * @param new_cost The new lowest known cost to reach this node
*/
void BaseNode::set_current_cost(uint64_t new_cost) {
    this->current_cost = new_cost;
}

/**
 * Sets the heuristic value of the node
 *   
 * @param new_cost The heuristic value of this node
*/
void BaseNode::set_heuristic(uint64_t heuristic) {
    this->heuristic_value = heuristic;
}

/**
 * Sets the current estimated cost of the node
 *   
 * @param new_cost The new estimated cost to reach this node
*/
void BaseNode::set_current_estimate(uint64_t estimate) {
    this->best_estimate = estimate;
}

/**
 * Sets the best known parent
 *   
 * @param new_parent New best known parent for this node 
*/
void BaseNode::set_best_parent(std::weak_ptr<BaseNode> new_parent) {
    this->best_parent = new_parent;
}

/**
 * Sets the current path cost to 0
*/
void BaseNode::set_as_start() {
    this->current_cost = 0;
}

/**
 * Sets a shared pointer to itself
 * 
 * @param self_ptr Shared pointer to the node. Made by Pathfinder::AStar
*/
void BaseNode::set_ptr(std::weak_ptr<BaseNode> self_ptr) {
    this->self_ptr = self_ptr;
}

/**
 * Resets all values of the node.  
 * Useful when re-using nodes for different searches.
*/
void BaseNode::reset() {
    this->current_cost = UINT64_MAX;
    this->heuristic_value = UINT64_MAX;
    this->best_estimate = UINT64_MAX;
    this->best_parent.reset();
}

/**
 * Adds an outgoing connection to this node
 * 
 * @param edge Edge coming out of this node
*/
void BaseNode::add_edge(Outgoing_Edge edge) {
    this->edges.push_back(edge);
}

/**
 * Updates the values of this node
 * 
 * @param updated_nodes A list of nodes which were updated and might need to
 * be added back into queue
*/
void BaseNode::update_edges(std::vector<std::shared_ptr<BaseNode>>& updated_nodes) {
    std::shared_ptr<BaseNode> node;
    uint64_t new_cost;
    for (Outgoing_Edge& edge : this->edges) {
        node = edge.get_destination();
        new_cost = this->current_cost + edge.get_cost();
        if (new_cost < node->get_current_cost()) {
            node->set_current_estimate(new_cost +
                edge.get_destination()->get_heuristic());
            node->set_current_cost(new_cost);
            node->set_best_parent(this->self_ptr);
            updated_nodes.push_back(node);
        }
    }
}

} // namespace Pathfinder

#endif