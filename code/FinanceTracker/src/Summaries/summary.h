#ifndef SUMMARY_H
#define SUMMARY_H

#include <QString>
#include <QDate>
#include <QList>
#include <QMap>
#include <QPointF>
#include "src/modules/goal.h"
#include "src/modules/transaction.h"
#include "src/modules/category.h"

class summary
{
public:
    enum Type {
        GOAL,
        CATEGORY
    };

    enum TimeFrame {
        DAILY,
        MONTHLY,
        YEARLY
    };

    summary();
    void setViewForGoal(const Goal& goal);
    void setViewForCategory(const Category& category);
    void setTimeFrame(TimeFrame frame);

    void generateSummary(const QList<Transaction>& transactions);
    QMap<QDate, double> getSummaryData() const;
    QList<QPair<QDate, double>> getGoalProgressData() const;


    TimeFrame getTimeFrameValue() const { return timeFrame; }
    QString getTitle() const;
    QString getSubtitle() const;

    double getTotalAmount() const;
    int getTransactionCount() const;
    QDate getStartDate() const;
    QDate getEndDate() const;

    double getGoalTarget() const;
    double getGoalCurrent() const;
    double getGoalProgress() const;

private:
    TimeFrame timeFrame;                   // For category only
    Type viewType;

    Goal selectedGoal;
    QList<QPair<QDate, double>> goalProgressData;
    Category selectedCategory;
    QMap<QDate, double> summaryData;

    void generateGoalSummary(const QList<Transaction>& transactions);
    void generateCategorySummary(const QList<Transaction>& transactions);
    QDate getGroupedDate(const QDate& date) const;
    QPair<QDate, QDate> getDateRangeForTimeFrame() const;
};


#endif // SUMMARY_H
