#include "consts.h"
#include "timestep.h"

const int TimeStep::INDEX_BEG;
const int TimeStep::INDEX_END;
const int TimeStep::SECONDS_END;

TimeStep::TimeStep(QObject *parent) :
    QObject(parent),
    steps({SECONDS_JOB, SECONDS_MIN_BREAK,
           SECONDS_JOB, SECONDS_MIN_BREAK,
           SECONDS_JOB, SECONDS_MIN_BREAK,
           SECONDS_JOB, SECONDS_MAX_BREAK}),
    index(TimeStep::INDEX_BEG)
{
}

void TimeStep::setSteps(int jobSteps, int jobSeconds, int minBreakSeconds, int maxBreakSeconds)
{
    if(jobSteps == 0)
        throw new std::underflow_error("Argument \"jobSteps\" allowed value is 1 or more");

    if(jobSeconds == 0)
        throw new std::underflow_error("Argument \"jobSeconds\" allowed value is 1 or more");

    if(minBreakSeconds == 0)
        throw new std::underflow_error("Argument \"minBreakSeconds\" allowed value is 1 or more");

    if(maxBreakSeconds == 0)
        throw new std::underflow_error("Argument \"maxBreakSeconds\" allowed value is 1 or more");

    steps.clear();
    steps.reserve(jobSteps*2);

    for(int i=0; i<jobSteps; ++i)
    {
        steps.push_back(jobSeconds);
        steps.push_back(minBreakSeconds);
    }
    steps.back() = maxBreakSeconds;

    reset();
}

TTimeSteps::size_type TimeStep::getSize() const
{
    return steps.size();
}

int TimeStep::getIndex() const
{
    return index;
}

int TimeStep::getSeconds() const
{
    if(index == TimeStep::INDEX_BEG)
        throw new std::underflow_error("Member \"index\" is at begin position");

    if(index == TimeStep::INDEX_END)
        throw new std::underflow_error("Member \"index\" is at end position");

    return steps[index];
}

int TimeStep::next()
{
    int size = steps.size();
    if(index < size)
        index += 1;
    else
        index = TimeStep::INDEX_END;

    return steps[index];
}

bool TimeStep::hasNext()
{
    return static_cast<std::vector<int>::size_type>(index+1) != steps.size();
}

bool TimeStep::isLast()
{
    return static_cast<std::vector<int>::size_type>(index+1) == steps.size();
}

bool TimeStep::isFirst()
{
    return index == 0;
}

void TimeStep::reset()
{
    index = TimeStep::INDEX_BEG;
}

bool TimeStep::isJob() const
{
    return index % 2 == 0 && static_cast<std::vector<int>::size_type>(index+1) != steps.size();
}

bool TimeStep::isBreakMin() const
{
    return index % 2 != 0 && static_cast<std::vector<int>::size_type>(index+1) != steps.size();
}

bool TimeStep::isBreakMax() const
{
    return static_cast<std::vector<int>::size_type>(index+1) == steps.size();
}
