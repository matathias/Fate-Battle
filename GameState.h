
#pragma once

#include "Servant.h"
#include "PlayField.h"

using namespace std;

class GameState
{
    public:
        // Initialize the playing field within this constructor. Can also sort
        // the servants into teams from the turnOrder list.
        GameState(vector<Servant*> tO, int l, int w);
        
        vector<Servant*> getTurnOrder();
        
        Servant* getNextServant();
        Servant* peekNextServant();
        
        vector<Servant*> getDead();
        void addDead(Servant* s);
        void removeDead(Servant* s);
        bool isServantDead(Servant* s); //Returns true if dead, false otherwise.
        
        vector<Servant*> getAlphaTeam();
        vector<Servant*> getOmegaTeam();
    
    protected:
        vector<Servant*> turnOrder; // Contains servant pointers in turn order.
                                    // When it is a servant's turn, they are
                                    // popped off the vector and then placed at
                                    // the back.
        vector<Servant*> dead; // Contains a list of dead servants. If a servant
                               // is in this list, then getNextServant and
                               // peekNextServant will skip them.
        
        vector<Servant*> alphaTeam;
        vector<Servant*> omegaTeam;
        
        PlayField* field;
}
