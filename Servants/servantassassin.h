#ifndef SERVANTASSASSIN_H
#define SERVANTASSASSIN_H

#pragma once

#include "Servant.h"

class ServantAssassin : public Servant
{
public:
    ServantAssassin(int as, Team t, Logger *l);

    int poisonStrike(vector<Servant*> defenders);
    int presenceConcealment(vector<Servant*> defenders);

    virtual int attack(vector<Servant *> defenders, bool counter);

    virtual vector<int> getEvade();
    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);

    virtual bool isPoisonAction(int action);

    virtual int doAction(int actionNum, vector<Servant *> defenders);

protected:
    Debuff *classDebuff;
    bool presenceConcealed();
};

#endif // SERVANTASSASSIN_H
