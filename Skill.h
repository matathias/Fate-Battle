using namespace std;

class Skill
{
    public:
        Skill(string n, string d0, string d1, string d2, bool al, int alvl);
        void setAscensionLvl(int alvl);
        
        string getSklName();
        string getSklDescrip();

        bool isAscensionLocked();
        
    protected:
        string name;
        vector<string> description;
        bool ascensionLocked;
        int ascensionLvl; // If the skill is locked to a certain ascension, then
                          // this value is the ascension level at which the
                          // skill is unlocked. Otherwise this simply tracks the
                          // servant's ascension level in order to return the
                          // right description.
}
