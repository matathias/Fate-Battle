#ifndef SERVANTBERSERKERFLAIL_H
#define SERVANTBERSERKERFLAIL_H

#pragma once

#include "Servants/servantberserker.h"

class ServantBerserkerFlail : public ServantBerserker
{
public:
    ServantBerserkerFlail(int as, Team t, Logger *l);

    virtual int attack(vector<Servant *> defenders, bool counter);

    virtual void subHP(int hp, DamageType dt);

    virtual vector<Coordinate> getActionRange(int action);
};

#endif // SERVANTBERSERKERFLAIL_H
