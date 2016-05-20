#ifndef BOSSMIKA_H
#define BOSSMIKA_H

#pragma once

#include "Servants/servantsaber.h"

class BossMika : public ServantSaber
{
public:
    BossMika(int as, Team t, Logger *l);

    int bloodThirst(vector<Servant*> defenders);

    virtual void subHP(int hp, DamageType dt);

    virtual int doAction(int actionNum, vector<Servant *> defenders);
    virtual vector<Coordinate> getActionRange(int action);
    virtual int isActionNP(int action);

    // Blade Gust
    virtual int activateNP1(vector<Servant *> defenders);
    // Blood Blast
    virtual int activateNP2(vector<Servant *> defenders);

private:
    bool bloodBlastActive();
};

#endif // BOSSMIKA_H
