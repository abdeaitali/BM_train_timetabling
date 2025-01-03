#ifndef NODE_H
#define NODE_H

#include <cassert>
#include <iostream>
#include <unordered_set>

using namespace std;



template <class N, typename C>
// Class that describes a node in the graph
class Node 
{
	public:
		//  static variables (mainly for ouputting control)
		static bool printInEdges;
		static bool printOutEdges;
		static bool printLabels;
		static bool printCosts;

	protected:

		// static variable (used to count number of instantiations)
		static int instances_;

		int id_; // node identifier (number)
		N label_; // node label (mainly string)
		unordered_map<const Node*, C> inEdges_; // incoming edges with their costs
		unordered_map<const Node*, C> outEdges_; // outgoing edges with their costs

	public:
		// constructor from node label
		Node(const N& label) :
			id_(instances_++),
			label_(label)
        {};

		// compare nodes instantiation order
        virtual bool operator < (const Node& node) const {
            return id() < node.id();
        }

		// compare nodes equality
        virtual bool operator == (const Node& o) const {
            return id_ == o.id_;
        }


        //  add incoming edge to the node with cost (default. 0)
        void addEdgeFrom(Node* parent, const C& cost = 0.0) 
        {
            if (parent == nullptr)
                return;

            inEdges_[parent] = cost;
        }

		//  add outgoing edge to the node with cost (default. 0)
        void addEdgeTo(Node* child, const C& cost = 0.0) 
        {
            if (child == nullptr)
                return;

            outEdges_[child] = cost;
        }


        // Check if the node has an edge as an incoming edge
        const bool hasEdgeFrom(const Node* key) const {
            return inEdges_.find(key) != inEdges_.end();
        }

		// Check if the node has an edge as an outgoing edge
        const bool hasEdgeTo(const Node* key) const {
            return outEdges_.find(key) != outEdges_.end();
        }

		// get the cost of the given incoming edge
        const C edgeFrom(const Node* key) const { 
            assert(hasEdgeFrom(key));
            return inEdges_.at(key); 
        }

		// get the cost of the given outgoing edge
        const C edgeTo(const Node* key) const { 
            assert(hasEdgeTo(key));
            return outEdges_.at(key); 
        }

		// get the node from the first outgoing edge
        const Node<N, C>* firstTo() const 
        {
            if (outEdges_.size() == 0)
                return nullptr;

            return outEdges_.begin()->first;
        }

		// get the label of the node from the first outgoing edge
        const N& firstLabelTo() const {
            assert (outEdges_.size() > 0);
            return outEdges_.begin()->first->label();
        }

		// get the node from the first incoming edge
        const Node<N, C>* firstFrom() const 
        {
            if (inEdges_.size() == 0)
                return nullptr;

            return inEdges_.begin()->first;
        }

		// get the label of the node from the first incoming edge
        const N& firstLabelFrom() const {
            assert (inEdges_.size() > 0);
            return inEdges_.begin()->first->label();
        }

		// get the id of the node
        const int id() const { return id_; }

		// get the label of the node
        const N& label() const { return label_; }

		// get all the incoming edges (const version)
        const unordered_map<const Node*, C>& inEdges() const { return inEdges_; }

		// get all the outgoing edges (const version)
        const unordered_map<const Node*, C>& outEdges() const { return outEdges_; }

		// get all the incoming edges 
        unordered_map<const Node*, C>& inEdges() { return inEdges_; }

		// get all the outgoing edges 
        unordered_map<const Node*, C>& outEdges() { return outEdges_; }


        //  friends (for outputting the node info) uses the static variables for verbosity
		friend ostream& operator << (ostream& out, const Node<N, C>& node)
        {
            out << node.id(); 

			if (Node<N, C>::printLabels)
                out << " (" << node.label() << ")";

			if (Node<N, C>::printInEdges)
            {
                size_t count = 0;  

                if (node.inEdges().size() > 0) 
                    out << " <- ";

                for (const auto child_cost : node.inEdges()) 
                {
                    const Node* child = child_cost.first;
                    const C& cost = child_cost.second;

                    out << child->id();
                    
					if (Node<N, C>::printLabels || Node<N, C>::printCosts)
                        out << " (";

					if (Node<N, C>::printLabels)
                        out << child->label() << ", ";

					if (Node<N, C>::printCosts)
                        out << "c=" << cost;

					if (Node<N, C>::printLabels || Node<N, C>::printCosts)
                        out << ")";                   

                    if (++count < node.inEdges().size()) 
                        out << ", ";
                }
            }

			if (Node<N, C>::printOutEdges)
            {
                if (node.outEdges().size() > 0) 
                    out << " -> ";

                size_t count = 0;

                for (const auto child_cost : node.outEdges()) 
                {
                    const Node* child = child_cost.first;
                    const C& cost = child_cost.second;

                    out << child->id();

					if (Node<N, C>::printLabels || Node<N, C>::printCosts)
                        out << " (";

					if (Node<N, C>::printLabels)
                        out << child->label() << ", ";

					if (Node<N, C>::printCosts)
                        out << "c=" << cost;

					if (Node<N, C>::printLabels || Node<N, C>::printCosts)
                        out << ")";    

                    if (++count < node.outEdges().size()) 
                        out << ", ";            
                }
            }
        
            return out;
        }

};


template <class N, typename C>
int Node<N, C>::instances_ = 0;

template <class N, typename C>
bool Node<N, C>::printInEdges = true;

template <class N, typename C>
bool Node<N, C>::printOutEdges = true;

template <class N, typename C>
bool Node<N, C>::printLabels = true;

template <class N, typename C>
bool Node<N, C>::printCosts = true;


typedef const Node<string, int> nodesi;


#endif