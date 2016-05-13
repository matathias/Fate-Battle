#ifndef SERVANTLANCER_H
#define SERVANTLANCER_H

#pragma once

#include "Servant.h"

class ServantLancer : public Servant
{
public:
    ServantLancer(int as, Team t, Logger *l);

    int manaBurst(vector<Servant*> defenders);

    virtual void subHP(int hp, DamageType dt);

    virtual int doAction(int actionNum, vector<Servant *> defenders);

    virtual vector<int> getEvade();
    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);

protected:
    bool manaBurstActive();
};

#endif // SERVANTLANCER_H
