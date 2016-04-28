#ifndef SERVANTTEST_H
#define SERVANTTEST_H

#pragma once
#include "Servant.h"

class ServantTest : public Servant
{
    public:
        ServantTest(int as, Team t);

        // Straight Slash
        int activateNP1(vector<Servant *> defenders);
        // Side Slash
        int activateNP2(vector<Servant *> defenders);
        // Omni Slash
        int activateNP3(vector<Servant *> defenders);
};

#endif // SERVANTTEST_H
