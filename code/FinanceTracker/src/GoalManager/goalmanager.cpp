#include "goalmanager.h"
#include "ui_goalmanager.h"
#include "src/modules/datamanager.h"
#include "goalbuilder.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QCalendarWidget>
#include <QShowEvent>

GoalManager::GoalManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GoalManager)
    , selectedGoalName("")
{
    ui->setupUi(this);
    ui->goalStartInput->setDate(QDate::currentDate());
    ui->goalEndInput->setDate(QDate::currentDate().addYears(1));
    this->setAttribute(Qt::WA_StyledBackground, true);

    connect(&DataManager::instance(), &DataManager::goalsChanged, this, &GoalManager::displayGoals);
    displayGoals();
    ui->goalListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

GoalManager::~GoalManager() {
    delete ui;
}

bool GoalManager::addGoal(const Goal &goal) {
    const QList<Goal>& goals = DataManager::instance().getGoals();
    for (const Goal &i : goals) {
        if (i.getName() == goal.getName()) {
            QMessageBox::warning(this, "Duplicate Goal", "A goal with this name already exists!");
            return false;
        }
    }

    DataManager::instance().addGoal(goal);
    return true;
}

void GoalManager::removeGoal(QString name) {
    Goal goal = DataManager::instance().getGoal(name);
    if (goal.getName().isEmpty()) {
        QMessageBox::warning(this, "Goal Not Found", QString("No goal found with Name: %1").arg(name));
        return;
    }
    DataManager::instance().removeGoal(name);
}

void GoalManager:: updateGoal(QString name, Goal &goal) {
    Goal existingGoal = DataManager::instance().getGoal(name);
    if (existingGoal.getName().isEmpty()) {
        QMessageBox::warning(this, "Goal Not Found", QString("No goal found with name: %1").arg(name));
        return;
    }
    DataManager::instance().updateGoal(name, goal);
}

Goal GoalManager::getGoal(QString name) {
    Goal goal = DataManager::instance().getGoal(name);
    if (goal.getName().isEmpty()) {
        QMessageBox::warning(this, "Goal Not Found", QString("No goal found with name: %1").arg(name));
    }
    return goal;
}

void GoalManager::on_addGoalBtn_clicked() {
    QString errorMsg;

    GoalBuilder builder = GoalBuilder()
        .setName(ui->goalNameInput->text())
        .setTarget(ui->goalTargetInput->value())
        .setCurrentAmount(ui->goalCurrentInput->value())
        .setDateRange(ui->goalStartInput->date(), ui->goalEndInput->date());

    if (!builder.isValid(&errorMsg)) {
        QMessageBox::warning(this, "Invalid Input", errorMsg);
        return;
    }

    if (!addGoal(builder.build())) {
        return;
    }
    clearFields();
    QMessageBox::information(this, "Success", "Goal was successfully added!");
}

void GoalManager::on_updateGoalBtn_clicked() {
    if (selectedGoalName.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a goal to update!");
        return;
    }

    Goal currentGoal = getGoal(selectedGoalName);
    if (currentGoal.getName().isEmpty()) {
        QMessageBox::warning(this, "Error", "Could not find goal!");
        return;
    }

    QDialog updateDialog(this);
    updateDialog.setWindowTitle("Update Goal: " + currentGoal.getName());
    updateDialog.setModal(true);
    updateDialog.setMinimumWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(&updateDialog);
    QFormLayout *formLayout = new QFormLayout();

    QLineEdit *nameEdit = new QLineEdit(currentGoal.getName());
    nameEdit->setReadOnly(true);

    QDoubleSpinBox *targetSpin = new QDoubleSpinBox();
    targetSpin->setMaximum(999999999.99);
    targetSpin->setDecimals(2);
    targetSpin->setValue(currentGoal.getTarget());

    QDoubleSpinBox *currentSpin = new QDoubleSpinBox();
    currentSpin->setMaximum(999999999.99);
    currentSpin->setDecimals(2);
    currentSpin->setValue(currentGoal.getCurrentAmount());

    QDateEdit *startDateEdit = new QDateEdit(currentGoal.getStartDate());
    startDateEdit->setCalendarPopup(true);
    QCalendarWidget *startCal = new QCalendarWidget();
    startCal->setStyleSheet(
        "QCalendarWidget { background-color: white; }"
        "QCalendarWidget QTableView { color: black; background-color: white; selection-background-color: #81c784; }"
        "QCalendarWidget QWidget { alternate-background-color: #f0f0f0; color: black; }"
        "QCalendarWidget QAbstractItemView:enabled { color: black; background-color: white; }"
        "QCalendarWidget QMenu { color: black; background-color: white; }"
        "QCalendarWidget QSpinBox { color: black; background-color: white; }"
        "QCalendarWidget QToolButton { color: black; }"
        );
    startDateEdit->setCalendarWidget(startCal);

    QDateEdit *endDateEdit = new QDateEdit(currentGoal.getEndDate());
    endDateEdit->setCalendarPopup(true);
    QCalendarWidget *endCal = new QCalendarWidget();
    endCal->setStyleSheet(
        "QCalendarWidget { background-color: white; }"
        "QCalendarWidget QTableView { color: black; background-color: white; selection-background-color: #81c784; }"
        "QCalendarWidget QWidget { alternate-background-color: #f0f0f0; color: black; }"
        "QCalendarWidget QAbstractItemView:enabled { color: black; background-color: white; }"
        "QCalendarWidget QMenu { color: black; background-color: white; }"
        "QCalendarWidget QSpinBox { color: black; background-color: white; }"
        "QCalendarWidget QToolButton { color: black; }"
        );
    endDateEdit->setCalendarWidget(endCal);

    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("Target Amount:", targetSpin);
    formLayout->addRow("Current Amount:", currentSpin);
    formLayout->addRow("Start Date:", startDateEdit);
    formLayout->addRow("End Date:", endDateEdit);

    mainLayout->addLayout(formLayout);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, &updateDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &updateDialog, &QDialog::reject);

    if (updateDialog.exec() == QDialog::Accepted) {
        QString errorMsg;
        GoalBuilder builder = GoalBuilder()
            .setName(nameEdit->text())
            .setTarget(targetSpin->value())
            .setCurrentAmount(currentSpin->value())
            .setDateRange(startDateEdit->date(), endDateEdit->date());

        if (!builder.isValid(&errorMsg)) {
            QMessageBox::warning(this, "Invalid Input", errorMsg);
            return;
        }

        Goal updatedGoal = builder.build();
        updateGoal(selectedGoalName, updatedGoal);
        QMessageBox::information(this, "Success", QString("Goal '%1' updated successfully!").arg(selectedGoalName));
    }
}

void GoalManager::on_goalListWidget_itemClicked(QListWidgetItem *item) {
    if (item) {
        selectedGoalName = item->data(Qt::UserRole).toString();
    }
}

void GoalManager::on_removeGoalBtn_clicked() {
    if (selectedGoalName.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a goal to remove!");
        return;
    }

    auto reply = QMessageBox::question(this, "Confirm Removal",
    QString("Are you sure you want to remove the goal '%1'?").arg(selectedGoalName),
    QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        removeGoal(selectedGoalName);
        QMessageBox::information(this, "Success", QString("Goal '%1' removed successfully!").arg(selectedGoalName));
    }
}

void GoalManager::on_clearFieldsBtn_clicked() {
    auto reply = QMessageBox::question(this, "Clear Fields", QString("Are you sure you want clear input fields?"),
    QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        clearFields();
        QMessageBox::information(this, "Success", QString("Input fields have been cleared!"));
    }
}

void GoalManager::on_homeBtn_clicked() {
    emit goToMainMenu();
}


void GoalManager::on_goalListWidget_itemSelectionChanged() {
    QList<QListWidgetItem*> selected = ui->goalListWidget->selectedItems();
    if (selected.isEmpty()) {
        selectedGoalName = "";
    } else {
        selectedGoalName = selected.first()->data(Qt::UserRole).toString();
    }
}

void GoalManager::clearFields() {
    ui->goalNameInput->clear();
    ui->goalTargetInput->setValue(0.0);
    ui->goalCurrentInput->setValue(0.0);
    ui->goalStartInput->setDate(QDate::currentDate());
    ui->goalEndInput->setDate(QDate::currentDate().addYears(1));
}

void GoalManager:: displayGoals() {
    ui->goalListWidget->clear();
    const QList<Goal>& goals = DataManager::instance().getGoals();
    if (goals.isEmpty()) {
        ui->goalListWidget->addItem("No goals yet. Add your first goal!");
        return;
    }

    for (const Goal &goal : goals) {
        QString goalInfo = QString("%1: $%2 / $%3 (%4%) - Ends: %5")
        .arg(goal.getName())
            .arg(goal.getCurrentAmount(), 0, 'f', 2)
            .arg(goal.getTarget(), 0, 'f', 2)
            .arg(goal.getProgress(), 0, 'f', 1)
            .arg(goal.getEndDate().toString("yyyy-MM-dd"));
        QListWidgetItem *item = new QListWidgetItem(goalInfo);
        item->setData(Qt::UserRole, goal.getName());

        ui->goalListWidget->addItem(item);
    }
}

