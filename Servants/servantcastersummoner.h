#ifndef SERVANTCASTERSUMMONER_H
#define SERVANTCASTERSUMMONER_H

#pragma once

#include "Servants/servantcaster.h"

class ServantCasterSummoner : public ServantCaster
{
public:
    ServantCasterSummoner(int as, Team t, Logger *l);

    int summonGolem();
    int itemConstruction();
    int territoryCreation(vector<Servant*> defenders);

    // Re-defined functions
    virtual int getTerritoryMP();
    virtual string getTerritoryName();

    virtual void subHP(int hp, DamageType dt);
    virtual int attack(vector<Servant *> defenders, bool counter);

    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);
    virtual int doAction(int actionNum, vector<Servant *> defenders);

    // Solar Rift
    virtual int activateNP1(vector<Servant *> defenders);
    // Black Hole Rift
    virtual int activateNP2(vector<Servant *> defenders);
    // Universal Rift
    virtual int activateNP3(vector<Servant *> defenders);

private:
    bool golemSummoned;
    int golemHP;
    int golemCost;

    int numMinorGates;
    int minorGateCost;
};

#endif // SERVANTCASTERSUMMONER_H
