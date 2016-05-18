#ifndef SERVANTARCHERMINION_H
#define SERVANTARCHERMINION_H

#pragma once

#include "Servants/servantarcher.h"

class ServantArcherMinion : public ServantArcher
{
public:
    ServantArcherMinion(int as, Team t, Logger *l);

    virtual int doAction(int actionNum, vector<Servant *> defenders);
    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);
};

#endif // SERVANTARCHERMINION_H
