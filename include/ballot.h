#ifndef BALLOT_H
#define BALLOT_H

#include <iostream>
#include <candidate.h>
#include <vector>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

class candidate;

class ranking {
    public:
        unsigned int position;
        candidate * person;
    private:
        friend class boost::serialization::access;
        template <typename Archive>
        void serialize(Archive& ar, const unsigned version) {
            ar & position & person;
        }
};

class ballot {
    public:
        ballot();
        virtual ~ballot();
        bool prefAoverB(candidate * A, candidate * B); //out of two candidates, who is preferred
        void setRank(candidate &A, unsigned int r);
        std::vector<ranking> getRankedList();
        candidate * getHighestActive();
        void print();
    protected:
    private:
        std::vector<ranking> rankedList;

        friend class boost::serialization::access;
        template <typename Archive>
        void serialize(Archive& ar, const unsigned version) {
            ar & rankedList;
        }

};

#endif // BALLOT_H
