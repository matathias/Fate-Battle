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
Skill::Skill(string n, string d, int alvl)
{
    name = n;

    description = d;

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
    return description;
}

int Skill::getAscensionLvl()
{
    return ascensionLvl;
}
