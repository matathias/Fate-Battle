#ifndef SERVANTCASTERELEMENTAL_H
#define SERVANTCASTERELEMENTAL_H

#pragma once

#include "Servants/servantcaster.h"

class ServantCasterElemental : public ServantCaster
{
public:
    ServantCasterElemental(int as, Team t, Logger *l);

    int cleansingFlame(vector<Servant*> defenders);
    int itemConstruction();
    int flameBlade(vector<Servant*> defenders);
    int territoryCreation(vector<Servant*> defenders);

    // Re-defined functions
    virtual int getTerritoryMP();
    virtual string getTerritoryName();

    virtual void addDebuff(Debuff* d);

    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);
    virtual int doAction(int actionNum, vector<Servant *> defenders);

    // Elemental Beam
    virtual int activateNP1(vector<Servant *> defenders);
    // Meteor Slam
    virtual int activateNP2(vector<Servant *> defenders);
    // Searing Maelstrom
    virtual int activateNP3(vector<Servant *> defenders);

private:
    bool flameBladeCreated;
};

#endif // SERVANTCASTERELEMENTAL_H
