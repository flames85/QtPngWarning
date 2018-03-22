#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    void transformedPng(const QString &pngFileName, bool success);

protected slots:

    void chooseFolderClicked();
    void onRun();
    void onTransformedPng(const QString &pngFileName, bool success);

    void onFolderLineTextChanged(const QString &);

private:

    void transformAllPngFiles(const QDir &dir);
    void clearTransformResult();


private:
    Ui::MainWindow      *ui;
    qint32              m_successNum;
};

#endif // MAINWINDOW_H
