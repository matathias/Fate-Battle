#ifndef SERVANTRIDER_H
#define SERVANTRIDER_H

#pragma once

#include "Servant.h"

class ServantRider : public Servant
{
public:
    ServantRider(int as, Team t, Logger *l);

    virtual int attack(vector<Servant*> defenders, bool counter);
};

#endif // SERVANTRIDER_H
