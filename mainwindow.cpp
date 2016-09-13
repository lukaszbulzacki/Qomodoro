#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/qobject.h>
#include <QtDebug>
#include <QTimer>
#include <QTextStream>
#include <QMediaPlayer>
#include <QSystemTrayIcon>

#include "consts.h"
#include "timememory.h"
#include "timestep.h"

void initQLCDNumbers(MainWindow *self, TimeMemory *timeMemory)
{
    QLCDNumber* lcdHH = self->findChild<QLCDNumber*>("lcdHH");
    self->connect(
            timeMemory, static_cast<void (TimeMemory::*)(int)>(&TimeMemory::hoursChanged),
            lcdHH,      static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

    QLCDNumber* lcdMM = self->findChild<QLCDNumber*>("lcdMM");
    self->connect(
            timeMemory, static_cast<void (TimeMemory::*)(int)>(&TimeMemory::minutesChanged),
            lcdMM,      static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

    QLCDNumber* lcdSS = self->findChild<QLCDNumber*>("lcdSS");
    self->connect(
            timeMemory, static_cast<void (TimeMemory::*)(int)>(&TimeMemory::secondsChanged),
            lcdSS,      static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));
}

void initQSpinBoxes(MainWindow *self, TimeMemory *timeMemory)
{
    timeMemory = timeMemory; //żeby nie było logu warning w kompilatorze
    QSpinBox* spinBoxJobXSteps = self->findChild<QSpinBox*>("spinBoxJobXSteps");
    self->connect(
            spinBoxJobXSteps, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            self,             static_cast<void (MainWindow::*)(int)>(&MainWindow::setStepsChanged));

    QSpinBox* spinBoxJobMM = self->findChild<QSpinBox*>("spinBoxJobMM");
    self->connect(
            spinBoxJobMM, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            self,         static_cast<void (MainWindow::*)(int)>(&MainWindow::setStepsChanged));

    QSpinBox* spinBoxMinBreakMM = self->findChild<QSpinBox*>("spinBoxMinBreakMM");
    self->connect(
            spinBoxMinBreakMM, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            self,              static_cast<void (MainWindow::*)(int)>(&MainWindow::setStepsChanged));

    QSpinBox* spinBoxMaxBreakMM = self->findChild<QSpinBox*>("spinBoxMaxBreakMM");
    self->connect(
            spinBoxMaxBreakMM, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            self,              static_cast<void (MainWindow::*)(int)>(&MainWindow::setStepsChanged));

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this)),
    media(new QMediaPlayer),
    tray(new QSystemTrayIcon(this)),
    timeMemory(new TimeMemory(this)),
    timeStep(new TimeStep(this)),
    stepsChanged(false)
{
    ui->setupUi(this);

    connect(
        timer, &QTimer::timeout,
        this, &MainWindow::setTimerTick);

    QPushButton *pushButtonPlay = findChild<QPushButton*>("pushButtonPlay");
    connect(
        this, &MainWindow::setPushButtonPlayChanged,
        pushButtonPlay, &QPushButton::setChecked);

    QPushButton *pushButtonNext = findChild<QPushButton*>("pushButtonNext");
    connect(
        this, &MainWindow::setPushButtonNextDisabled,
        pushButtonNext, &QPushButton::setDisabled);

    QLabel *labelMinBreakXSteps = findChild<QLabel*>("labelMinBreakXSteps");
    connect(
        this, static_cast<void (MainWindow::*)(int)>(&MainWindow::setLabelMinBreakXSteps),
       labelMinBreakXSteps, static_cast<void (QLabel::*)(int)>(&QLabel::setNum));

    initQLCDNumbers(this, timeMemory);
    initQSpinBoxes(this, timeMemory);

    media->setMedia(QUrl(DEFAULT_AUDIO_MESSAGE));
    media->setVolume(50);

    makeHistory(DEFAULT_JOB_NUM);
    setHistoryPart(0);
    timeMemory->setSeconds(timeStep->next());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonPlay_toggled(bool checked)
{
    if(checked)
    {
        qDebug() << "Timer start";
        timer->start(SECOND_IN_TICKS);
    }
    else
    {
        qDebug() << "Timer stop";
        timer->stop();
    }
}

void MainWindow::setTimerTick()
{
    int seconds = timeMemory->tick();
    THumanTime htime = timeMemory->getHumanTime();

    if(seconds == 0) {
        const char *tryFormat = timeStep->isJob()
                ? DEFAULT_TRAY_MESSAGE[0]
                : (timeStep->isBreakMin()
                        ? DEFAULT_TRAY_MESSAGE[1]
                        : (timeStep->isBreakMax()
                                ? DEFAULT_TRAY_MESSAGE[2]
                                : DEFAULT_TRAY_UNDEFINED));
        QString tryMessage = QString::asprintf(tryFormat, timeStep->getIndex()+1, timeStep->getSize());
        tray->show();
        tray->showMessage(DEFAULT_TRAY_TITLE, tryMessage, QSystemTrayIcon::Information);
        emit setPushButtonPlayChanged(false);
        media->play();
    }


    qDebug() << "Timer tick " << std::get<0>(htime) << ":" << std::get<1>(htime) << ":" << std::get<2>(htime);
}

void MainWindow::setStepsChanged(int dummy)
{
    dummy = dummy; // żeby nie było warningów
    stepsChanged = true;
}


void MainWindow::on_pushButtonStop_clicked()
{
    qDebug() << "Timer reset current time";
    timeMemory->setSeconds(timeStep->getSeconds());
    emit setPushButtonPlayChanged(false);
}

void MainWindow::on_pushButtonNext_clicked()
{
    qDebug() << "Timer next time";

    int seconds = timeStep->next();
    timeMemory->setSeconds(seconds);
    timeMemory->emitAll();

    clearHistoryPart(false);
    setHistoryPart(timeStep->getIndex());

    emit setPushButtonPlayChanged(false);

    if(timeStep->isLast())
    {
        emit setPushButtonNextDisabled(true);
    }
}

THumanSteps MainWindow::getStepsFromGUI()
{
    QSpinBox* spinBoxJobXSteps  = findChild<QSpinBox*>("spinBoxJobXSteps");
    QSpinBox* spinBoxJobMM      = findChild<QSpinBox*>("spinBoxJobMM");
    QSpinBox* spinBoxMinBreakMM = findChild<QSpinBox*>("spinBoxMinBreakMM");
    QSpinBox* spinBoxMaxBreakMM = findChild<QSpinBox*>("spinBoxMaxBreakMM");
    THumanSteps tup = std::make_tuple(
                                spinBoxJobXSteps->value(),
                                spinBoxJobMM->value(),
                                spinBoxMinBreakMM->value(),
                                spinBoxMaxBreakMM->value());
    return tup;
}

void MainWindow::makeHistory(int jobParts)
{
    QHBoxLayout *historyLayout = findChild<QHBoxLayout*>("historyLayout");

    while(historyLayout->count() > 0)
        delete historyLayout->itemAt(0)->widget();

    int allParts = jobParts * 2;
    for(int part=0; part<allParts; part++)
    {
        bool isLast= part+1 == allParts;
        bool isJob = part % 2 == 0 && !isLast;
        bool isMin = part % 2 != 0 && !isLast;
        bool isMax = isLast;

        QString historyKidName;
        QTextStream(&historyKidName) << "historyKidNr" << part;

        Qt::GlobalColor historyColor = isJob
                ? Qt::blue : (isMin ? Qt::green : (isMax ? Qt::green : Qt::gray));

        QPalette historyPalette(palette());
        historyPalette.setColor(QPalette::Background, historyColor);
        historyPalette.setColor(QPalette::Foreground, Qt::black);

        QFrame *historyKid = new QFrame(this);
        historyKid->setPalette(historyPalette);
        historyKid->setObjectName(historyKidName);
        historyKid->setAutoFillBackground(false);
        historyKid->setFrameStyle(QFrame::NoFrame);

        int historyStretch = isJob
                ? 3 : (isMin ? 1 : (isMax ? 2 : 2));
        historyLayout->addWidget(historyKid, historyStretch);
    }
}

void MainWindow::setHistoryPart(int part)
{
    QString historyKidName;
    QTextStream(&historyKidName) << "historyKidNr" << part;
    QFrame *historyPart = findChild<QFrame*>(historyKidName);
    historyPart->setAutoFillBackground(true);
    historyPart->setFrameStyle(QFrame::Box);
}

void MainWindow::clearHistoryPart(bool hardClear)
{
    QHBoxLayout *historyLayout = findChild<QHBoxLayout*>("historyLayout");

    int kidsCount = historyLayout->count();
    qDebug() << "History kids num is " << kidsCount;
    for(int i=0; i<kidsCount; i++)
    {
        QLayoutItem *historyItem = historyLayout->itemAt(i);
        QWidget *historyWidget = historyItem->widget();
        QFrame *historyKid = dynamic_cast<QFrame*>(historyWidget);
        if(historyKid == nullptr) continue;

        historyKid->setFrameStyle(QFrame::NoFrame);
        if(hardClear)
            historyKid->setAutoFillBackground(false);
        qDebug() << "Clear history kid with name " << historyKid->objectName();
    }
}

void MainWindow::on_toolBox_currentChanged(int index)
{
    qDebug() << "Page changed with index " << index;

    if(stepsChanged == false) return;

    if(index == PAGE_POMODORO)
    {
        emit setPushButtonPlayChanged(false);
        emit setPushButtonNextDisabled(false);

        THumanSteps hsteps = getStepsFromGUI();
        int jobSteps = std::get<0>(hsteps);
        int jobSeconds = std::get<1>(hsteps) * MINUTE_IN_SECONDS;
        int minBreakSeconds = std::get<2>(hsteps) * MINUTE_IN_SECONDS;
        int maxBreakSeconds = std::get<3>(hsteps) * MINUTE_IN_SECONDS;
        timeStep->setSteps(jobSteps, jobSeconds, minBreakSeconds, maxBreakSeconds);
        timeMemory->setSeconds(timeStep->next());

        makeHistory(jobSteps);
        setHistoryPart(0);
    }
    //else if(index == PAGE_SETTINGS)
    //{
    //
    //}
}

void MainWindow::on_spinBoxJobXSteps_valueChanged(int arg1)
{
    emit setLabelMinBreakXSteps(arg1-1);
}

void MainWindow::on_pushButton_clicked()
{

    emit setPushButtonPlayChanged(false);
    emit setPushButtonNextDisabled(false);

    clearHistoryPart(true);
    setHistoryPart(0);

    timeStep->reset();
    timeMemory->setSeconds(timeStep->next());
}
