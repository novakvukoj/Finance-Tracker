#include "datamanager.h"

DataManager::DataManager(): QObject(nullptr)
{}

DataManager& DataManager::instance()
{
    static DataManager instance;
    return instance;
}

void DataManager::clear()
{
    goals.clear();
    categories.clear();
    transactions.clear();
}

void DataManager::addGoal(const Goal& goal)
{
    goals.append(goal);
    emit goalsChanged();
}

void DataManager::removeGoal(const QString& name)
{
    for (int i = 0; i < goals.size(); ++i) {
        if (goals[i].getName() == name) {


            for (int j = transactions.size() - 1; j >= 0; --j) {
                if (transactions[j].getTargetId() == goals[i].getName()) {
                    transactions.removeAt(j);
                }
            }
            goals.removeAt(i);
            emit goalsChanged();
            emit transactionsChanged();
            return;
        }
    }
}

void DataManager::updateGoal(const QString& name, const Goal& goal) {
    for (int i = 0; i < goals.size(); ++i) {
        if (goals[i].getName() == name) {
            goals[i] = goal;
            emit goalsChanged();
            return;
        }
    }
}

Goal DataManager::getGoal(const QString& name) const
{
    for (const Goal& goal : goals) {
        if (goal.getName() == name) {
            return goal;
        }
    }
    return Goal();
}

const QList<Goal>& DataManager::getGoals() const
{
    return goals;
}

void DataManager::addTransaction(const Transaction& transaction)
{
    Transaction newTransaction = transaction;
    QString newId = QString::number(transactions.size() + 1);
    newTransaction.setId(newId);
    transactions.append(newTransaction);
    emit transactionsChanged();
}

void DataManager::removeTransaction(int index)
{
    if (index >= 0 && index < transactions.size()) {
        transactions.removeAt(index);
        emit transactionsChanged();
    }
}

const QList<Transaction>& DataManager::getTransactions() const {
    return transactions;
}

const QList<Category>& DataManager::getCategories() const {
    return categories;
}

void DataManager::addCategory(const Category& category) {
    Category newCategory = category;
    QString newId = QString::number(categories.size() + 1);
    newCategory.setCategoryID(newId);
    categories.append(newCategory);
    emit categoriesChanged();
}

void DataManager::removeCategory(const QString& id) {
    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].getCategoryID() == id) {
            categories.removeAt(i);
            emit categoriesChanged();
            return;
        }
    }
}

void DataManager::updateCategory(const QString& id, const Category& category) {
    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].getCategoryID() == id) {
            categories[i].setCategoryName(category.getCategoryName());
            emit categoriesChanged();
            return;
        }
    }
}

Category DataManager::getCategory(const QString& id) const {
    for (const Category& c : categories) {
        if (c.getCategoryID() == id)
            return c;
    }
    return Category();
}
