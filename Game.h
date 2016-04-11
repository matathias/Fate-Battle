
#pragma once

#include "Servant.h"
#include "GameState.h"

using namespace std;

/* Initialize everything. Servants, Weapons, Skills, Noble Phantasms, etc. */
void initialize(); 

/* Allow the setup of the scenario: select which Servants are on each team,
 * what their ascension is, their location on the playing field, etc.
 */
void setup();
// sub-methods to assist with setup
void selectServants();
void placeOnField();

/* Main play loop. Keeps going through the Servants until one team is defeated.
 *
 * Returns the victorious team.
 */
Team playGame();
// sub-methods to assist with playGame
void moveUnit();
void selectAction();
void checkDead();

/* Displays end results and statistics... if we keep track of that. */
void finalScreen();
