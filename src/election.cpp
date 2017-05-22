#include "election.h"
#include <math.h>
#include "graph.h"

election::election() {
    isOpen = true;
}

election::~election() {
    //dtor
}

void election::closeNominations() {
    isOpen = false;
}

void election::addCandidate(std::string candidateName) {
    if(isOpen) {
        candidate newCandidate;
        newCandidate.setName(candidateName);

        candidates.push_back(newCandidate);
    }
}

void election::addBallot(ballot newBallot) {
    if(!isOpen) {
        ballots.push_back(newBallot);
    }
}

std::vector<candidate> * election::getCandidateList() {
    if(candidates.size() > 0)
        return &candidates;

    throw "Not enough candidates!";
}

std::vector<candidate *> election::getHighestScorer() {
    std::vector<candidate *> winnerSoFar;
    double largest = -1;
    for (unsigned int k = 0 ; k < candidates.size() ; k++) {
        if(candidates[k].getScore() > largest) {
            largest = candidates[k].getScore();
            winnerSoFar.clear();
            winnerSoFar.push_back(&candidates[k]);
        }
        else if (candidates[k].getScore() == largest){
            winnerSoFar.push_back(&candidates[k]);
        }
    }

    return winnerSoFar;
}

std::vector<candidate *> election::getBordaResult() {
    resetScores();
    for (unsigned int i = 0 ; i < ballots.size() ; i++) {
        std::vector<ranking> ranks = ballots[i].getRankedList();
        for (unsigned int j = 0 ; j < ranks.size() ; j++) {
            double score = ranks.size() - (ranks[j].position - 1);
            ranks[j].person->increaseScore(score);
        }
    }

    return getHighestScorer();
}

std::vector<candidate *> election::getBucklinResult() {
    resetScores();
    unsigned int neededVotes = ceil(((double)ballots.size()) / 2);

    unsigned int round = 1;
    while (true) {
        for (unsigned int j = 0 ; j < ballots.size() ; j++) {
            std::vector<ranking> ranks = ballots[j].getRankedList();

            bool check = true;
            for (unsigned int i = 0 ; check && i < ranks.size() ; i++ ) {
                if (ranks[i].position == round) {
                    check = false;
                    ranks[i].person->increaseScore(1);
                }
            }
        }

        std::vector<candidate *> highestScorer = getHighestScorer();
        if(highestScorer[0]->getScore() >= neededVotes) {
            return highestScorer;
        }
        round++;
    }
}

std::vector<candidate *> election::getFPTPResult() {
    resetScores();

    for (unsigned int i = 0 ; i < ballots.size() ; i++) {
        for (unsigned int j = 0 ; j < ballots[i].getRankedList().size() ; j++) {
            if((ballots[i].getRankedList())[j].position == 1) {
                (*((ballots[i].getRankedList())[j].person)).increaseScore(1);
            }
        }
    }

    return getHighestScorer();
}

std::vector<candidate *> election::getInstantRunoffResult() {
    resetScores();
    int neededVotes = ceil(((double)ballots.size()) / 2);
    //add first place votes
    for (unsigned int i = 0 ; i < ballots.size() ; i++) {
        ballots[i].getHighestActive()->addVote(&ballots[i]);
        ballots[i].getHighestActive()->increaseScore(1);
    }

    while (true) { //should hopefully always return
        //find the candidate with the largest number of votes

        int highestScore = -1;
        int lowestScore  = -1;
        std::vector<candidate *> highestPeople;
        candidate * lowestPerson;

        for (unsigned int  i = 0 ; i < candidates.size() ; i++) {
            if (candidates[i].getScore() > highestScore) {
                highestPeople.clear();
                highestPeople.push_back(&candidates[i]);
                highestScore = candidates[i].getScore();
            }
            else if (candidates[i].getScore() == highestScore){
                highestPeople.push_back(&candidates[i]);
            }
            if ((candidates[i].getScore() < lowestScore || lowestScore == -1) && !candidates[i].isEliminated() ) {
                lowestPerson = &candidates[i];
                lowestScore = lowestPerson->getScore();
            }
        }

        //if more than needed votes then return winner
        if(highestScore >= neededVotes) {
            return highestPeople;
        }

        //else eliminate lowest candidate
        lowestPerson->eliminate();
        lowestPerson->setScore(0);
        for (unsigned int i = 0 ; i < lowestPerson->getVotes().size() ; i++) {
            //redistribute these votes
            lowestPerson->getVotes()[i]->getHighestActive()->addVote(lowestPerson->getVotes()[i]);
            lowestPerson->getVotes()[i]->getHighestActive()->increaseScore(1);
        }
        lowestPerson->clearVotes();
    }
}

template <class itemValue>
struct linkedListItem {
    int nextItemIndex;
    unsigned int currentItemIndex;
    itemValue value;
};

std::vector<candidate *> election::getSchulzeResult() {
    resetScores();

    //find pairwise rankings as in ranked pairs
    std::vector<rankedPair> rankedPairs;
    for ( unsigned int i = 0 ; i < candidates.size() ; i++ ) {

        for (unsigned int j = i + 1 ; j < candidates.size() ; j++ ) {
            rankedPair thisPair;
            thisPair.Aindex = i;
            thisPair.Bindex = j;
            thisPair.AoverB = 0;
            thisPair.BoverA = 0;

            for (unsigned int k = 0 ; k < ballots.size() ; k++ ) {

                if(ballots[k].prefAoverB(&candidates[i], &candidates[j])) { //
                    thisPair.AoverB++;
                }
                else{
                    thisPair.BoverA++;
                }

            }

            //Make it so the preferred candidate is the first one
            if(thisPair.BoverA > thisPair.AoverB) {
                unsigned int tempRank = thisPair.AoverB;
                unsigned int tempCandidate = thisPair.Aindex;
                thisPair.AoverB = thisPair.BoverA;
                thisPair.BoverA = tempRank;
                thisPair.Aindex = thisPair.Bindex;
                thisPair.Bindex = tempCandidate;
            }
            rankedPairs.push_back(thisPair);
        }
    }

    //generate graph of these pairs
    //add nodes
    graph<candidate> schulzeGraph;
    for ( unsigned int i = 0 ; i < candidates.size() ; i++ ) {
        unsigned int j = schulzeGraph.addNode(candidates[i]);
        if (j != i ) {
            throw "j != i";
        }
    }
    //add edges
    for ( unsigned int k = 0 ; k < rankedPairs.size() ; k++ ) {
        if(rankedPairs[k].AoverB > rankedPairs[k].BoverA) {
            schulzeGraph.addEdge(rankedPairs[k].Aindex, rankedPairs[k].Bindex, rankedPairs[k].AoverB);
        }
        else if (rankedPairs[k].BoverA > rankedPairs[k].AoverB) {
            schulzeGraph.addEdge(rankedPairs[k].Bindex, rankedPairs[k].Aindex, rankedPairs[k].BoverA);
        }
        else { //equal
            schulzeGraph.addEdge(rankedPairs[k].Aindex, rankedPairs[k].Bindex, rankedPairs[k].AoverB);
            schulzeGraph.addEdge(rankedPairs[k].Bindex, rankedPairs[k].Aindex, rankedPairs[k].BoverA);
        }
    }

    std::vector<std::vector<unsigned int>> schulzeMatrix(candidates.size(), std::vector<unsigned int>(candidates.size(), 0));

    //for each candidate, find strongest path to each other candidate
    for ( unsigned int i = 0 ; i < schulzeGraph.getNodes().size() ; i++ ) {
        std::vector<unsigned int> dijkstraValues;
        std::vector<bool> indexesUsed;
        for ( unsigned int j = 0 ; j < schulzeGraph.getNodes().size() ; j++ ) {
            dijkstraValues.push_back(0);
            indexesUsed.push_back(false);
        }

        for ( unsigned int k = 0 ; k < (schulzeGraph.getNodes()[i]).getOutEdges().size() ; k++) {
            unsigned int neighbourIndex = schulzeGraph.getNodes()[i].getOutEdges()[k].neighbourIndex;
            unsigned int weight = (unsigned int)(schulzeGraph.getNodes()[i].getOutEdges()[k].weight);

            if (dijkstraValues[neighbourIndex] == 0 || weight > dijkstraValues[neighbourIndex]) {
                dijkstraValues[neighbourIndex] = dijkstraValues[i] + weight;
            }
        }
        indexesUsed[i] = true;

        bool carryOn = true;
        while (carryOn) {
            //get node with highest value
            bool anyUnused = false;
            unsigned int largestValue = 0;
            unsigned int largestIndex;
            for (unsigned int l = 0 ; l < indexesUsed.size() ; l++) {
                if ( (indexesUsed[l] == false) && (dijkstraValues[l] >= largestValue)) {
                    anyUnused = true;
                    largestValue = dijkstraValues[l];
                    largestIndex = l;
                }
            }
            if (anyUnused == false) {
                carryOn = false;
            }

            //use modified Dijkstra to find strongest path to each other node
            for (unsigned int k = 0 ; (carryOn) && (k < (schulzeGraph.getNodes()[largestIndex]).getOutEdges().size()) ; k++) {
                unsigned int neighbourIndex = schulzeGraph.getNodes()[largestIndex].getOutEdges()[k].neighbourIndex;
                unsigned int weight = (unsigned int)(schulzeGraph.getNodes()[largestIndex].getOutEdges()[k].weight);

                if ((indexesUsed[neighbourIndex] == false) && (dijkstraValues[neighbourIndex] == 0 || std::min(dijkstraValues[largestIndex], weight) > dijkstraValues[neighbourIndex])) {
                    dijkstraValues[neighbourIndex] = std::min(dijkstraValues[largestIndex], weight);
                }
            }
            indexesUsed[largestIndex] = true;

        }

        //populate the matrix
        for (unsigned int m = 0 ; m < dijkstraValues.size() ; m++ ){
            schulzeMatrix[i][m] = dijkstraValues[m];
        }
    }

    //form linked list of vectors of candidates where position in list is finishing position
    //compare each candidate with the linked list in order and insert them where needed
    int firstLinkedListItemIndex = -1;
    std::vector<linkedListItem<std::vector<unsigned int>>> linkedList;
    for (unsigned int i = 0 ; i < candidates.size() ; i++ ) {

        linkedListItem<std::vector<unsigned int>> newLinkedListItem; //make into a vector later
        std::vector<unsigned int> currentValue;
        currentValue.push_back(i);
        newLinkedListItem.value = currentValue;
        newLinkedListItem.currentItemIndex = i;
        newLinkedListItem.nextItemIndex = -1;

        int currentLinkedListItemIndex = firstLinkedListItemIndex;
        int previousLinkedListItemIndex = -1;

        bool inserted = false;
        while (!inserted) {
            //if the linked list is currently empty
            if (currentLinkedListItemIndex == -1) {
                firstLinkedListItemIndex = i;

                linkedList.push_back(newLinkedListItem);
                inserted = true;
            }
            else {
                //compare
                if (schulzeMatrix[i][linkedList[currentLinkedListItemIndex].value[0]] > schulzeMatrix[linkedList[currentLinkedListItemIndex].value[0]][i]) {
                    //insert the new item before the current one
                    if (previousLinkedListItemIndex == -1) {
                        firstLinkedListItemIndex = i;
                    }
                    else {
                        linkedList[previousLinkedListItemIndex].nextItemIndex = i;
                    }
                    newLinkedListItem.nextItemIndex = currentLinkedListItemIndex;
                    linkedList.push_back(newLinkedListItem);
                    inserted = true;
                }
                else if (schulzeMatrix[i][linkedList[currentLinkedListItemIndex].value[0]] < schulzeMatrix[linkedList[currentLinkedListItemIndex].value[0]][i]) {
                    //move on to the next one
                    previousLinkedListItemIndex = currentLinkedListItemIndex;

                    //check if the next one exists
                    if (linkedList[currentLinkedListItemIndex].nextItemIndex == -1) {
                        linkedList[currentLinkedListItemIndex].nextItemIndex = i;
                        linkedList.push_back(newLinkedListItem);
                        inserted = true;
                    }
                    else {
                        currentLinkedListItemIndex = linkedList[currentLinkedListItemIndex].nextItemIndex;
                    }
                }
                else { //equal
                    linkedList[currentLinkedListItemIndex].value.push_back(i);
                    inserted = true;
                }
            }
        }
    }

    std::vector<candidate *> winners;
    for ( unsigned int j = 0 ; j < linkedList[firstLinkedListItemIndex].value.size() ; j++ ) {
        winners.push_back(&candidates[linkedList[firstLinkedListItemIndex].value[j]]);
    }

    return winners;
}

struct greater {
    template<class T>
    bool operator()(T const &a, T const &b) const { return !(a < b); }
};

void printMatrix(std::vector<std::vector<int>> matrix) {
    std::cout << std::endl;
    for ( unsigned int i = 0 ; i < matrix.size() ; i++ ) {
        std::cout << std::endl;
        for ( unsigned int j = 0 ; j < matrix[i].size() ; j++ ) {
            std::cout << " " << matrix[i][j];
        }
    }
    std::cout << std::endl;
}

std::vector<candidate *> election::getRankedPairsResult() {

    std::vector<rankedPair> rankedPairs;
    std::vector<std::vector<int>> laplaceMatrix(candidates.size(), std::vector<int>(candidates.size(), 0));

    for (unsigned int i = 0 ; i < candidates.size() ; i++ ) {

        for (unsigned int j = i + 1 ; j < candidates.size() ; j++ ) {
            rankedPair thisPair;
            thisPair.Aindex = i;
            thisPair.Bindex = j;
            thisPair.AoverB = 0;
            thisPair.BoverA = 0;

            for (unsigned int k = 0 ; k < ballots.size() ; k++ ) {

                if(ballots[k].prefAoverB(&candidates[i], &candidates[j])) { //
                    thisPair.AoverB++;
                }
                else{
                    thisPair.BoverA++;
                }

            }

            //Make it so the preferred candidate is the first one
            if(thisPair.BoverA > thisPair.AoverB) {
                unsigned int tempRank = thisPair.AoverB;
                unsigned int tempCandidate = thisPair.Aindex;
                thisPair.AoverB = thisPair.BoverA;
                thisPair.BoverA = tempRank;
                thisPair.Aindex = thisPair.Bindex;
                thisPair.Bindex = tempCandidate;
            }
            rankedPairs.push_back(thisPair);
        }
    }

    graph<candidate> pairsGraph;
    for ( unsigned int i = 0 ; i < candidates.size() ; i++ ) {
        unsigned int j = pairsGraph.addNode(candidates[i]);
        if (j != i ) {
            throw "j != i";
        }
    }

    //"lock in" step
    //adds edges to the graph as long as it doesn't create a cycle
    for ( unsigned int j = 0 ; j < rankedPairs.size() ; j++ ) {
        graph<candidate> tempGraph = pairsGraph;
        tempGraph.addEdge(rankedPairs[j].Aindex, rankedPairs[j].Bindex, rankedPairs[j].AoverB);

        std::vector<unsigned int> emptyAncestors;

        if (!tempGraph.hasCycle( rankedPairs[j].Aindex, emptyAncestors)) {
            pairsGraph.addEdge(rankedPairs[j].Aindex, rankedPairs[j].Bindex, rankedPairs[j].AoverB);
        }
    }

    std::vector<candidate *> winners;

    //scan through graph for no incoming nodes
    for ( unsigned int i = 0 ; i < pairsGraph.getNodes().size() ; i++ ) {
        if ( pairsGraph.getNodes()[i].getInEdges().size() == 0 ) {
            winners.push_back(&(candidates[i]));
        }
    }

    return winners;
}

std::vector<candidate *> election::getResult(std::string method)
{
    if (method == "First Past The Post" || method == "FPTP")
        return getFPTPResult();
    else if (method == "Instant Runoff" || method == "AV")
        return getInstantRunoffResult();
    else if (method == "Schulze")
        return getSchulzeResult();
    else if (method == "Bucklin")
        return getBucklinResult();
    else if (method ==  "Borda")
        return getBordaResult();
    else if (method ==  "Ranked Pairs")
        return getRankedPairsResult();
    else
        return getFPTPResult();
}

void election::resetScores() {
    for (unsigned int i = 0 ; i < candidates.size() ; i++) {
        candidates[i].clearVotes();
    }
}
