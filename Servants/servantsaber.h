#ifndef SERVANTSABER_H
#define SERVANTSABER_H

#pragma once
#include "Servant.h"

class ServantSaber : public Servant
{
public:
    ServantSaber(int as, Team t, Logger *l);

    int guardianKnight(vector<Servant*> defenders);
    int manaBurst(vector<Servant*> defenders);

    // The magnitude of the Charisma buff is equal to 5% of the Saber's STR and
    // DEF.
    Debuff* getCharisma();
    int getCharismaRange();

    virtual vector<int> getEvade();
    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);

private:
    bool guardianKnightActive();
    bool manaBurstActive();
};

#endif // SERVANTSABER_H
