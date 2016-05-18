#ifndef SERVANTASSASSINDAGGER_H
#define SERVANTASSASSINDAGGER_H

#pragma once

#include "Servants/servantassassin.h"

class ServantAssassinDagger : public ServantAssassin
{
public:
    ServantAssassinDagger(int as, Team t, Logger *l);

    // Backslash!
    virtual int activateNP1(vector<Servant *> defenders);
    // Reach of Darkness
    virtual int activateNP2(vector<Servant *> defenders);
    // Daggerworks
    virtual int activateNP3(vector<Servant *> defenders);
};

#endif // SERVANTASSASSINDAGGER_H
