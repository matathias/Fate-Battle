#ifndef SERVANTCASTERNECROMANCY_H
#define SERVANTCASTERNECROMANCY_H

#pragma once

#include "Servants/servantcaster.h"

class ServantCasterNecromancy : public ServantCaster
{
public:
    ServantCasterNecromancy(int as, Team t, Logger *l);

    int necroally(vector<Servant*> defenders);
    int itemConstruction();
    int deathSeal(vector<Servant*> defenders);
    int territoryCreation(vector<Servant*> defenders);

    // Re-defined functions
    virtual int getTerritoryMP();
    virtual string getTerritoryName();
    virtual int getRealityMarbleMP();

    virtual int isActionNP(int action);
    virtual bool isActionRealityMarble(int action);
    virtual vector<Coordinate> getActionRange(int action);
    virtual int doAction(int actionNum, vector<Servant *> defenders);

    // Bane of Life
    virtual int activateNP1(vector<Servant *> defenders);
    // Harbinger of Death
    virtual int activateNP2(vector<Servant *> defenders);
    // Commander of Life and Death
    virtual int activateNP3(vector<Servant *> defenders);

private:
    bool deathSealCreated;
    int deathSealCost;
    int necroallyBase;
};

#endif // SERVANTCASTERNECROMANCY_H
