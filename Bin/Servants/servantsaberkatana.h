#ifndef SERVANTSABERKATANA_H
#define SERVANTSABERKATANA_H

#pragma once
#include "servantsaber.h"

class ServantSaberKatana : public ServantSaber
{
public:
    ServantSaberKatana(int as, Team t, Logger *l);

    virtual int doAction(int actionNum, vector<Servant *> defenders);

    virtual int attack(vector<Servant *> defenders, bool counter);

    // Ten Strikes
    virtual int activateNP1(vector<Servant *> defenders);
    // Hundred Strikes
    virtual int activateNP2(vector<Servant *> defenders);
    // Thousand Strikes
    virtual int activateNP3(vector<Servant *> defenders);
};

#endif // SERVANTSABERKATANA_H
