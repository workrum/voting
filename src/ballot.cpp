#include "ballot.h"

ballot::ballot() {
    //ctor
}

ballot::~ballot() {
    //dtor
}

void ballot::setRank(candidate &A, unsigned int r) {
    //Check if A is already ranked and change it if so?

    ranking newRank;

    newRank.person = &A;
    newRank.position = r;

    rankedList.push_back(newRank);

    return;
}
std::vector<ranking> ballot::getRankedList() {
    return rankedList;
}

candidate * ballot::getHighestActive() {
    candidate * highestPerson;
    unsigned int highestPosition = rankedList.size() + 2;
    for (unsigned int i = 0 ; i < rankedList.size() ; i++) {
        if(rankedList[i].position < highestPosition && !(rankedList[i].person->isEliminated())) {
            highestPerson = rankedList[i].person;
            highestPosition = rankedList[i].position;
        }
    }
    return highestPerson;
}

bool ballot::prefAoverB(candidate * A, candidate * B) {

    int Arank = -1;
    int Brank = -1;

    for (unsigned int i = 0 ; i < rankedList.size() ; i++) {
        if(rankedList[i].person == A) {
            Arank = rankedList[i].position;
        }
        else if(rankedList[i].person == B){
            Brank = rankedList[i].position;
        }
    }

    if (Arank == -1 || Brank == -1) {
        throw "error in function prefAoverB";
    }

    return (Arank < Brank);
}

void ballot::print() {
    std::cout << std::endl;
    for (unsigned int i = 0 ; i < rankedList.size() ; i++) {
        std::cout << rankedList[i].person->getName() << ": " << rankedList[i].position << std::endl;
    }
}
