#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <QWidget>
#include <QString>

namespace Ui {
class SaveLoad;
}

class SaveLoad : public QWidget
{
    Q_OBJECT

public:
    explicit SaveLoad(QWidget *parent = nullptr);
    ~SaveLoad();

    bool writeToFile(const QString &filePath);
    bool readFromFile(const QString &filePath);

signals:
    void goToMainMenu();

private slots:
    void on_homeBtn_clicked();

private:
    Ui::SaveLoad *ui;
};

#endif // SAVELOAD_H
