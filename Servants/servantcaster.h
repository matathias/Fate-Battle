#ifndef SERVANTCASTER_H
#define SERVANTCASTER_H

#pragma once

#include "Servant.h"

class ServantCaster : public Servant
{
public:
    ServantCaster(int as, Team t, Logger *l);

    int heal(vector<Servant*> defenders);
    int healAll();
    int resurrect(vector<Servant*> defenders);
    int castFromLife();

    virtual int attack(vector<Servant *> defenders, bool counter);

    // These almost aren't worth it, since all Caster subclasses will have to
    //  redefine them again anyways...
    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);

    virtual bool isHealAction(int action);

private:
    int ressurectCount;
};

#endif // SERVANTCASTER_H
