#ifndef ELECTION_H
#define ELECTION_H

#include <ballot.h>
#include <candidate.h>
#include <vector>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

class election
{

    public:
        election();
        virtual ~election();
        void addCandidate(std::string candidateName);
        void addBallot(ballot newBallot);
        ballot newBallot();
        std::vector<candidate *> getResult(std::string method); //Will want to pass the counting system as a parameter. Might also want to return more than one candidate as a result
        std::vector<candidate> * getCandidateList(); //So that it can request and generate a ballot for input
        void closeNominations();
        void resetScores();
        std::vector<candidate *> getFPTPResult();
        std::vector<candidate *> getInstantRunoffResult();
        std::vector<candidate *> getSchulzeResult();
        std::vector<candidate *> getBucklinResult();
        std::vector<candidate *> getBordaResult();
        std::vector<candidate *> getRankedPairsResult();
        std::vector<candidate *> getHighestScorer();
        unsigned int getCandidateIndex(candidate * point);
    protected:
    private:

        struct rankedPair {
            unsigned int Aindex;
            unsigned int Bindex;
            unsigned int AoverB;
            unsigned int BoverA;

            bool operator < (const rankedPair& other) const {
                if( (AoverB < other.AoverB) || (AoverB == other.BoverA && other.BoverA <= BoverA) ) {
                    return true;
                }
                return false;
            }
        };

        std::vector<ballot> ballots;
        std::vector<candidate> candidates;
        bool isOpen;

        friend class boost::serialization::access;
        template <typename Archive>
        void serialize(Archive& ar, const unsigned version) {
            ar & isOpen;
            ar & candidates;
            ar & ballots;
        }
};

#endif // ELECTION_H
