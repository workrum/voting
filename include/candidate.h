#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string.h>
#include <ballot.h>
#include <vector>
#include <iostream>
#include <boost/serialization/base_object.hpp>

class ballot;

class candidate
{
    public:
        candidate();
        virtual ~candidate();
        void setName(std::string newName);
        std::string getName();
        double getScore();
        void addVote(ballot * vote);
        void clearVotes();
        void setScore(double newScore);
        void increaseScore(double diff);
        void eliminate();
        void activate();
        bool isEliminated();
        std::vector<ballot *> getVotes();
    protected:
    private:
        std::string name;
        std::vector<ballot *> votes;
        double score;
        bool eliminated;

        friend class boost::serialization::access;
        template <typename Archive>
        void serialize(Archive& ar, const unsigned version) {
            ar & name;
        }
};

#endif // CANDIDATE_H
