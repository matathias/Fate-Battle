#ifndef SERVANTTEST_H
#define SERVANTTEST_H

#pragma once
#include "servantsaber.h"

class ServantTest : public ServantSaber
{
    public:
        ServantTest(int as, Team t, Logger *l);

        virtual int doAction(int actionNum, vector<Servant *> defenders);

        // Straight Slash
        virtual int activateNP1(vector<Servant *> defenders);
        // Side Slash
        virtual int activateNP2(vector<Servant *> defenders);
        // Omni Slash
        virtual int activateNP3(vector<Servant *> defenders);
};

#endif // SERVANTTEST_H
