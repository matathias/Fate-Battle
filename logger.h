#ifndef LOGGER_H
#define LOGGER_H

#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

class Logger
{
public:
    Logger();

    vector<string> getEventLog();
    vector<string> getErrorLog();
    void addToEventLog(string s);
    void addToErrorLog(string s);
    int getEventNum();
    int getErrorNum();

    void closeFiles();

    void addEventStartTurn(string tn, string sn);
    void addEventEndTurn(string tn, string sn);

private:
    int eventNum;
    int errorNum;

    std::ofstream evFile;
    std::ofstream erFile;

    vector<string> eventLog;
    vector<string> errorLog;

    void addLineSeperator();
};

#endif // LOGGER_H
