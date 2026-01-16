#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>
#include <QDate>
#include "../Interface/record.h"

class Category : public Record
{
public:
    Category();
    Category(const QString &name, const QString &id, const QDate &startDate = QDate::currentDate());

    QString getCategoryName() const;
    QString getCategoryID() const;
    QDate getStartDate() const override;

    void setCategoryName(const QString &name);
    void setCategoryID(const QString &id);
    void setStartDate(const QDate &date) override;

    QString getCategoryType() const;
    QString getName() const override;
    void setName(const QString &name) override;
    void setCategoryType(const QString &category);

private:
    QString categoryName;
    QString categoryID;
    QDate startDate;
};

#endif // CATEGORY_H
