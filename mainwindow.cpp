#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <stdexcept>
#include <QRegularExpression>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , progressBarValue(0)
    , timer(new QTimer(this))
    , loadingComplete(false)
{
    ui->setupUi(this);
    ui->progressBar->hide();

    connect(timer, &QTimer::timeout, this, &MainWindow::updateProgressBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::FindLongest() {
    QString input = ui->textEdit_input->toPlainText().trimmed();
    if (input.isEmpty()) {
        throw std::invalid_argument("В строке нет слов.");
    }
    QRegularExpression nonLetterRegex("[^а-яёА-ЯЁa-zA-Z\\s]");
    QString words = input;
    words.replace(nonLetterRegex, "");
    int startPos = 0;
    int maxLen = 0;
    QString longestWords;

    for (int i = 0; i < words.length(); i++) {
        if (words.at(i) == ' ' || i == words.length() - 1) {
            int len = (i == words.length() - 1) ? i - startPos + 1 : i - startPos;
            if (len > maxLen) {
                maxLen = len;
                longestWords = words.mid(startPos, len);
            } else if (len == maxLen) {
                longestWords += " " + words.mid(startPos, len);
            }
            startPos = i + 1;
        }
    }

    return longestWords;
}

int MainWindow::CountSigns() {
    QString Words = ui->textEdit_input->toPlainText();
    int count = 0;
    for (int i = 0; i < Words.length(); ++i) {
        if (Words.at(i).isPunct() || Words.at(i).isSymbol()) {
            ++count;
        }
    }

    return count;
}

int MainWindow::CountLetter() {
    QString Words = ui->textEdit_input->toPlainText();
    int count = 0;
    for (int i = 0; i < Words.length(); ++i) {
        if (Words.at(i).isLetter()) {
            ++count;
        }
    }

    return count;
}

void MainWindow::Loading() {
    ui->progressBar->setValue(0);
    ui->progressBar->show();
    progressBarValue = 0;
    loadingComplete = false;
    timer->start(20);
}

void MainWindow::updateProgressBar() {
    progressBarValue += 1;
    ui->progressBar->setValue(progressBarValue);

    int red = 5 + (progressBarValue * 2.5);
    int blue = 184 - (progressBarValue * 1.84);
    int green = 204 - (progressBarValue * 2.04);

    ui->progressBar->setStyleSheet(
        QString("QProgressBar {"
                "    border: 2px solid grey;"
                "    border-radius: 5px;"
                "    text-align: center;"
                "}"
                "QProgressBar::chunk {"
                "    background-color: rgb(%1, %2, %3);"
                "    width: 20px;"
                "}").arg(red).arg(green).arg(blue)
        );

    if (progressBarValue >= 100) {
        timer->stop();
        ui->progressBar->hide();
        loadingComplete = true;
        displayResults();
    }
}

void MainWindow::displayResults() {
    try {
        QString longestWords = FindLongest();
        int signCount = CountSigns();
        ui->textBrowser_output->setText("Наибольшие слова: " + longestWords
                                        + "\nКоличество знаков: " + QString::number(signCount)
                                        + "\nКоличество букв: " + QString::number(CountLetter()));
    } catch (const std::invalid_argument &e) {
        ui->textBrowser_output->setText(e.what());
    }
}

void MainWindow::on_pushButton_Calculate_clicked() {
    Loading();
}
