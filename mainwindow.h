#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Calculate_clicked();
    void updateProgressBar();
    void displayResults();

private:
    Ui::MainWindow *ui;
    QString FindLongest();
    int CountSigns();
    int CountLetter();
    void Loading();

    int progressBarValue;
    QTimer *timer;
    bool loadingComplete;
};

#endif // MAINWINDOW_H
