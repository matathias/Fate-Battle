#ifndef SERVANTCASTERMINION_H
#define SERVANTCASTERMINION_H

#pragma once

#include "Servants/servantcaster.h"

class ServantCasterMinion : public ServantCaster
{
public:
    ServantCasterMinion(int as, Team t, Logger *l);

    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);
    virtual int doAction(int actionNum, vector<Servant *> defenders);
};

#endif // SERVANTCASTERMINION_H
