/* Class file for Debuffs.
 *
 * Contains the implementation for all stat modifier objects. The class is
 * called 'Debuff' but some modifiers may have a positive impact instead
 * (in this case a positive impact would be represented by a negative value).
 *
 * 'Static' stat modifiers will have a negative turns remaining value, to
 * signify that they always apply. Otherwise, when a debuff's 'turns remaining'
 * value reaches 0, the debuff is removed.
 *
 */

#pragma once

#include "Debuff.h"
using namespace std;

/********** Function Definitions **********/
// Constructor
Debuff::Debuff(string dN, string dD, Team t, vector<Stat> dS, vector<int> dA,
               int tR)
{
    debuffName = dN;
    debuffDescrip = dD;
    targetTeam = t;
    debuffStat = dS;
    debuffAmount = dA;
    turnsRemaining = tR;
}

//***** Manipulators *****
int Debuff::decrementTurnsRemaining()
{
    turnsRemaining -= 1;
    return turnsRemaining;
}

int Debuff::setTurnsRemaining(int tr)
{
    turnsRemaining = tr;
    return turnsRemaining;
}

//***** Retrievers *****
string Debuff::getDebuffName()
{
    return debuffName;
}

string Debuff::getDebuffDescrip()
{
    return debuffDescrip;
}

Team Debuff::getTargetTeam()
{
    return targetTeam;
}

vector<Stat> Debuff::getDebuffStats()
{
    return debuffStat;
}

vector<int> Debuff::getDebuffAmounts()
{
    return debuffAmount;
}

int Debuff::getTurnsRemaining()
{
    return turnsRemaining;
}
