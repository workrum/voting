#include "candidate.h"
#include <string.h>

candidate::candidate()
{
    score =  0;
    eliminated = false;
}

candidate::~candidate()
{
    //dtor
}

void candidate::setName(std::string newName)
{
    name = newName;
}

std::string candidate::getName()
{
    return name;
}

void candidate::addVote(ballot * vote) {
    votes.push_back(vote);
}

void candidate::clearVotes() {
    votes.clear();
    score = 0;
}

void candidate::setScore(double newScore) {
    score = newScore;
}

double candidate::getScore() {
    return score;
}

void candidate::increaseScore(double diff) {
    score += diff;
}

void candidate::eliminate() {
    eliminated = true;
}

void candidate::activate() {
    eliminated = false;
}

bool candidate::isEliminated() {
    return eliminated;
}

std::vector<ballot *> candidate::getVotes() {
    return votes;
}
