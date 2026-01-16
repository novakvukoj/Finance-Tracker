#include "summaryview.h"
#include "ui_summaryview.h"
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSet>

SummaryView::SummaryView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SummaryView)
    , chartView(nullptr)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
}

SummaryView::~SummaryView()
{
    if (chartView) {
        delete chartView;
    }
    delete ui;
}

void SummaryView::displayGoalGraph(summary* data)
{
    clearChart();
    createGoalChart(data);
}

void SummaryView::displayCategoryGraph(summary* data)
{
    clearChart();
    createCategoryChart(data);
}

void SummaryView::clearChart()
{
    if (chartView) {
        ui->chartLayout->removeWidget(chartView);
        delete chartView;
        chartView = nullptr;
    }
}

void SummaryView::createGoalChart(summary* data)
{
    QLineSeries *progressSeries = new QLineSeries();
    progressSeries->setName("Progress");

    QLineSeries *targetLine = new QLineSeries();
    targetLine->setName("Target");
    QPen targetPen(Qt::red);
    targetPen.setStyle(Qt::DashLine);
    targetPen.setWidth(2);
    targetLine->setPen(targetPen);

    QList<QPair<QDate, double>> progressData = data->getGoalProgressData();

    if (progressData.isEmpty()) {
        return;
    }

    for (const auto& point : progressData) {
        QDateTime dt(point.first, QTime(0, 0));
        progressSeries->append(dt.toMSecsSinceEpoch(), point.second);
    }

    qint64 minTime = QDateTime(progressData.first().first, QTime(0,0)).toMSecsSinceEpoch();
    qint64 maxTime = QDateTime(progressData.last().first, QTime(0,0)).toMSecsSinceEpoch();

    double targetValue = data->getGoalTarget();
    targetLine->append(minTime, targetValue);
    targetLine->append(maxTime, targetValue);

    QChart* chart = new QChart();
    chart->addSeries(progressSeries);
    chart->addSeries(targetLine);
    chart->setTitle(data->getTitle());
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTitleText("Date");
    axisX->setFormat("MMM yyyy");
    axisX->setTickCount(6);
    axisX->setRange(QDateTime(progressData.first().first, QTime(0, 0)), QDateTime(progressData.last().first, QTime(0, 0)));
    chart->addAxis(axisX, Qt::AlignBottom);
    progressSeries->attachAxis(axisX);
    targetLine->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Amount ($)");
    axisY->setLabelFormat("$%.2f");
    axisY->setRange(0, targetValue * 1.1);
    axisY->setTickCount(6);
    axisY->setLabelFormat("$%.0f");
    chart->addAxis(axisY, Qt::AlignLeft);
    progressSeries->attachAxis(axisY);
    targetLine->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartLayout->addWidget(chartView);
}

void SummaryView::createCategoryChart(summary* data)
{
    QMap<QDate, double> newData = data->getSummaryData();

    if (newData.isEmpty()) {
        return;
    }

    QString categoryName = data->getTitle().replace("Category Summary: ", "");
    QBarSet *barSet = new QBarSet(categoryName);
    QStringList categories;

    QList<QDate> dates = newData.keys();
    std::sort(dates.begin(), dates.end());

    summary::TimeFrame tf = data->getTimeFrameValue();

    for (const QDate& date : dates) {
        *barSet << newData[date];

        QString label;
        if (tf == summary::DAILY) {
            label = date.toString("dd");
        } else if (tf == summary::MONTHLY) {
            label = date.toString("MMM");
        } else {
            label = date.toString("yyyy");
        }
        categories << label;
    }

    QBarSeries *series = new QBarSeries();
    series->append(barSet);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(data->getTitle());
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Time Period");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Amount ($)");
    axisY->setTickCount(6);
    axisY->setLabelFormat("$%.0f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartLayout->addWidget(chartView);
}

void SummaryView::on_backBtn_clicked()
{
    emit goBack();
}
