#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SaveLoad/saveLoad.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QInputDialog>
#include <QDir>
#include <QFileDialog>
#include "src/modules/datamanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentSaveFile("")
{
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    goalManager = new GoalManager(this);
    summaryPage = new SummaryPage(this);
    summaryView = new SummaryView(this);
    transactionManager = new TransactionManager(this);
    categoryManager = new CategoryManager(this);
    saveLoad = new SaveLoad(this);

    ui->stackedWidget->addWidget(goalManager);
    ui->stackedWidget->addWidget(summaryPage);
    ui->stackedWidget->addWidget(transactionManager);
    ui->stackedWidget->addWidget(categoryManager);
    ui->stackedWidget->addWidget(saveLoad);
    ui->stackedWidget->addWidget(summaryView);

    connect(goalManager, &GoalManager::goToMainMenu, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(summaryPage, &SummaryPage::goToMainMenu, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(transactionManager, &TransactionManager::goToMainMenu, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(categoryManager, &CategoryManager::goToMainMenu, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(summaryPage, &SummaryPage::showSummaryView, this, [this](summary* data, bool isGoal) {
        if (isGoal) {
            summaryView->displayGoalGraph(data);
        } else {
            summaryView->displayCategoryGraph(data);
        }
        ui->stackedWidget->setCurrentWidget(summaryView);
    });

    connect(summaryView, &SummaryView::goBack, this, [this]() {
        ui->stackedWidget->setCurrentWidget(summaryPage);
    });

    connect(saveLoad, &SaveLoad::goToMainMenu, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);  // main menu index
    });

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Exit Application",
        "Are you sure you want to close the application?\nUnsaved data may be lost.",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::on_goalButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(goalManager);
}

void MainWindow::on_summaryButton_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(categoryManager);
}

void MainWindow::on_transactionButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(transactionManager);
}

void MainWindow::on_summaryButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(summaryPage);
}

void MainWindow::on_saveButton_clicked()
{
    SaveLoad saver;

    QDir dir("saves");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QStringList saveFiles = dir.entryList(QStringList() << "*.json", QDir::Files, QDir::Name);

    QString fileToSave;

    if (saveFiles.isEmpty()) {
        bool ok;
        QString fileName = QInputDialog::getText(
            this,
            "Save File",
            "Enter a name for your save file:",
            QLineEdit::Normal,
            "",
            &ok
            );

        if (!ok || fileName.isEmpty()) {
            QMessageBox::warning(this, "Save Cancelled", "No file name provided.");
            return;
        }

        fileToSave = dir.filePath(fileName + ".json");
    } else {
        bool ok;
        QStringList options = saveFiles;
        options << "New File...";
        QString chosen = QInputDialog::getItem(
            this,
            "Select Save File",
            "Choose a save file to overwrite or select 'New File...' to create a new save:",
            options,
            0,
            false,
            &ok
            );

        if (!ok) return;

        if (chosen == "New File...") {
            QString newFileName = QInputDialog::getText(
                this,
                "New Save File",
                "Enter a name for your new save file:",
                QLineEdit::Normal,
                "",
                &ok
                );

            if (!ok || newFileName.isEmpty()) {
                QMessageBox::warning(this, "Save Cancelled", "No file name provided.");
                return;
            }

            fileToSave = dir.filePath(newFileName + ".json");
        } else {
            fileToSave = dir.filePath(chosen);
        }
    }

    if (saver.writeToFile(fileToSave)) {
        currentSaveFile = fileToSave;
        QMessageBox::information(this, "Saved", "Data saved successfully to " + fileToSave);
    } else {
        QMessageBox::critical(this, "Error", "Failed to save data.");
    }
}

void MainWindow::on_loadButton_clicked()
{
    QDir dir("saves");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QStringList saveFiles = dir.entryList(QStringList() << "*.json", QDir::Files, QDir::Name);

    if (saveFiles.isEmpty()) {
        QMessageBox::information(this, "No Saves", "No save files found.");
        return;
    }

    bool ok;
    QString chosen = QInputDialog::getItem(
        this,
        "Load Save File",
        "Select a save file to load:",
        saveFiles,
        0,
        false,
        &ok
        );

    if (!ok || chosen.isEmpty()) return;

    QString fileToLoad = dir.filePath(chosen);
    SaveLoad loader;
    DataManager::instance().clear();
    if (loader.readFromFile(fileToLoad)) {
        currentSaveFile = fileToLoad;
        recreateManagers();
        QMessageBox::information(this, "Loaded", "Data loaded successfully from " + fileToLoad);
    } else {
        QMessageBox::critical(this, "Error", "Failed to load data.");
    }
}

void MainWindow::recreateManagers()
{
    ui->stackedWidget->removeWidget(goalManager);
    ui->stackedWidget->removeWidget(summaryPage);
    ui->stackedWidget->removeWidget(transactionManager);
    ui->stackedWidget->removeWidget(categoryManager);

    delete goalManager;
    delete summaryPage;
    delete transactionManager;
    delete categoryManager;

    goalManager = new GoalManager(this);
    summaryPage = new SummaryPage(this);
    transactionManager = new TransactionManager(this);
    categoryManager = new CategoryManager(this);

    ui->stackedWidget->addWidget(goalManager);
    ui->stackedWidget->addWidget(summaryPage);
    ui->stackedWidget->addWidget(transactionManager);
    ui->stackedWidget->addWidget(categoryManager);

    connect(goalManager, &GoalManager::goToMainMenu, this, [this]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(summaryPage, &SummaryPage::goToMainMenu, this, [this]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(transactionManager, &TransactionManager::goToMainMenu, this, [this]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(categoryManager, &CategoryManager::goToMainMenu, this, [this]() { ui->stackedWidget->setCurrentIndex(0); });
    connect(summaryPage, &SummaryPage::showSummaryView, this, [this](summary* data, bool isGoal) {
        if (isGoal) {
            summaryView->displayGoalGraph(data);
        } else {
            summaryView->displayCategoryGraph(data);
        }
        ui->stackedWidget->setCurrentWidget(summaryView);
    });
}


void MainWindow::on_deleteSaveButton_clicked()
{
    QDir dir("saves");
    if (!dir.exists()) {
        QMessageBox::information(this, "No Saves", "No save files found.");
        return;
    }

    QStringList saveFiles = dir.entryList(QStringList() << "*.json", QDir::Files, QDir::Name);
    if (saveFiles.isEmpty()) {
        QMessageBox::information(this, "No Saves", "No save files found.");
        return;
    }

    bool ok;
    QString chosen = QInputDialog::getItem(
        this,
        "Delete Save File",
        "Select a save file to delete:",
        saveFiles,
        0,
        false,
        &ok
        );

    if (!ok || chosen.isEmpty()) return;

    QString fileToDelete = dir.filePath(chosen);
    if (QFile::remove(fileToDelete)) {
        QMessageBox::information(this, "Deleted", "Save file deleted successfully.");
        if (currentSaveFile == fileToDelete) {
            currentSaveFile.clear();
        }
        DataManager::instance().clear();
        recreateManagers();
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete the save file.");
    }
}


void MainWindow::on_deleteSaveButton_2_clicked()
{
    DataManager::instance().clear();
    recreateManagers();

    QMessageBox::information(
        this,
        "New File Ready",
        "All data has been cleared. You can start a new file now."
        );
}

