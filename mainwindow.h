#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

namespace Ui {
class MainWindow;
}

class QTableWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

protected slots:

    void chooseFolderClicked();
    void onRun();
    void onAddressTextChanged(const QString &);

private:

    void trackAllPngFiles(const QDir &dir);
    void clearTransformResult();

    QTableWidgetItem* createTableWidgetItem(const QString &text, int alignment, QIcon *icon = NULL);

private:
    Ui::MainWindow      *ui;
    qint32              m_successNum;
};

#endif // MAINWINDOW_H
