#ifndef CATEGORYMANAGER_H
#define CATEGORYMANAGER_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QListWidgetItem>
#include "../modules/category.h"

namespace Ui {
class CategoryManager;
}

class CategoryManager : public QWidget
{
    Q_OBJECT

public:
    explicit CategoryManager(QWidget *parent = nullptr);
    ~CategoryManager();

    void addCategory(const Category &category);
    void updateCategory(const QString &categoryId, const Category &updatedCategory);
    void deleteCategory(const QString &categoryId);
    Category getCategory(const QString &categoryId) const;
    QList<Category> getAllCategories() const;

signals:
    void goToMainMenu();

private slots:
    void on_homeBtn_clicked();
    void on_addCategoryBtn_clicked();
    void on_updateCategoryBtn_clicked();
    void on_deleteCategoryBtn_clicked();

private:
    Ui::CategoryManager *ui;
    void clearForm();
    void populateCategoryList();
};

#endif // CATEGORYMANAGER_H
