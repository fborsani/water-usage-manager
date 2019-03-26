#ifndef ESSENTIALS_H
#define ESSENTIALS_H

#include <QString>
#include <QVector>
#include <QDateTime>
#include <QTime>

#define THREADTIMEOUT 5000

#define DATE_FORMAT "yyyy-MM-dd"
#define TIME_FORMAT "HH:mm:ss"

#define DATE_TIME_FORMAT "yyyy-MM-dd HH:mm:ss"

#define LOWEST_DATE QDateTime::fromString("2015-01-01 00:00:00",DATE_TIME_FORMAT)
#define HIGHTEST_DATE QDateTime::fromString("2015-12-31 00:00:00",DATE_TIME_FORMAT)

enum class scan_timeSpan{HOUR,DAY,WEEK,MONTH};
enum class scan_type{CONS_GRAPH,CONS_MED,LEAKS,HIGHTCONS};

#endif // ESSENTIALS_H
