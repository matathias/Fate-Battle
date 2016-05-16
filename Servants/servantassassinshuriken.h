#ifndef SERVANTASSASSINSHURIKEN_H
#define SERVANTASSASSINSHURIKEN_H

#pragma once

#include "Servants/servantassassin.h"

class ServantAssassinShuriken : public ServantAssassin
{
public:
    ServantAssassinShuriken(int as, Team t, Logger *l);

    // Shurikenfaire
    virtual int activateNP1(vector<Servant *> defenders);
    // Death from the Shadows
    virtual int activateNP2(vector<Servant *> defenders);
    // Shurikenworks
    virtual int activateNP3(vector<Servant *> defenders);
};

#endif // SERVANTASSASSINSHURIKEN_H
