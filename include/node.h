#ifndef NODE_H
#define NODE_H

#include <vector>

struct edge {
        unsigned int neighbourIndex;
        double weight;
};

template <class nodeType>
class node
{
    public:
        node() {
            //ctor
        }
        virtual ~node() {
            //dtor
        }

        void setValue(nodeType newValue) {
            value = newValue;
            return;
        }

        nodeType getValue() {
            return value;
        }

        void addOutEdge(unsigned int index, double weight) {
            edge newEdge;
            newEdge.neighbourIndex = index;
            newEdge.weight = weight;
            outEdges.push_back(newEdge);

            return;
        }

        void addInEdge(unsigned int index, double weight) {
            edge newEdge;
            newEdge.neighbourIndex = index;
            newEdge.weight = weight;
            inEdges.push_back(newEdge);

            return;
        }

        std::vector<edge> getOutEdges() {
            return outEdges;
        }

        std::vector<edge> getInEdges() {
            return inEdges;
        }

        unsigned int getIndex() {
            return index;
        }

        void setIndex(unsigned int i) {
            index = i;
        }

    protected:

    private:
        unsigned int index;
        nodeType value;
        std::vector<edge> outEdges;
        std::vector<edge> inEdges;
};

#endif // NODE_H
