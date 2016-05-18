#ifndef SERVANTLANCERLANCE_H
#define SERVANTLANCERLANCE_H

#pragma once

#include "servantlancer.h"

class PlayField;

class ServantLancerLance : public ServantLancer
{
public:
    ServantLancerLance(int as, Team t, Logger *l);

    virtual void setPlayField(PlayField *h);

    // Titanreach
    virtual int activateNP1(vector<Servant *> defenders);
    // Essence of Gae Bolg
    virtual int activateNP2(vector<Servant *> defenders);
    // Essence of Gae Bolg (Anti-Fortress)
    virtual int activateNP3(vector<Servant *> defenders);
};

#endif // SERVANTLANCERLANCE_H
