
#pragma once

#include "Servant.h"
#include "PlayField.h"
#include "logger.h"

#include <QObject>
#include <QWidget>
#include <QComboBox>

using namespace std;


class GameState : public QWidget
{
    Q_OBJECT

    public:
        // Initialize the playing field within this constructor. Can also sort
        // the servants into teams from the turnOrder list.
        GameState(vector<Servant*> tO, int l, int w, Logger *lo);
        
        vector<Servant*> getTurnOrder();
        
        Servant* getNextServant();
        Servant* peekNextServant();
        Servant* getCurrentServant();
        string getCurrentServantType();
        string getCurrentServantTeam();

        vector<Coordinate> getValidMoves(Servant* s, int moveRange);
        vector<Coordinate> getFullMoveRange(Servant* s, int moveRange);
        vector<Coordinate> getAdjacentSpaces(Coordinate c);

        // If c is not adjacent to any space in range and getClosest is false,
        // the returned coordinate will have x = y = -1.
        // If c is not adjacent to any space in range and getClosest is true,
        // the returned coordinate will be the coordinate in range that is
        // closest to c.
        // Otherwise, returns a coordinate in range adjacent to c.
        Coordinate getAdjacentInRange(Coordinate c, vector<Coordinate> range,
                                      bool getClosest);
        // Same as getAdjacentInRange, except this function picks a space that
        //  is r away from c.
        Coordinate getSpaceInRange(Coordinate c, vector<Coordinate> range,
                                   bool getClosest, int r);

        // Ensures that a registered click on the gameboard is meaningful and
        // valid. nextTurnState should be called from here.
        bool isValidAction(Coordinate c);

        // These functions help the gui to determine what color a space should
        // be
        Servant* isSpaceServant(int x, int y);
        bool isSpaceSelection(int x, int y);
        bool isSpaceRealityMarble(int x, int y);
        bool isSpaceDebuff(int x, int y);
        bool isSpaceMove(int x, int y);
        Team spaceDebuffTeam(int x, int y);

        // Returns the actionList material so it can be displayed.
        vector<string> getActionList();
        vector<ActionType> getActionListType();
        vector<int> getActionMPCosts();

        vector<Servant*> getDead();
        vector<Servant*> getTeamDead(Team t);
        vector<Servant*> getTeamAlive(Team t);
        // Returns NULL if there is no alive Servant of class c on team t.
        Servant* getTeamAliveServant(Team t, Class c);
        vector<Servant*> getOpposingTeamDead(Team t);
        vector<Servant*> getOpposingTeamAlive(Team t);
        // Returns a list of Servants who are Permadead.
        vector<Servant*> filterPermaDead(vector<Servant*> d);
        void addDead(Servant* s);
        void removeDead(Servant* s);
        bool isServantDead(Servant* s); //Returns true if dead, false otherwise.
        bool isTeamDead(Team t);
        
        vector<Servant*> getAlphaTeam();
        vector<Servant*> getOmegaTeam();
        vector<Servant*> getBossTeam();
        vector<Servant*> getEnemyTeam(Servant* s);
        vector<Servant*> getEnemyTeam(Team t);
        vector<Servant*> getAlliedTeam(Team t);

        /* Functions to track the turn state and help process everything. */
        int getTurnState();

        // These two functions return the turn state after they are done
        // processing.
        int nextTurnState(); // Returns 999 if it fails. Otherwise returns the
                             // result of the relevant turnState function.
        int prevTurnState(); // Returns 999 if it fails. Otherwise returns the
                             // result of the relevant turnState function.

        int endTurn();
        int endTurnProcess();

        int getFieldWidth();
        int getFieldLength();

        void setClickedX(int x);
        void setClickedY(int y);
        void setChosenAction(int a);

        // These functions return 0 if they finished successfully, and another
        // value otherwise.
        int turnStatePreTurn();      //Errors in range 1-10
                                        // 10: invalid turnState
        int turnStateMove();         //Errors in range 11-20
                                        // 20: invalid turnState
                                        // 11: invalid space selection
        int turnStateChoseAction();  //Errors in range 21-30
                                        // 30: invalid turnState
                                        // 21: invalid action choice
                                        // 22: player not have enough MP
                                        // 25: player decided to not choose action
        int turnStateChoseTargets(); //Errors in range 31-40
                                        // 40: invalid turnState
                                        // 31: invalid space selection
                                        // 32: AOE chosen direction isn't valid
                                        // 33: AOE chosen direction isn't valid
                                        // 34: invalid chosen Action type
                                        // (32 through 34 should never be
                                        //     reached, like, at all)
                                        // 35: Action canceled
        int turnStateApplyAction();  //Errors in range 41-50
                                        // 41: Invalid action (go back to
                                        //     chooseAction turn state)
                                        // 50: invalid turnState
        int turnStateExtraMove();    //Errors in range 51-60
                                        // 60: invalid turnState
                                        // 51: invalid space selection
        int turnStatePostTurn();     //Errors in range 61+
                                        // 70: invalid turnState
                                        // 1000: Alpha team is dead
                                        // 1001: Omega team is dead
                                        // 1002: Boss team is dead
                                 // Post Turn is also responsible for resetting
                                 // the turn state and getting the next Servant

        void resetTurnValues();

        // If num is less than 0, returns 0. Otherwise, returns num.
        int capZero(int num);

    private slots:
        void processDeathComboBox(int index);
    
    protected:
        vector<Servant*> turnOrder; // Contains servant pointers in turn order.
                                    // When it is a servant's turn, they are
                                    // popped off the vector and then placed at
                                    // the back.
        vector<Servant*> dead; // Contains a list of dead servants. If a servant
                               // is in this list, then getNextServant and
                               // peekNextServant will skip them.
        vector<Servant*> revived; // Contains a list of Servants that have been
                                  // revived in the current turn. This list is
                                  // cleared at the end of every turn.
        
        vector<Servant*> alphaTeam;
        vector<Servant*> omegaTeam;
        vector<Servant*> bossTeam;

        vector<Team> activeTeams;
        
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

        int clickedX;
        int clickedY;

        int deathComboBoxIndex;

        // The Servant who's turn it is right now and related data.
        Servant* currentServant;
        Coordinate servStart;
        Coordinate servEnd;
        int chosenAction;
        ActionType chosenActionType;
        Direction chosenDirection;
        vector<Servant*> chosenDefenders;
        int remainingMove;

        vector<Coordinate> selectionRange;
        vector<Coordinate> validMoves;

        vector<string> actionList;
        vector<ActionType> actionListTypes;
        vector<int> actionMPCosts;

        bool archerSecondTurn;
        bool ionioiSecondTurn;

        // Event logger
        Logger *log;
};
