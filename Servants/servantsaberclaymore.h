#ifndef SERVANTSABERCLAYMORE_H
#define SERVANTSABERCLAYMORE_H

# pragma once
# include "servantsaber.h"

class ServantSaberClaymore : public ServantSaber
{
public:
    ServantSaberClaymore(int as, Team t, Logger *l);

    virtual int doAction(int actionNum, vector<Servant *> defenders);
    virtual int isActionNP(int action);

    // Valone is a passive Noble Phantasm. Requires re-implementing subHP.
    virtual void subHP(int hp, DamageType dt);
    // Knights of the Square Table
    virtual int activateNP1(vector<Servant *> defenders);
    // X-Calibre
    virtual int activateNP2(vector<Servant *> defenders);
};

#endif // SERVANTSABERCLAYMORE_H
