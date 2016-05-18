#pragma once

#include "logger.h"


Logger::Logger()
{
    eventNum = 1;
    errorNum = 1;
}

vector<string> Logger::getEventLog()
{
    return eventLog;
}

vector<string> Logger::getErrorLog()
{
    return errorLog;
}

int Logger::getEventNum()
{
    return eventNum;
}

int Logger::getErrorNum()
{
    return errorNum;
}

void Logger::addToEventLog(string s)
{
    string num = "Event " + to_string(eventNum) + ": ";
    eventLog.push_back(num + s);
    eventNum++;
}

void Logger::addToErrorLog(string s)
{
    string num = "Error on Event " + to_string(eventNum) + ": ";
    errorLog.push_back(num + s);
    errorNum = eventNum;
}

void Logger::addEventStartTurn(string tn, string sn)
{
    addLineSeperator();
    string num = "Event " + to_string(eventNum) + ": " + tn + " " + sn +
                 ": TURN START";
    eventLog.push_back(num);
    eventNum++;
    addLineSeperator();
}

void Logger::addEventEndTurn(string tn, string sn)
{
    addLineSeperator();
    string num = "Event " + to_string(eventNum) + ": " + tn + " " + sn +
                 ": TURN END";
    eventLog.push_back(num);
    eventNum++;
    addLineSeperator();
}

void Logger::addLineSeperator()
{
    eventLog.push_back("--------------------------------------------------");
}
