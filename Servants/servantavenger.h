#ifndef SERVANTAVENGER_H
#define SERVANTAVENGER_H

#pragma once

#include "Servant.h"

class ServantAvenger : public Servant
{
public:
    ServantAvenger(int as, Team t, Logger *l);
    void setTeam(vector<Servant*> ts);

    virtual Debuff* finalRevenge();
    int carryingOnTheTorch(vector<Servant*> defenders);

    // Avenger's Rage be applied as a buff to all stats. Each time it's
    //  updated, remove the previous buff and add a new one with the new stats.
    //  Each one has an infinite apply time.
    void updateAvengersRage();

    virtual int attack(vector<Servant *> defenders, bool counter);

private:
    vector<Servant*> teamServants; // Includes the Avenger
    bool carriedTheTorch;
};

#endif // SERVANTAVENGER_H
