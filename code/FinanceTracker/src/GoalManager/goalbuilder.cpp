#include "goalbuilder.h"
#include <QMessageBox>

GoalBuilder::GoalBuilder()
    : target(0.0)
    , currentAmount(0.0)
    , hasName(false)
    , hasTarget(false)
    , hasDates(false)
{
}

GoalBuilder& GoalBuilder::setName(const QString& goalName) {
    this->name = goalName;
    this->hasName = true;
    return *this;
}

GoalBuilder& GoalBuilder::setTarget(double targetAmount) {
    this->target = targetAmount;
    this->hasTarget = true;
    return *this;
}

GoalBuilder& GoalBuilder::setCurrentAmount(double amount) {
    this->currentAmount = amount;
    return *this;
}

GoalBuilder& GoalBuilder::setStartDate(const QDate& date) {
    this->startDate = date;
    return *this;
}

GoalBuilder& GoalBuilder::setEndDate(const QDate& date) {
    this->endDate = date;
    this->hasDates = true;
    return *this;
}

GoalBuilder& GoalBuilder::setDateRange(const QDate& start, const QDate& end) {
    this->startDate = start;
    this->endDate = end;
    this->hasDates = true;
    return *this;
}

bool GoalBuilder::isValid(QString* errorMessage) const {
    if (!hasName) {
        if (errorMessage) *errorMessage = "Goal name is required";
        return false;
    }

    if (name.trimmed().isEmpty()) {
        if (errorMessage) *errorMessage = "Goal name cannot be empty";
        return false;
    }

    if (!hasTarget) {
        if (errorMessage) *errorMessage = "Target amount is required";
        return false;
    }

    if (target <= 0) {
        if (errorMessage) *errorMessage = "Target amount must be greater than 0";
        return false;
    }

    if (!hasDates) {
        if (errorMessage) *errorMessage = "Start and end dates are required";
        return false;
    }

    if (endDate < startDate) {
        if (errorMessage) *errorMessage = "End date must be after start date";
        return false;
    }

    if (currentAmount < 0) {
        if (errorMessage) *errorMessage = "Current amount cannot be negative";
        return false;
    }

    if (currentAmount > target) {
        if (errorMessage) *errorMessage = "Current amount cannot exceed target";
        return false;
    }

    return true;
}

Goal GoalBuilder::build() const {
    QString error;
    if (!isValid(&error)) {
        qWarning() << "GoalBuilder validation failed:" << error;
    }

    Goal goal;
    goal.setName(name);
    goal.setTarget(target);
    goal.setCurrentAmount(currentAmount);
    goal.setStartDate(startDate);
    goal.setEndDate(endDate);

    return goal;
}

Goal GoalBuilder::buildOrDefault() const {
    Goal goal;
    goal.setName(hasName ? name : "Unnamed Goal");
    goal.setTarget(hasTarget ? target : 1000.0);
    goal.setCurrentAmount(currentAmount);

    if (hasDates) {
        goal.setStartDate(startDate);
        goal.setEndDate(endDate);
    } else {
        goal.setStartDate(QDate::currentDate());
        goal.setEndDate(QDate::currentDate().addYears(1));
    }

    return goal;
}
