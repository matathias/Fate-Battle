#ifndef SERVANTLANCERHALBERD_H
#define SERVANTLANCERHALBERD_H

#pragma once

#include "servantlancer.h"

class ServantLancerHalberd : public ServantLancer
{
public:
    ServantLancerHalberd(int as, Team t, Logger *l);

    virtual int attack(vector<Servant *> defenders, bool counter);

    virtual int activateNP1(vector<Servant *> defenders);
    virtual int activateNP2(vector<Servant *> defenders);
    virtual int activateNP3(vector<Servant *> defenders);

private:
    int piercingBlow(vector<Servant*> defenders, int damMult);
    int swingingLunge(vector<Servant*> defenders, int damMult);

    int piercingTip(Servant* defender);
    int piercingEdge(Servant* defender);
};

#endif // SERVANTLANCERHALBERD_H
