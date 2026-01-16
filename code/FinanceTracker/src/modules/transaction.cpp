#include "transaction.h"

Transaction::Transaction()
    : transactionId("")
    , amount(0.0)
    , inputDate(QDate::currentDate())
    , targetType(TransactionTargetType::None)
    , targetId("")
{
}

Transaction::Transaction(double amt, TransactionTargetType type, const QString& target, QDate date)
    : transactionId("")
    , amount(amt)
    , inputDate(date)
    , targetType(type)
    , targetId(target)
{
}

QString Transaction::getId() const {
    return transactionId;
}

double Transaction::getAmount() const {
    return amount;
}

QDate Transaction::getInputDate() const {
    return inputDate;
}

TransactionTargetType Transaction::getTargetType() const {
    return targetType;
}

QString Transaction::getTargetId() const {
    return targetId;
}

void Transaction::setId(const QString &id) {
    transactionId = id;
}

void Transaction::setAmount(double amt) {
    amount = amt;
}

void Transaction::setInputDate(const QDate &date) {
    inputDate = date;
}

void Transaction::setTargetType(TransactionTargetType type) {
    targetType = type;
}

void Transaction::setTargetId(const QString& target) {
    targetId = target;
}
