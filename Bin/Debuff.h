
#pragma once

#include "datatypes.h"
#include <string>
#include <vector>
using namespace std;

class Debuff
{
    public:
        Debuff(string dN, string dD, Team t, vector<Stat> dS, vector<int> dA,
               int tR);
        string getDebuffName();
        string getDebuffDescrip();
        Team getTargetTeam();
        vector<Stat> getDebuffStats();
        vector<int> getDebuffAmounts();
        int getTurnsRemaining();

        // Returns 0 if s is not in debuffStat
        int getDebuffAmount(Stat s);
        
        int decrementTurnsRemaining(); // Returns the number of turns remaining
        int setTurnsRemaining(int tr);
    
    protected:
        // If a Debuff represents a Territory then its name will simply be
        // "Territory". Likewise, if a Debuff represents a Reality Marble then
        // its name will simply be "Reality Marble".
        string debuffName;
        // If a debuff is a territory or reality marble, then the description
        // will be the debuff's actual name.
        string debuffDescrip;
        Team targetTeam;
        vector<Stat> debuffStat;
        // If the amount is positive then it's a buff; if it's negative then
        //  it's a debuff.
        vector<int> debuffAmount;
        int turnsRemaining;
};
