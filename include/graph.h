#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include <vector>

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
            nodes[outNodeIndex].addEdge(inNodeIndex, weight); //also add to the inNode?
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
                for(unsigned int j = 0 ; j < nodes[i].getEdges().size() ; j++) {
                    answer[i][nodes[i].getEdges()[j].neighbourIndex]++;
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
    protected:

    private:
        std::vector<node<nodeType>> nodes;
        unsigned int maxIndex;
};

#endif // GRAPH_H
