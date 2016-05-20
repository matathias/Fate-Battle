#ifndef BOSSRIN_H
#define BOSSRIN_H

#pragma once

#include "Servants/servantcaster.h"

class BossRin : public ServantCaster
{
public:
    BossRin(int as, Team t, Logger *l);

    int legReinforcement();

    // For Rin's Skill: My Willpower is A-Plus!
    int turnUpdate();

    // Re-defined functions
    virtual void setPlayField(PlayField *f);

    virtual int isActionNP(int action);
    virtual vector<Coordinate> getActionRange(int action);
    virtual int doAction(int actionNum, vector<Servant *> defenders);

    // Eins, Zwei, Drei
    virtual int activateNP1(vector<Servant *> defenders);
    // Gandr Shot
    virtual int activateNP2(vector<Servant *> defenders);
    // Azoth Sword
    virtual int activateNP3(vector<Servant *> defenders);

private:
    bool legReinforceCooldown();
    bool azothCooldown();
    int azothUseCount;
};

#endif // BOSSRIN_H
