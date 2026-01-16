#include "transactionmanager.h"
#include "ui_transactionmanager.h"
#include "src/modules/datamanager.h"
#include <QMessageBox>

TransactionManager::TransactionManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TransactionManager)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);

    ui->transactionDateInput->setDate(QDate::currentDate());
    ui->transactionAmountInput->setKeyboardTracking(true);
    ui->transactionAmountInput->setDecimals(2);
    ui->transactionAmountInput->setMinimum(0.0);
    ui->transactionAmountInput->setMaximum(1000000.0);
    ui->transactionAmountInput->setSingleStep(1.0);
    ui->transactionAmountInput->setValue(0.0);

    ui->transactionTargetTypeInput->setCurrentText("All Goals");

    connect(ui->homeBtn, &QPushButton::clicked, this, &TransactionManager::on_homeBtn_clicked);
    connect(ui->transactionTargetTypeInput, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransactionManager::on_targetTypeChanged);
    connect(ui->clearFieldsBtn, &QPushButton::clicked, this, &TransactionManager::clearForm);

    connect(&DataManager::instance(), &DataManager::transactionsChanged, this, &TransactionManager::refreshTransactionList);
    connect(&DataManager::instance(), &DataManager::categoriesChanged, this, &TransactionManager::populateCategoryGoalDropdown);
    connect(&DataManager::instance(), &DataManager::goalsChanged, this, &TransactionManager::populateCategoryGoalDropdown);

    populateCategoryGoalDropdown();
    refreshTransactionList();
}

TransactionManager::~TransactionManager() {
    delete ui;
}

void TransactionManager::clearForm() {
    ui->transactionAmountInput->setValue(0.0);
    ui->transactionDateInput->setDate(QDate::currentDate());
    ui->transactionTargetTypeInput->setCurrentText("All Goals");
    populateCategoryGoalDropdown();
}

void TransactionManager::on_homeBtn_clicked() {
    emit goToMainMenu();
}

void TransactionManager::on_addTransactionBtn_clicked() {
    QDate date = ui->transactionDateInput->date();
    double amount = ui->transactionAmountInput->value();
    QString target = ui->transactionCategoryGoalInput->currentData().toString();

    QString errorMessage;
    if (amount <= 0.0) errorMessage += "Amount must be greater than 0.\n";
    if (target.isEmpty()) errorMessage += "A target must be selected.\n";
    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", errorMessage.trimmed());
        return;
    }

    Transaction t;
    t.setAmount(amount);
    t.setInputDate(date);

    if (ui->transactionTargetTypeInput->currentText() == "Goal") {
        applyTransactionToSpecificGoal(t, target);
    } else if (ui->transactionTargetTypeInput->currentText() == "All Goals") {
        t.setTargetType(TransactionTargetType::Goal);
        t.setTargetId("All");
        applyTransactionToAllGoals(t);
    } else if (ui->transactionTargetTypeInput->currentText() == "Category") {
        applyTransactionToCategory(t, target);
    }
    refreshTransactionList();
    clearForm();
}

void TransactionManager::on_deleteTransactionBtn_clicked() {
    QListWidgetItem *item = ui->transactionListWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "No Selection", "Please select a transaction to delete!");
        return;
    }

    int index = item->data(Qt::UserRole).toInt();
    const QList<Transaction>& transactions = DataManager::instance().getTransactions();
    if (index < 0 || index >= transactions.size()) {
        QMessageBox::warning(this, "Error", "Invalid transaction selection.");
        return;
    }

    Transaction t = transactions[index];

    if (t.getTargetType() == TransactionTargetType::Goal) {
        if (t.getTargetId() == "All") {
            const QList<Goal>& goals = DataManager::instance().getGoals();
            for (const Goal &goal : goals) {
                Goal g = goal;
                double newAmount = g.getCurrentAmount() - t.getAmount();
                if (newAmount < 0.0) newAmount = 0.0;
                g.setCurrentAmount(newAmount);
                DataManager::instance().updateGoal(g.getName(), g);
            }
        } else {
            Goal g = DataManager::instance().getGoal(t.getTargetId());
            double newAmount = g.getCurrentAmount() - t.getAmount();
            if (newAmount < 0.0) newAmount = 0.0;
            g.setCurrentAmount(newAmount);
            DataManager::instance().updateGoal(g.getName(), g);
        }
    }
    else if (t.getTargetType() == TransactionTargetType::Category) {
        QString catName;
        for (const Category &c : DataManager::instance().getCategories()) {
            if (c.getCategoryID() == t.getTargetId()) {
                catName = c.getCategoryName();
                break;
            }
        }
        DataManager::instance().removeTransaction(index);
        QMessageBox::information(this, "Deleted", QString("Transaction for category '%1' deleted.").arg(catName));
        return;
    }

    DataManager::instance().removeTransaction(index);
    QMessageBox::information(this, "Deleted", "Transaction deleted and goals updated.");
}

void TransactionManager::on_targetTypeChanged(int) {
    populateCategoryGoalDropdown();
}

void TransactionManager::populateCategoryGoalDropdown() {
    ui->transactionCategoryGoalInput->clear();
    QString type = ui->transactionTargetTypeInput->currentText();

    if (type == "Goal") {
        QList<Goal> goals = DataManager::instance().getGoals();
        for (const Goal &g : goals) {
            ui->transactionCategoryGoalInput->addItem(g.getName(), g.getName());
        }
    } else if (type == "Category") {
        QList<Category> categories = DataManager::instance().getCategories();
        for (const Category &c : categories) {
            ui->transactionCategoryGoalInput->addItem(c.getName(), c.getCategoryID());
        }
    } else if (type == "All Goals") {
        ui->transactionCategoryGoalInput->addItem("All Goals", "All");
    }

    ui->transactionCategoryGoalInput->setStyleSheet(
        "QComboBox { color: black; background-color: #f9fff9; }"
        "QComboBox QAbstractItemView { background-color: white; color: black; selection-background-color: #a5d6a7; }"
        );
}

void TransactionManager::on_clearFieldsBtn_clicked() {
    clearForm();
}

void TransactionManager::deleteTransaction(int index) {
    DataManager::instance().removeTransaction(index);
}

void TransactionManager::applyTransactionToAllGoals(const Transaction &transaction) {
    const QList<Goal>& goals = DataManager::instance().getGoals();
    for (const Goal& goal : goals) {
        Goal g = goal;
        g.setCurrentAmount(g.getCurrentAmount() + transaction.getAmount());
        DataManager::instance().updateGoal(g.getName(), g);
    }
    Transaction t = transaction;
    DataManager::instance().addTransaction(t);
}

void TransactionManager::applyTransactionToSpecificGoal(const Transaction &transaction, const QString &goalId) {
    Transaction t = transaction;
    t.setTargetType(TransactionTargetType::Goal);
    t.setTargetId(goalId);
    DataManager::instance().addTransaction(t);

    Goal g = DataManager::instance().getGoal(goalId);
    g.setCurrentAmount(g.getCurrentAmount() + transaction.getAmount());
    DataManager::instance().updateGoal(goalId, g);
}


void TransactionManager::applyTransactionToCategory(const Transaction &transaction, const QString &categoryId) {
    Transaction t = transaction;
    t.setTargetType(TransactionTargetType::Category);
    t.setTargetId(categoryId);
    DataManager::instance().addTransaction(t);
}

void TransactionManager::refreshTransactionList() {
    ui->transactionListWidget->clear();
    ui->transactionListWidget->setWordWrap(false);

    const QList<Transaction>& transactions = DataManager::instance().getTransactions();

    for (int i = 0; i < transactions.size(); ++i) {
        const Transaction &t = transactions[i];
        QString displayText;

        if (t.getTargetType() == TransactionTargetType::Goal) {
            if (t.getTargetId() == "All") {
                displayText = QString("Applied to All Goals - Amount: $%1").arg(t.getAmount(), 0, 'f', 2);
            } else if (!t.getTargetId().isEmpty()) {
                Goal g = DataManager::instance().getGoal(t.getTargetId());
                displayText = QString("Applied to Goal: %1 - Amount added: $%2 | Current: $%3 | Target: $%4 | Progress: %5% | Dates: %6 to %7")
                                  .arg(g.getName())
                                  .arg(t.getAmount(), 0, 'f', 2)
                                  .arg(g.getCurrentAmount(), 0, 'f', 2)
                                  .arg(g.getTarget(), 0, 'f', 2)
                                  .arg(g.getProgress(), 0, 'f', 2)
                                  .arg(g.getStartDate().toString("yyyy-MM-dd"))
                                  .arg(g.getEndDate().toString("yyyy-MM-dd"));
            }
        } else if (t.getTargetType() == TransactionTargetType::Category) {
            QString catName;
            for (const Category &c : DataManager::instance().getCategories()) {
                if (c.getCategoryID() == t.getTargetId()) {
                    catName = c.getCategoryName();
                    break;
                }
            }
            if (!catName.isEmpty())
                displayText = QString("Applied to Category: %1 - Amount: $%2")
                                  .arg(catName)
                                  .arg(t.getAmount(), 0, 'f', 2);
        }

        if (!displayText.trimmed().isEmpty()) {
            QListWidgetItem *item = new QListWidgetItem(displayText.trimmed());
            item->setData(Qt::UserRole, i);
            ui->transactionListWidget->addItem(item);
        }
    }
}


void TransactionManager::on_transactionAmountInput_valueChanged(double arg1)
{
    ui->transactionAmountInput->setValue(arg1);
}

