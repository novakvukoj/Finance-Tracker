#ifndef SUMMARYPAGE_H
#define SUMMARYPAGE_H

#include <QWidget>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include "src/modules/goal.h"
#include "src/modules/transaction.h"
#include "src/modules/category.h"
#include "src/Summaries/summary.h"

namespace Ui {
class SummaryPage;
}

class SummaryPage : public QWidget
{
    Q_OBJECT

public:
    explicit SummaryPage(QWidget *parent = nullptr);
    ~SummaryPage();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void on_goalRadioBtn_toggled(bool checked);
    void on_categoryRadioBtn_toggled(bool checked);
    void on_generateBtn_clicked();
    void on_homeBtn_clicked();

private:
    Ui::SummaryPage *ui;
    QList<Goal> goalList;
    QList<Transaction> transactionList;
    QList<Category> categoryList;

    summary* summaryGenerator;

    void listGoals();
    void listCategories();

signals:
    void goToMainMenu();
    void showSummaryView(summary* data, bool isGoal);
};



#endif // SUMMARYPAGE_H
