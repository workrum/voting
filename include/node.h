#ifndef NODE_H
#define NODE_H

#include <vector>

struct outEdge {
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

        void addEdge(unsigned int index, double weight) {
            outEdge newEdge;
            newEdge.neighbourIndex = index;
            newEdge.weight = weight;

            outEdges.push_back(newEdge);

            return;
        }

        std::vector<outEdge> getEdges() {
            return outEdges;
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
        std::vector<outEdge> outEdges;
};

#endif // NODE_H
