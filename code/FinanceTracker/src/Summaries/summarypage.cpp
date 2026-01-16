#include "summarypage.h"
#include "ui_summarypage.h"
#include "src/modules/datamanager.h"
#include <QShowEvent>

SummaryPage::SummaryPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SummaryPage)
    , summaryGenerator(new summary())
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
    ui->timeFrameLabel->setVisible(false);
    ui->timeFrameComboBox->setVisible(false);

    connect(&DataManager::instance(), &DataManager::categoriesChanged, this, [this]() {
        if (ui->categoryRadioBtn->isChecked()) {
            listCategories();
        }
    });

    connect(&DataManager::instance(), &DataManager::goalsChanged, this, [this]() {
        if (ui->goalRadioBtn->isChecked()) {
            listGoals();
        }
    });
    listGoals();

}

SummaryPage::~SummaryPage() {
    delete summaryGenerator;
    delete ui;
}

void SummaryPage::on_goalRadioBtn_toggled(bool checked) {
    if (checked) {
        ui->selectionLabel->setText("Select Goal:");
        ui->timeFrameLabel->setVisible(false);
        ui->timeFrameComboBox->setVisible(false);
        listGoals();
    }
}

void SummaryPage::on_categoryRadioBtn_toggled(bool checked) {
    if (checked) {
        ui->selectionLabel->setText("Select Category:");
        ui->timeFrameLabel->setVisible(true);
        ui->timeFrameComboBox->setVisible(true);
        listCategories();
    }
}


void SummaryPage::listGoals() {
    ui->selectionComboBox->clear();

    goalList = DataManager::instance().getGoals();
    if (goalList.isEmpty()) {
        ui->selectionComboBox->addItem("No goals available");
        return;
    }

    for (const Goal& goal : goalList) {
        ui->selectionComboBox->addItem(goal.getName());
    }
}

void SummaryPage::listCategories() {
    ui->selectionComboBox->clear();

    categoryList = DataManager::instance().getCategories();
    if (categoryList.isEmpty()) {
        ui->selectionComboBox->addItem("No categories available");
        return;
    }

    for (const Category& category : categoryList) {
        ui->selectionComboBox->addItem(category.getCategoryName());
    }
}

void SummaryPage::on_generateBtn_clicked() {
    QString selectedItem = ui->selectionComboBox->currentText();

    if (selectedItem.isEmpty() ||
        selectedItem == "No goals available" ||
        selectedItem == "No categories available") {
        return;
    }

    if (ui->goalRadioBtn->isChecked()) {
        Goal goal = DataManager::instance().getGoal(selectedItem);
        if (goal.getName().isEmpty()) {
            return;
        }

        summaryGenerator->setViewForGoal(goal);
        summaryGenerator->generateSummary(DataManager::instance().getTransactions());
        emit showSummaryView(summaryGenerator, true);

    } else if (ui->categoryRadioBtn->isChecked()) {
        Category selectedCategory;
        for (const Category& c : categoryList) {
            if (c.getCategoryName() == selectedItem) {
                selectedCategory = c;
                break;
            }
        }

        if (selectedCategory.getCategoryID().isEmpty()) {
            return;
        }
        summaryGenerator->setViewForCategory(selectedCategory);

        QString timeFrameText = ui->timeFrameComboBox->currentText();
        summary::TimeFrame tf = summary::MONTHLY;
        if (timeFrameText == "Daily") tf = summary::DAILY;
        else if (timeFrameText == "Monthly") tf = summary::MONTHLY;
        else if (timeFrameText == "Yearly") tf = summary::YEARLY;

        summaryGenerator->setTimeFrame(tf);
        summaryGenerator->generateSummary(DataManager::instance().getTransactions());
        emit showSummaryView(summaryGenerator, false);
    }
}

QMap<QDate, double> summary::getSummaryData() const {
    return summaryData;
}

QList<QPair<QDate, double>> summary::getGoalProgressData() const {
    return goalProgressData;
}

void SummaryPage::on_homeBtn_clicked() {
    emit goToMainMenu();
}

void SummaryPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    goalList = DataManager::instance().getGoals();
    transactionList = DataManager::instance().getTransactions();

    if (ui->goalRadioBtn->isChecked()) {
        listGoals();
    }
}
