/* Class file for Weapons.
 *
 * Contains the implementation for Weapon objects. Noble Phantasms are tied to
 * weapon and thus stored with the weapon. Weapon modifications to stats are
 * stored as 'Debuffs' since the Debuff class can handle both Stat and
 * magnitude.
 *
 */

#pragma once

#include "Weapon.h"

using namespace std;

/********** Function Definitions **********/
// Constructor
Weapon::Weapon(vector<Skill> skls, vector<NoblePhantasm> nps, vector<Debuff> mS,
               int mpC)
{
    skills = skls;
    noblePhantasms = nps;
    modStats = mS;
    mpCost = mpC;
}

/***** Retrievers *****/
vector<Skill> Weapon::getSkills()
{
    return skills;
}

vector<NoblePhantasm> Weapon::getNoblePhantasms()
{
    return noblePhantasms;
}

vector<Debuff> Weapon::getWeaponMods()
{
    return modStats;
}

int Weapon::getMPCost()
{
    return mpCost;
}
