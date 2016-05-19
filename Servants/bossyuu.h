#ifndef BOSSYUU_H
#define BOSSYUU_H

#pragma once

#include "Servants/servantsaber.h"

class BossYuu : public ServantSaber
{
public:
    BossYuu(int as, Team t, Logger *l);

    int druggingUp();

    virtual bool isBerserk();

    virtual int doAction(int actionNum, vector<Servant *> defenders);
    virtual vector<Coordinate> getActionRange(int action);
    virtual int isActionNP(int action);

    virtual int attack(vector<Servant *> defenders, bool counter);

    // Asuramaru
    virtual int activateNP1(vector<Servant *> defenders);
    // Asura-Kannon
    virtual int activateNP2(vector<Servant *> defenders);
    // Seraph of the End
    virtual int activateNP3(vector<Servant *> defenders);

private:
    bool asuramaruCooldown();
    bool asuraKannonCooldown();
    bool seraphOfTheEndCooldown();
    bool druggedUp();
};

#endif // BOSSYUU_H
