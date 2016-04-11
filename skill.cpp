/* Class file for skills.
 *
 * Really, all this class is good for is storing skill names and descriptions
 * outside of Servants and Weapons. Stat bonuses from skills are automatically
 * applied to the Servant or the Weapon, and the active part of skills... will
 * do something elsewhere. Should figure that out, probably.
 */

#include "Skill.h"
#include <vector>

/********** Function Definitions **********/
// Constructor
Skill::Skill(string n, string d0, string d1, string d2, bool al, int alvl)
{
    name = n;

    description.push_back(d0);
    description.push_back(d1);
    description.push_back(d2);

    ascensionLocked = al;
    ascensionLvl = alvl;
}

//***** Manipulators *****
void Skill::setAscensionLvl(int alvl)
{
    ascensionLvl = alvl;
}

//***** Retrievers *****
string Skill::getSklName()
{
    return name;
}

string Skill::getSklDescrip()
{
    if (ascensionLocked)
        return description[0];
    else
        return description[ascensionLvl];
}

bool Skill::isAscensionLocked()
{
    return ascensionLocked;
}
