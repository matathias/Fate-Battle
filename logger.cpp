#pragma once

#include "logger.h"


Logger::Logger()
{
    eventNum = 1;
    errorNum = 1;

    evFile.open("EventLog.txt");
    erFile.open("ErrorLog.txt");
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

    evFile << num << s << endl;

    eventNum++;
}

void Logger::addToErrorLog(string s)
{
    string num = "Error on Event " + to_string(eventNum) + ": ";
    errorLog.push_back(num + s);

    erFile << num << s << endl;

    errorNum = eventNum;
}

void Logger::addEventStartTurn(string tn, string sn)
{
    addLineSeperator();
    string num = "Event " + to_string(eventNum) + ": " + tn + " " + sn +
                 ": TURN START";
    eventLog.push_back(num);
    evFile << num << endl;
    addLineSeperator();


    eventNum++;
}

void Logger::addEventEndTurn(string tn, string sn)
{
    addLineSeperator();
    string num = "Event " + to_string(eventNum) + ": " + tn + " " + sn +
                 ": TURN END";
    eventLog.push_back(num);
    evFile << num << endl;
    addLineSeperator();


    eventNum++;
}

void Logger::addLineSeperator()
{
    eventLog.push_back("--------------------------------------------------");
    evFile << "--------------------------------------------------" << endl;
}

void Logger::closeFiles()
{
    evFile.close();
    erFile.close();
}
