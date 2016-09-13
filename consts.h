#ifndef CONSTS_H
#define CONSTS_H

const int SECOND_IN_TICKS = 1000;
const int MINUTE_IN_SECONDS = 60;
const int HOUR_IN_SECONDS = MINUTE_IN_SECONDS*MINUTE_IN_SECONDS;

const int SECONDS_JOB = 25 * MINUTE_IN_SECONDS;
const int SECONDS_MIN_BREAK = 5 * MINUTE_IN_SECONDS;
const int SECONDS_MAX_BREAK = 15 * MINUTE_IN_SECONDS;

const int PAGE_POMODORO = 0;
const int PAGE_SETTINGS = 1;

const int DEFAULT_JOB_NUM = 4;
const int DEFAULT_MIN_NUM = 3;
const int DEFAULT_MAX_NUM = 1;

const char * const DEFAULT_AUDIO_MESSAGE = "qrc:/resources/audio/im-phone-ring.ogg";
const char * const DEFAULT_TRAY_TITLE = "Qomodoro information";
const char * const DEFAULT_TRAY_UNDEFINED = "Undefined step";
const char * const DEFAULT_TRAY_MESSAGE[3] = {"Finished JOB (step %d/%d)",
                                              "Finished MIN BREAK (step %d/%d)",
                                              "Finished MAX BREAK (step %d/%d)"};
#endif // CONSTS_H
