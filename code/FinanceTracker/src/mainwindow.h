#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/GoalManager/goalmanager.h"
#include "Summaries/summarypage.h"
#include "src/TransactionManager/transactionmanager.h"
#include "src/CategoryManager/categorymanager.h"
#include "src/Summaries/summaryview.h"
#include "SaveLoad/saveLoad.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_goalButton_clicked();

    void on_transactionButton_clicked();

    void on_summaryButton_2_clicked();

    void on_summaryButton_clicked();

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void recreateManagers();

    void on_deleteSaveButton_clicked();

    void on_deleteSaveButton_2_clicked();

private:
    Ui::MainWindow *ui;
    GoalManager *goalManager;
    SummaryPage *summaryPage;
    TransactionManager *transactionManager;
    CategoryManager *categoryManager;
    SaveLoad *saveLoad;
    SummaryView *summaryView;

    QString currentSaveFile;

};

#endif // MAINWINDOW_H
