#include "consts.h"
#include "timememory.h"

TimeMemory::TimeMemory(QObject *parent) :
    QObject(parent),
    seconds(SECONDS_JOB)
{
}

void TimeMemory::setSeconds(int newSeconds)
{
    seconds = newSeconds;
    emitAll();
}

int TimeMemory::tick()
{
    seconds -= seconds > 0 ? 1 : 0;
    emitAll();

    return seconds;
}

THumanTime TimeMemory::getHumanTime()
{
    int hours = seconds / HOUR_IN_SECONDS;
    int hoursr = seconds % HOUR_IN_SECONDS;
    int minutes = hoursr / MINUTE_IN_SECONDS;
    int seconds = hoursr % MINUTE_IN_SECONDS;
    THumanTime tuplehms = std::make_tuple(hours, minutes, seconds);
    return tuplehms;
}

void TimeMemory::emitAll()
{
    THumanTime htime = getHumanTime();

    emit hoursChanged(std::get<0>(htime));
    emit minutesChanged(std::get<1>(htime));
    emit secondsChanged(std::get<2>(htime));
}

