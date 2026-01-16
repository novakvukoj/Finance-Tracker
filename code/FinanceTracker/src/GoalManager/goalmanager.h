#ifndef GOALMANAGER_H
#define GOALMANAGER_H

#include <QWidget>
#include <QList>
#include <QVector>
#include <QString>
#include <QListWidgetItem>
#include "src/modules/goal.h"

namespace Ui {
class GoalManager;
}

class GoalManager : public QWidget
{
    Q_OBJECT

public:
    explicit GoalManager(QWidget *parent = nullptr);
    ~GoalManager();

    bool addGoal(const Goal &goal);
    void removeGoal(QString name);
    void updateGoal(QString name, Goal &goal);
    Goal getGoal(QString name);
    QList<Goal>& getAllGoals();
    void displayGoals();


private slots:
    void on_addGoalBtn_clicked();
    void on_updateGoalBtn_clicked();
    void on_removeGoalBtn_clicked();
    void on_clearFieldsBtn_clicked();
    void on_goalListWidget_itemClicked(QListWidgetItem *item);
    void on_goalListWidget_itemSelectionChanged();
    void on_homeBtn_clicked();

private:
    Ui::GoalManager *ui;
    QList<Goal> goalList;
    QString selectedGoalName;

    void clearFields();
    void updateButtonStates();

signals:
    void goToMainMenu();
};

#endif // GOALMANAGER_H
