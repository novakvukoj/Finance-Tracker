#include "saveLoad.h"
#include "ui_saveLoad.h"
#include "src/modules/datamanager.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>
#include <QDebug>

SaveLoad::SaveLoad(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SaveLoad)
{
    ui->setupUi(this);
}

SaveLoad::~SaveLoad()
{
    delete ui;
}

bool SaveLoad::writeToFile(const QString& filePath)
{
    DataManager& dm = DataManager::instance();
    QJsonObject root;

    QJsonArray goalsArray;
    for (const Goal& g : dm.getGoals()) {
        QJsonObject obj;
        obj["name"] = g.getName();
        obj["target"] = g.getTarget();
        obj["currentAmount"] = g.getCurrentAmount();
        obj["startDate"] = g.getStartDate().toString(Qt::ISODate);
        obj["endDate"] = g.getEndDate().toString(Qt::ISODate);
        goalsArray.append(obj);
    }
    root["goals"] = goalsArray;

    QJsonArray categoriesArray;
    for (const Category& c : dm.getCategories()) {
        QJsonObject obj;
        obj["id"] = c.getCategoryID();
        obj["name"] = c.getCategoryName();
        obj["startDate"] = c.getStartDate().toString(Qt::ISODate);
        categoriesArray.append(obj);
    }
    root["categories"] = categoriesArray;

    QJsonArray transactionsArray;
    for (const Transaction& t : dm.getTransactions()) {
        QJsonObject obj;
        obj["id"] = t.getId();
        obj["amount"] = t.getAmount();
        obj["inputDate"] = t.getInputDate().toString(Qt::ISODate);
        obj["targetType"] = static_cast<int>(t.getTargetType());
        obj["targetId"] = t.getTargetId();
        transactionsArray.append(obj);
    }
    root["transactions"] = transactionsArray;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool SaveLoad::readFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file for reading:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON in file:" << filePath;
        return false;
    }

    QJsonObject root = doc.object();
    DataManager& dm = DataManager::instance();
    dm.clear();

    if (root.contains("goals") && root["goals"].isArray()) {
        QJsonArray goalsArray = root["goals"].toArray();
        for (const QJsonValue& val : goalsArray) {
            if (!val.isObject()) continue;
            QJsonObject obj = val.toObject();

            Goal g;
            g.setName(obj["name"].toString());
            g.setTarget(obj["target"].toDouble());
            g.setCurrentAmount(obj["currentAmount"].toDouble());
            g.setStartDate(QDate::fromString(obj["startDate"].toString(), Qt::ISODate));
            g.setEndDate(QDate::fromString(obj["endDate"].toString(), Qt::ISODate));

            dm.addGoal(g);
        }
    }

    if (root.contains("categories") && root["categories"].isArray()) {
        QJsonArray categoriesArray = root["categories"].toArray();
        for (const QJsonValue& val : categoriesArray) {
            if (!val.isObject()) continue;
            QJsonObject obj = val.toObject();

            Category c;
            c.setCategoryID(obj["id"].toString());
            c.setCategoryName(obj["name"].toString());
            c.setStartDate(QDate::fromString(obj["startDate"].toString(), Qt::ISODate));

            dm.addCategory(c);
        }
    }

    if (root.contains("transactions") && root["transactions"].isArray()) {
        QJsonArray transactionsArray = root["transactions"].toArray();
        for (const QJsonValue& val : transactionsArray) {
            if (!val.isObject()) continue;
            QJsonObject obj = val.toObject();

            Transaction t;
            t.setId(obj["id"].toString());
            t.setAmount(obj["amount"].toDouble());
            t.setInputDate(QDate::fromString(obj["inputDate"].toString(), Qt::ISODate));
            t.setTargetType(static_cast<TransactionTargetType>(obj["targetType"].toInt()));
            t.setTargetId(obj["targetId"].toString());

            dm.addTransaction(t);
        }
    }

    return true;
}

void SaveLoad::on_homeBtn_clicked()
{
    emit goToMainMenu();
}

