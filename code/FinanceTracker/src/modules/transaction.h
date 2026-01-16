#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDate>

enum class TransactionTargetType {
    None,
    Goal,
    Category
};

class Transaction
{
public:
    Transaction();
    Transaction(double amount, TransactionTargetType type, const QString& targetId, QDate inputDate = QDate::currentDate());

    QString getId() const;
    double getAmount() const;
    QDate getInputDate() const;
    TransactionTargetType getTargetType() const;
    QString getTargetId() const;

    void setId(const QString &id);
    void setAmount(double amount);
    void setInputDate(const QDate &date);
    void setTargetType(TransactionTargetType type);
    void setTargetId(const QString& targetId);

private:
    QString transactionId;
    double amount;
    QDate inputDate;

    TransactionTargetType targetType;
    QString targetId;
};

#endif
