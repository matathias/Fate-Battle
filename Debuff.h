
#include "Servant.h"
using namespace std;

class Debuff
{
    public:
        Debuff(string dN, string dD, Team t, Stat dS, int dA, int tR);
        string getDebuffName();
        string getDebuffDescrip();
        Team getTargetTeam();
        Stat getDebuffStat();
        int getDebuffAmount();
        int getTurnsRemaining();
        
        int decrementTurnsRemaining(); // Returns the number of turns remaining
    
    protected:
        string debuffName;
        string debuffDescrip;
        Team targetTeam;
        Stat debuffStat;
        int debuffAmount;
        int turnsRemaining;
}
