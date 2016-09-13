#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <vector>
#include <QObject>

using TTimeSteps = std::vector<int>;

class TimeStep : public QObject
{
    Q_OBJECT

public:
    explicit TimeStep(QObject *parent = nullptr);

    static const int INDEX_BEG = -1;
    static const int INDEX_END = -2;

    static const int SECONDS_END = 0;


public:
    void setSteps(int jobSteps, int jobSeconds, int minBreakSeconds, int maxBreakSeconds);
    TTimeSteps::size_type getSize() const; //zwraca ilość kroków
    int getIndex() const; //zwraca sekundy przyszłego
    int getSeconds() const; //zwraca sekundy obecnego

    int next(); //zwraca sekundy następnego
    bool hasNext();
    bool isLast();
    bool isFirst();
    void reset();

    bool isJob() const;
    bool isBreakMin() const;
    bool isBreakMax() const;

private:
    TTimeSteps steps;
    int index;

signals:

public slots:
};

#endif // TIMESTEP_H
