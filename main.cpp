#include <iostream>
#include <election.h>
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

bool createNewBallot(election &elect);

unsigned int charToInt(char c) {
    return c - 48;
}

int main()
{
    const std::string filename = "save.txt";
    election electionTest;

    std::string mode;
    getline(std::cin, mode);

    if (mode == "l" || mode == "load") {
        std::ifstream ifs(filename);
        boost::archive::text_iarchive ar(ifs);
        ar & electionTest;
    }

    else {
        bool check = true;
        while(check) {
            std::string newName;
            std::cout << "Enter candidate name: ";
            getline(std::cin, newName);

            if(newName == "-1") {
                check  = false;
            }
            else {
                electionTest.addCandidate(newName);
            }
        }

        electionTest.closeNominations();

        while(createNewBallot(electionTest));


        std::ofstream ofs(filename);
        {
            boost::archive::text_oarchive ar(ofs);
            ar & electionTest;
        }
    }

    std::vector<candidate *> winnerFPTP        = electionTest.getResult("FPTP");
    std::vector<candidate *> winnerIR          = electionTest.getResult("AV");
    std::vector<candidate *> winnerBorda       = electionTest.getResult("Borda");
    std::vector<candidate *> winnerBucklin     = electionTest.getResult("Bucklin");
    std::vector<candidate *> winnerRankedPairs = electionTest.getResult("Ranked Pairs");
    std::vector<candidate *> winnerSchulze     = electionTest.getResult("Schulze");

    std::cout << std::endl << "FPTP Winner:           ";for (unsigned int i = 0 ; i < winnerFPTP.size()        ; i++) {if (i > 0) {std::cout << ", ";} std::cout << winnerFPTP[i]->getName()       ;} std::cout << std::endl;
    std::cout <<              "Instant Runoff Winner: ";for (unsigned int i = 0 ; i < winnerIR.size()          ; i++) {if (i > 0) {std::cout << ", ";} std::cout << winnerIR[i]->getName()         ;} std::cout << std::endl;
    std::cout <<              "Borda Winner:          ";for (unsigned int i = 0 ; i < winnerBorda.size()       ; i++) {if (i > 0) {std::cout << ", ";} std::cout << winnerBorda[i]->getName()      ;} std::cout << std::endl;
    std::cout <<              "Bucklin Winner:        ";for (unsigned int i = 0 ; i < winnerBucklin.size()     ; i++) {if (i > 0) {std::cout << ", ";} std::cout << winnerBucklin[i]->getName()    ;} std::cout << std::endl;
    std::cout <<              "Ranked Pairs Winner:   ";for (unsigned int i = 0 ; i < winnerRankedPairs.size() ; i++) {if (i > 0) {std::cout << ", ";} std::cout << winnerRankedPairs[i]->getName();} std::cout << std::endl;
    std::cout <<              "Schulze Winner:        ";for (unsigned int i = 0 ; i < winnerSchulze.size()     ; i++) {if (i > 0) {std::cout << ", ";} std::cout << winnerSchulze[i]->getName()    ;} std::cout << std::endl;

    return 0;
}

bool createNewBallot(election &elect) {
    std::vector<candidate> * candidateList = elect.getCandidateList();

    for (unsigned int i = 0 ; i < (*candidateList).size() ; i++) {
        std::cout << i + 1 << ": " << (*candidateList)[i].getName() << std::endl;
    }

    //Shuffle the ballot and then unshuffle the ranked list

    ballot newBallot;

    for (unsigned int j = 0 ; j < (*candidateList).size() ; j++ ) {
        std::cout << "Who is ranked #" << j + 1 << "? ";

        std::string candidateNoStr;
        getline(std::cin, candidateNoStr);

        if(candidateNoStr == "-1") {
            return false;
        }

        unsigned int candidateNo = charToInt(candidateNoStr[0]);

        newBallot.setRank(((*candidateList)[candidateNo - 1]), j + 1);
    }

    elect.addBallot(newBallot);

    return true;
}
