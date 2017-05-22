#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include <vector>
#include <algorithm>

template <class nodeType>
class graph {
    public:
        graph() {
            maxIndex = -1;
        }
        virtual ~graph() {
            //dctor
        };

        unsigned int addNode(nodeType value) {
            node<nodeType> newNode;
            maxIndex++;
            newNode.setIndex(maxIndex);
            newNode.setValue(value);
            nodes.push_back(newNode);

            return maxIndex;
        }

        void addEdge(unsigned int outNodeIndex, unsigned int inNodeIndex, double weight) {
            nodes[outNodeIndex].addOutEdge(inNodeIndex, weight);
            nodes[inNodeIndex].addInEdge(outNodeIndex, weight);
            return;
        }

        std::vector<node<nodeType>> getNodes() {
            return nodes;
        }

        unsigned int getMaxIndex() {
            return maxIndex;
        }

        std::vector<std::vector<unsigned int>> getAdjacencyMatrix() {
            std::vector<std::vector<unsigned int>> answer(nodes.size(), std::vector<unsigned int>(nodes.size(), 0));

            for (unsigned int i = 0 ; i <nodes.size() ; i++) {
                for(unsigned int j = 0 ; j < nodes[i].getOutEdges().size() ; j++) {
                    answer[i][nodes[i].getOutEdges()[j].neighbourIndex]++;
                }

            }
            return answer;
        }

        void printAdjacencyMatrix() {
            std::cout << std::endl;

            std::vector<std::vector<unsigned int>> matrix = getAdjacencyMatrix();
            for(unsigned int i = 0 ; i < matrix.size() ; i++ ) {
                std::cout << std::endl;

                for(unsigned int j = 0 ; j < matrix.size() ; j++ ) {
                    std::cout << " " << matrix[i][j];
                }
            }

            std::cout << std::endl;
        }


        bool hasCycle(unsigned int current, std::vector<unsigned int> ancestors) {
            //dfs to find if the graph has a cycle

            ancestors.push_back(current);

            for ( unsigned int i = 0 ; i < nodes[current].getOutEdges().size() ; i++ ) {
                if ( std::find(ancestors.begin(), ancestors.end(), nodes[current].getOutEdges()[i].neighbourIndex) != ancestors.end() ) { //if getOutEdges()[i] is an ancestor
                    return true;
                }
                if(hasCycle(nodes[current].getOutEdges()[i].neighbourIndex, ancestors)) {
                    return true;
                }
            }
            return false;
        }
    protected:

    private:
        std::vector<node<nodeType>> nodes;
        unsigned int maxIndex;
};

#endif // GRAPH_H
