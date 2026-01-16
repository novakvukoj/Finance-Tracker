#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "goal.h"
#include "src/modules/transaction.h"
#include "src/modules/category.h"

class DataManager : public QObject
{
    Q_OBJECT
public:
    static DataManager& instance();
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    void clear();
    const QList<Goal>& getGoals() const;
    const QList<Transaction>& getTransactions() const;
    const QList<Category>& getCategories() const;

    void addGoal(const Goal& goal);
    void removeGoal(const QString& name);
    void updateGoal(const QString& name, const Goal& goal);

    void addTransaction(const Transaction& transaction);
    void removeTransaction(int index);

    void addCategory(const Category& category);
    void removeCategory(const QString& id);
    void updateCategory(const QString& id, const Category& category);
    Category getCategory(const QString& id) const;

    Goal getGoal(const QString& name) const;

private:
    explicit DataManager();
    ~DataManager() = default;

    QList<Goal> goals;
    QList<Transaction> transactions;
    QList<Category> categories;

signals:
    void goalsChanged();
    void transactionsChanged();
    void categoriesChanged();
};

#endif // DATAMANAGER_H
