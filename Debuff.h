
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
        
        int decrementTurnsRemaining(); // Returns the number of turns remaining
        int setTurnsRemaining(int tr);
    
    protected:
        // If a Debuff represents a Territory then its name will simply be
        // "Territory". Likewise, if a Debuff represents a Reality Marble then
        // its name will simply be "Reality Marble".
        string debuffName;
        string debuffDescrip;
        Team targetTeam;
        vector<Stat> debuffStat;
        vector<int> debuffAmount;
        int turnsRemaining;
};
