
#pragma once

#include "Servant.h"
#include "PlayField.h"
#include "Debuff.h"
#include "Skill.h"
#include "NoblePhantasm.h"

using namespace std;

class Weapon
{
    public:
        Weapon(vector<Skill> skls, vector<NoblePhantasm> nps, vector<Debuff> mS,
               int mpC);

        vector<Skill> getSkills();
        vector<NoblePhantasm> getNoblePhantasms();
        vector<Debuff> getWeaponMods();

        int getMPCost();
        
    protected:
        vector<Skill> skills;
        vector<NoblePhantasm> noblePhantasms;
        
        // Track the stats that the weapon modifies as well as the magnitude by
        // which they are modified.
        vector<Debuff> modStats;
        
        int mpCost;
}
