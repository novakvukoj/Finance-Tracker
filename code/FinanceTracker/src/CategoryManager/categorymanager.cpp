#include "categorymanager.h"
#include "ui_categorymanager.h"
#include "src/modules/datamanager.h"
#include <QMessageBox>

CategoryManager::CategoryManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CategoryManager)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);

    connect(ui->homeBtn, &QPushButton::clicked, this, &CategoryManager::on_homeBtn_clicked);
    connect(&DataManager::instance(), &DataManager::categoriesChanged, this, &CategoryManager::populateCategoryList);


    populateCategoryList();
}

CategoryManager::~CategoryManager() {
    delete ui;
}

void CategoryManager::clearForm() {
    ui->categoryNameInput->clear();
    ui->categoryListWidget->clearSelection();
}

void CategoryManager::on_homeBtn_clicked() {
    emit goToMainMenu();
}

void CategoryManager::on_addCategoryBtn_clicked() {
    QString name = ui->categoryNameInput->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a category name.");
        return;
    }

    QList<Category> categories = getAllCategories();
    for (const Category &c : categories) {
        if (c.getName().compare(name, Qt::CaseInsensitive) == 0) {
            QMessageBox::warning(this, "Duplicate Category", "A category with this name already exists.");
            return;
        }
    }

    Category c;
    c.setName(name);
    addCategory(c);
    clearForm();
}

void CategoryManager::on_updateCategoryBtn_clicked() {
    QListWidgetItem *item = ui->categoryListWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "No Selection", "Please select a category to update!");
        return;
    }

    QString oldId = item->data(Qt::UserRole).toString();
    QString newName = ui->categoryNameInput->text().trimmed();
    if (newName.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Category name cannot be empty.");
        return;
    }

    Category existing = getCategory(oldId);
    existing.setCategoryName(newName);
    updateCategory(oldId, existing);

    clearForm();
}

void CategoryManager::on_deleteCategoryBtn_clicked() {
    QListWidgetItem *item = ui->categoryListWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "No Selection", "Please select a category to delete!");
        return;
    }

    QString id = item->data(Qt::UserRole).toString();
    deleteCategory(id);
    clearForm();
}

void CategoryManager::populateCategoryList() {
    ui->categoryListWidget->clear();
    QList<Category> categories = getAllCategories();
    for (const Category &c : categories) {
        QListWidgetItem *item = new QListWidgetItem(c.getName());
        item->setData(Qt::UserRole, c.getCategoryID());
        ui->categoryListWidget->addItem(item);
    }
}

void CategoryManager::addCategory(const Category &category) {
    DataManager::instance().addCategory(category);
}

void CategoryManager::updateCategory(const QString &categoryId, const Category &updatedCategory) {
    DataManager::instance().updateCategory(categoryId, updatedCategory);
}

void CategoryManager::deleteCategory(const QString &categoryId) {
    DataManager::instance().removeCategory(categoryId);
}

Category CategoryManager::getCategory(const QString &categoryId) const {
    return DataManager::instance().getCategory(categoryId);
}

QList<Category> CategoryManager::getAllCategories() const {
    return DataManager::instance().getCategories();
}
