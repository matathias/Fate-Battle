#ifndef SERVANTBERSERKERAXE_H
#define SERVANTBERSERKERAXE_H

#pragma once

#include "Servants/servantberserker.h"

class ServantBerserkerAxe : public ServantBerserker
{
public:
    ServantBerserkerAxe(int as, Team t, Logger *l);

    virtual bool isGodmindActive();
    virtual void turnUpdate();

    virtual int doAction(int actionNum, vector<Servant *> defenders);

    virtual int attack(vector<Servant *> defenders, bool counter);
    int extendGodmind();

    virtual void subHP(int hp, DamageType dt);
    virtual void setHP(int hp);

protected:
    bool isGodmindExtended();
    int godmindTurnsLeft();
    int deathCount;
    int deathCeiling;

    int numDamageFreeTurns;
};

#endif // SERVANTBERSERKERAXE_H
