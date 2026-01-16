#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QDate>

class Record {
public:
    virtual ~Record() = default;

    virtual QString getName() const = 0;
    virtual QDate getStartDate() const = 0;

    virtual void setStartDate(const QDate &date) = 0;
    virtual void setName(const QString &name) = 0;
};

#endif
