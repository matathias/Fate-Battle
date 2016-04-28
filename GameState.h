
#pragma once

#include "Servant.h"
#include "PlayField.h"

using namespace std;

enum Direction {NORTH, SOUTH, EAST, WEST};

class GameState
{
    public:
        // Initialize the playing field within this constructor. Can also sort
        // the servants into teams from the turnOrder list.
        GameState(vector<Servant*> tO, int l, int w);
        
        vector<Servant*> getTurnOrder();
        
        Servant* getNextServant();
        Servant* peekNextServant();

        vector<Coordinate> getValidMoves(Servant* s);

        // Ensures that a registered click on the gameboard is meaningful and
        // valid. nextTurnState should be called from here.
        bool isValidAction(Coordinate c);

        vector<Servant*> getDead();
        void addDead(Servant* s);
        void removeDead(Servant* s);
        bool isServantDead(Servant* s); //Returns true if dead, false otherwise.
        bool isTeamDead(Team t);
        
        vector<Servant*> getAlphaTeam();
        vector<Servant*> getOmegaTeam();
        vector<Servant*> getBossTeam();

        /* Functions to track the turn state and help process everything. */
        int getTurnState();
        // These two functions return the turn state after they are done
        // processing.
        int nextTurnState();
        int prevTurnState();

        void setClickedX(int x);
        void setClickedY(int y);

        // These functions return 0 if they finished successfully, and another
        // value otherwise.
        int turnStatePreTurn();
        int turnStateMove();
        int turnStateChoseAction();
        int turnStateChoseTargets();
        int turnStateApplyAction();
        int turnStateExtraMove();
        int turnStatePostTurn(); // Post Turn is also responsible for resetting
                                 // the turn state and getting the next Servant
    
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
        vector<Servant*> bossTeam;
        
        PlayField* field;

        // Determines what part of a Servant's turn it is.
        // 0: Pre-turn processing (i.e. apply debuffs and such)
        // 1: Beginning of active turn. Servant has yet to move.
        // 2: Servant has moved and is now choosing an action.
        // 3: Servant has picked an action and is choosing how to apply action.
        // 4: Servant has picked a target/direction. Apply damage as relevant.
        // 5: If Servant is a Rider, they get to choose to move again here.
        //    Also, if they are an Archer, they have a chance to take another
        //    turn entirely.
        // 6: End of turn. Do all post-turn processing before getting next
        //    Servant and setting turnState back to 1.
        int turnState;
        Coordinate tSCoord;

        int clickedX;
        int clickedY;

        // The Servant who's turn it is right now and related data.
        Servant* currentServant;
        Coordinate servStart;
        Coordinate servEnd;
        int chosenAction;
        ActionType chosenActionType;
        Direction chosenDirection;
        int remainingMove;

        vector<Coordinate> selectionRange;

        vector<string> actionList;
        vector<ActionType> actionListTypes;
        vector<int> actionMPCosts;

        bool archerSecondTurn;
};
