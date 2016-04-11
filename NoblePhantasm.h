
#pragma once

#include "Servant.h"
#include "PlayField.h"

using namespace std;

class NoblePhantasm
{
    public:
        NoblePhantasm(string n, string d);
        string getName();
        string getDescrip();
        vector<Coordinate> getRange(int direction);
        
        // This pretty much has to be hard-coded for each individual noble
        // phantasm...
        // Will have to make a subclass for each NP
        void activate(Servant attacker, vector<Servant> defenders);
        
    protected:
        string name;
        string descrip;
        
        vector<Coordinate> range; // Coordinate range relative to the Servant's
                                  // position. Default direction is north.
}
