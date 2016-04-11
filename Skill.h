using namespace std;

class Skill
{
    public:
        Skill(string n, string d, int alvl);
        void setAscensionLvl(int alvl);
        
        string getSklName();
        string getSklDescrip();
        int getAscensionLvl();
        
    protected:
        string name;
        vector description;
        int ascensionLvl;
}
