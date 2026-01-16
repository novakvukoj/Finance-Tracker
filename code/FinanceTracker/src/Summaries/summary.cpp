#include "summary.h"

summary::summary()
    : viewType(GOAL)
    , timeFrame(DAILY)
{
}

void summary::setViewForGoal(const Goal& goal) {
    viewType = GOAL;
    selectedGoal = goal;
    selectedCategory = Category();
}

void summary::setViewForCategory(const Category& category) {
    viewType = CATEGORY;
    selectedCategory = category;
    selectedGoal = Goal();
}

void summary::setTimeFrame(TimeFrame frame) {
    timeFrame = frame;
}

void summary::generateSummary(const QList<Transaction>& transactions) {
    summaryData.clear();
    goalProgressData.clear();

    if (viewType == GOAL) {
        generateGoalSummary(transactions);
    } else {
        generateCategorySummary(transactions);
    }
}

void summary::generateGoalSummary(const QList<Transaction>& transactions) {
    QList<Transaction> goalTransactions;
    QString goalName = selectedGoal.getName();

    for (const Transaction& t : transactions) {
        if (t.getTargetType() == TransactionTargetType::Goal &&
            (t.getTargetId() == goalName || t.getTargetId() == "All")) {
            goalTransactions.append(t);
        }
    }

    std::sort(goalTransactions.begin(), goalTransactions.end(),
              [](const Transaction& a, const Transaction& b) {
                  return a.getInputDate() < b.getInputDate();
              });

    QDate startDate = selectedGoal.getStartDate();
    goalProgressData.append(qMakePair(startDate, 0.0));

    double cumulativeAmount = 0.0;
    for (const Transaction& t : goalTransactions) {
        cumulativeAmount += t.getAmount();
        goalProgressData.append(qMakePair(t.getInputDate(), cumulativeAmount));
    }

    QDate endDate = selectedGoal.getEndDate();
    if (goalProgressData.size() == 1 || goalProgressData.last().first < endDate) {
        goalProgressData.append(qMakePair(endDate, cumulativeAmount));
    }
}

void summary::generateCategorySummary(const QList<Transaction>& transactions) {
    QPair<QDate, QDate> dateRange = getDateRangeForTimeFrame();
    QDate startDate = dateRange.first;
    QDate endDate = dateRange.second;
    QString categoryId = selectedCategory.getCategoryID();

    summaryData.clear();
    if (timeFrame == DAILY) {
        for (QDate d = startDate; d <= endDate; d = d.addDays(1)) {
            summaryData[d] = 0.0;
        }
    } else if (timeFrame == MONTHLY) {
        QDate d = QDate(startDate.year(), startDate.month(), 1);
        QDate end = QDate(endDate.year(), endDate.month(), 1);
        while (d <= end) {
            summaryData[d] = 0.0;
            d = d.addMonths(1);
        }
    } else if (timeFrame == YEARLY) {
        QDate d = QDate(startDate.year(), 1, 1);
        QDate end = QDate(endDate.year(), 1, 1);
        while (d <= end) {
            summaryData[d] = 0.0;
            d = d.addYears(1);
        }
    }

    for (const Transaction& t : transactions) {
        if (t.getTargetType() == TransactionTargetType::Category &&
            t.getTargetId() == categoryId &&
            t.getInputDate() >= startDate &&
            t.getInputDate() <= endDate) {

            QDate groupedDate = getGroupedDate(t.getInputDate());
            summaryData[groupedDate] += t.getAmount();
        }
    }
}

QDate summary::getGroupedDate(const QDate& date) const {
    switch (timeFrame) {
    case DAILY:
        return date;
    case MONTHLY:
        return QDate(date.year(), date.month(), 1);
    case YEARLY:
        return QDate(date.year(), 1, 1);
    default:
        return date;
    }
}

QPair<QDate, QDate> summary::getDateRangeForTimeFrame() const {
    QDate currentDate = QDate::currentDate();
    QDate startDate;

    switch (timeFrame) {
    case DAILY:
        startDate = QDate(currentDate.year(), currentDate.month(), 1);
        break;
    case MONTHLY:
        startDate = QDate(currentDate.year(), currentDate.month(), 1).addMonths(-11);
        break;
    case YEARLY:
        startDate = currentDate.addYears(-5);
        break;
    default:
        startDate = currentDate.addMonths(-1);
    }

    return qMakePair(startDate, currentDate);
}

QString summary::getTitle() const {
    if (viewType == GOAL) {
        return QString("Goal Progress: %1").arg(selectedGoal.getName());
    } else {
        return QString("Category Summary: %1").arg(selectedCategory.getCategoryName());
    }
}

QString summary::getSubtitle() const {
    if (viewType == GOAL) {
        return QString("Target: $%1").arg(selectedGoal.getTarget(), 0, 'f', 2);
    } else {
        QString timeFrameStr;
        switch (timeFrame) {
        case DAILY: timeFrameStr = "Daily (Last 30 Days)"; break;
        case MONTHLY: timeFrameStr = "Monthly (Last 12 Months)"; break;
        case YEARLY: timeFrameStr = "Yearly (Last 10 Years)"; break;
        }

        QPair<QDate, QDate> range = getDateRangeForTimeFrame();
        return QString("%1: %2 to %3")
            .arg(timeFrameStr)
            .arg(range.first.toString("MMM dd, yyyy"))
            .arg(range.second.toString("MMM dd, yyyy"));
    }
}

double summary::getTotalAmount() const {
    if (viewType == GOAL) {
        return goalProgressData.isEmpty() ? 0.0 : goalProgressData.last().second;
    } else {
        double total = 0.0;
        for (double amount : summaryData.values()) {
            total += amount;
        }
        return total;
    }
}

int summary::getTransactionCount() const {
    if (viewType == GOAL) {
        return goalProgressData.size() > 2 ? goalProgressData.size() - 2 : 0;
    } else {
        return summaryData.size();
    }
}

QDate summary::getStartDate() const {
    if (viewType == GOAL) {
        return goalProgressData.isEmpty() ? QDate() : goalProgressData.first().first;
    } else {
        return summaryData.isEmpty() ? QDate() : summaryData.firstKey();
    }
}

QDate summary::getEndDate() const {
    if (viewType == GOAL) {
        return goalProgressData.isEmpty() ? QDate() : goalProgressData.last().first;
    } else {
        return summaryData.isEmpty() ? QDate() : summaryData.lastKey();
    }
}

double summary::getGoalTarget() const {
    return selectedGoal.getTarget();
}

double summary::getGoalCurrent() const {
    return selectedGoal.getCurrentAmount();
}

double summary::getGoalProgress() const {
    return selectedGoal.getProgress();
}

