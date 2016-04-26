/* File for the Game
 *
 * Contains the functions that make the actual game run, from the starting
 * select screen to the gameplay itself.
 *
 */

#pragma once

#include "Game.h"

using namespace std;

GameState* game;
PlayField* field;

vector<Servant*> allServants;
vector<Servant*> activeServants;
vector<Servant*> teamA;
vector<Servant*> teamB;
Team teamAType;
Team teamBType;

// Initialize everything. Servants, Skills, etc.
void initialize()
{
    /*** Initialize all skills ***/
    //TODO: Actually do this

    /*** Initialize all servants ***/
    // Add each servant to the allServants list as we go
    //TODO: Actually do this
}

/* Allow the setup of the scenario: select which Servants are on each team,
 * what their ascension is, their location on the playing field, etc.
 */
void setup()
{
    /*** Make use of Qt to get a window or some shit ***/
    // One column that contains all possible players, two columns for the two
    // teams in the game, and two fields to determine the size of the playing
    // field.
    // TODO: gui programming

    /*** Alternatively: have option to load file that specifies field size and
     * what servants are on what team. Would then only need to make one gui
     * window for the game itself. After that is working, could go back and try
     * to make a setup screen.
     ***/

    /*** When the setup screen is done, process the input data ***/
    // Determine the turn order based on servant speed

    // Initialize the GameState with the turn order and playing field dimensions

    // Start the game
    playGame();
}

/* Main play loop. Keeps going through the Servants until one team is defeated.
 *
 * Returns the victorious team.
 */
Team playGame()
{
    /*** Continuously loop until an entire team is dead. ***/
    while (!game->isTeamDead(teamAType) && !game->isTeamDead(teamBType))
    {
        // --Get the current Servant

        // --Decrement the turns remaining on any Debuffs this Servant has cast.
        // Remove Debuffs as necessary. Territory and Reality Marble Debuffs
        // are checked later.

        // --Apply any relevant Debuffs, either that are on the Servant or the
        // field.

        // --Determine if there are any ongoing combat effects that belong to
        // this Servant (e.g. Reality Marbles or Caster Territories) and if the
        // player wants to continue them.

        // --Let the player move

        /*** Actions ***/
        // Get a list of player actions and have the player select one

        // -If the action is single-target, get the range and have the player
        // select a target from within that range.

        // -If the action is AOE, have the player determine the direction of
        // application and get all targets in that range.

        // --Apply damage or whatever to the targets.

        // --If the player is a Rider, allow them to use the rest of their
        // movement points.

        // --Check if the player's target location has any relevant debuffs and
        // apply them

        // --End the turn

        // --Check if anyone has died or revived and modify the death list in
        // the GameState accordingly
    }

    // Show the final screen
    //finalScreen();
}
