
#include "category.h"

Category::Category()
    : categoryName(""), categoryID(""), startDate(QDate::currentDate()) {}

Category::Category(const QString &name, const QString &id, const QDate &startDate)
    : categoryName(name), categoryID(id), startDate(startDate) {}

QString Category::getCategoryName() const {
    return categoryName;
}

QString Category::getCategoryID() const {
    return categoryID;
}

QDate Category::getStartDate() const {
    return startDate;
}

void Category::setCategoryName(const QString &name) {
    categoryName = name;
}

void Category::setCategoryID(const QString &id) {
    categoryID = id;
}

void Category::setStartDate(const QDate &date) {
    startDate = date;
}

QString Category::getCategoryType() const {
    return categoryName;
}

QString Category::getName() const {
    return categoryName;
}

void Category::setName(const QString &name) {
    categoryName = name;
}

void Category::setCategoryType(const QString &category) {
    categoryName = category;
}
