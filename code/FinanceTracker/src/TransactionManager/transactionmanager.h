#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QListWidgetItem>
#include "../modules/transaction.h"

namespace Ui {
class TransactionManager;
}

class TransactionManager : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionManager(QWidget *parent = nullptr);
    ~TransactionManager();

    void addTransaction(const Transaction &transaction);
    void deleteTransaction(int index);
    QList<Transaction> getTransactionsByDate(const QDate &startDate, const QDate &endDate) const;
    QList<Transaction> getTransactionsByCategory(const QString &categoryId) const;
    void applyTransactionToAllGoals(const Transaction &transaction);
    void applyTransactionToSpecificGoal(const Transaction &transaction, const QString &goalId);
    void applyTransactionToCategory(const Transaction &transaction, const QString &categoryId);

signals:
    void goToMainMenu();

private slots:
    void on_homeBtn_clicked();
    void on_addTransactionBtn_clicked();
    void on_deleteTransactionBtn_clicked();
    void on_targetTypeChanged(int index);
    void on_clearFieldsBtn_clicked();
    void on_transactionAmountInput_valueChanged(double arg1);

private:
    Ui::TransactionManager *ui;
    void clearForm();
    void populateCategoryGoalDropdown();
    void refreshTransactionList();
};

#endif // TRANSACTIONMANAGER_H
