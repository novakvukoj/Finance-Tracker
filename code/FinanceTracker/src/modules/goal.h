#ifndef GOAL_H
#define GOAL_H

#include <QString>
#include <QDate>
#include "src/Interface/Record.h"

class Goal : public Record
{
public:
    Goal();
    Goal(QString name, double target, QDate startDate, QDate endDate, double currentAmount = 0.0);

    // Methods implemented from interface
    QString getName() const override;
    QDate getStartDate() const override;

    void setName(const QString &name) override;
    void setStartDate(const QDate &startDate) override;

    double getTarget() const;
    double getCurrentAmount() const;
    QDate getEndDate()const;
    double getProgress() const;

    void setTarget(const double &target);
    void setCurrentAmount(const double &newAmount);
    void setEndDate(const QDate &endDate);
    void setProgress(const double &progress);

private:
    QString name;
    double target;
    QDate startDate;
    QDate endDate;
    double currentAmount;
};

#endif // GOAL_H
