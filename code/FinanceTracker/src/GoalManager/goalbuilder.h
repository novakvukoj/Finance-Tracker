#ifndef GOALBUILDER_H
#define GOALBUILDER_H

#include "src/modules/goal.h"
#include <QDate>
#include <QString>

class GoalBuilder {
private:
    QString name;
    double target;
    double currentAmount;
    QDate startDate;
    QDate endDate;

    bool hasName;
    bool hasTarget;
    bool hasDates;

public:
    GoalBuilder();

    GoalBuilder& setName(const QString& goalName);
    GoalBuilder& setTarget(double targetAmount);
    GoalBuilder& setCurrentAmount(double amount);
    GoalBuilder& setStartDate(const QDate& date);
    GoalBuilder& setEndDate(const QDate& date);
    GoalBuilder& setDateRange(const QDate& start, const QDate& end);

    bool isValid(QString* errorMessage = nullptr) const;
    Goal build() const;
    Goal buildOrDefault() const;
};

#endif // GOALBUILDER_H
