#ifndef SUMMARYVIEW_H
#define SUMMARYVIEW_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include "src/Summaries/summary.h"

namespace Ui {
class SummaryView;
}

class SummaryView : public QWidget
{
    Q_OBJECT

public:
    explicit SummaryView(QWidget *parent = nullptr);
    ~SummaryView();

    void displayGoalGraph(summary* data);
    void displayCategoryGraph(summary* data);

private slots:
    void on_backBtn_clicked();

private:
    Ui::SummaryView *ui;
    QChartView* chartView;

    void clearChart();
    void createGoalChart(summary* data);
    void createCategoryChart(summary* data);

signals:
    void goBack();
};

#endif // SUMMARYVIEW_H
