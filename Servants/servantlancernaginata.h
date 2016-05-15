#ifndef SERVANTLANCERNAGINATA_H
#define SERVANTLANCERNAGINATA_H

#pragma once
#include "servantlancer.h"

class ServantLancerNaginata : public ServantLancer
{
public:
    ServantLancerNaginata(int as, Team t, Logger *l);

    virtual vector<int> getEvade();

    virtual int activateNP1(vector<Servant *> defenders);
    virtual int activateNP2(vector<Servant *> defenders);
    virtual int activateNP3(vector<Servant *> defenders);

private:
    vector<Coordinate> getCoordRange(int range);

    int doDamage(Servant* defender, double damMult);
};

#endif // SERVANTLANCERNAGINATA_H
