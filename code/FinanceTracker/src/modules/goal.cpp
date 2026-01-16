#include "goal.h"

Goal::Goal()
    : name("")
    , target(0.0)
    , startDate(QDate())
    , endDate(QDate())
    , currentAmount(0.0)
{
}

Goal::Goal(QString name, double target, QDate startDate, QDate endDate, double currentAmount)
    : name(name)
    , target(target)
    , startDate(startDate)
    , endDate(endDate)
    , currentAmount(currentAmount)
{
}


QString Goal::getName() const {
    return name;
}

double Goal::getTarget() const {
    return target;
}

double Goal::getCurrentAmount() const {
    return currentAmount;
}

QDate Goal::getStartDate() const {
    return startDate;
}

QDate Goal::getEndDate() const {
    return endDate;
}

double Goal::getProgress() const {
    if (target <= 0.0)
        return 0.0;
    return (currentAmount / target) * 100.0;
}


void Goal::setName(const QString &name) {
    this->name = name;
}

void Goal::setTarget(const double &target) {
    this->target = target;
}

void Goal::setCurrentAmount(const double &newAmount) {
    this->currentAmount = newAmount;
}

void Goal::setStartDate(const QDate &startDate) {
    this->startDate = startDate;
}

void Goal::setEndDate(const QDate &endDate) {
    this->endDate = endDate;
}

void Goal::setProgress(const double &progress) {
    this->currentAmount = (progress / 100.0) * target;
}
