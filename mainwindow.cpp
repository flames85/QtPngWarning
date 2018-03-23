#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <qDebug>
#include <QMessageBox>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_successNum(0)
{
    ui->setupUi(this);

    // choose folder btn
    connect(ui->chooseFolderBtn, &QPushButton::clicked, this, &MainWindow::chooseFolderClicked);

    // run btn
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::onRun);
    ui->runBtn->setEnabled(false);


    // files table
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //! 设置选择行为，以行为单位
    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //! 设置选择模式，选择单行
    ui->tableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers ); //! 禁止编辑
    //    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0, 400);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Item") << tr("status"));
    ui->tableWidget->setWordWrap(false);

    // address combobox
    ui->addressComboBox->setEditable(true);
    ui->addressComboBox->setLineEdit(new QLineEdit(this));
    ui->addressComboBox->lineEdit()->setPlaceholderText(tr("Absolute path of target directory"));
    connect(ui->addressComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(onAddressTextChanged(const QString &)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseFolderClicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, tr("Choose folder"));

    if (-1 == ui->addressComboBox->findText(folderName)) {
        ui->addressComboBox->addItem(folderName);
    }
    ui->addressComboBox->setCurrentText(folderName);
}

void MainWindow::onRun()
{
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem * item = ui->tableWidget->item(row, 0);
        QTableWidgetItem * status = ui->tableWidget->item(row, 1);
        qDebug()<< "transforme png :" << item->text();
        QImage png = QImage(item->text());
        if (png.save(item->text())) {
            item->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
            status->setText(tr("OK"));
            ++m_successNum;
        } else {
            item->setIcon(style()->standardIcon(QStyle::SP_MessageBoxCritical));
            status->setText(tr("ERROR"));
        }
    }
    ui->statusBar->showMessage(tr("Total transforme %1, success %2").arg(ui->tableWidget->rowCount()).arg(m_successNum));
}

void MainWindow::onAddressTextChanged(const QString &)
{
    if (!ui->addressComboBox->currentText().isEmpty()) {
        QDir dir(ui->addressComboBox->currentText());
        if (!dir.exists()) {
            QMessageBox box(QMessageBox::Warning, tr("Warning"), tr("The folder you choose do not exist"), QMessageBox::Ok, this);
            box.exec();
            return;
        }

        clearTransformResult();

        qDebug() << "Start do track folder :" << dir.dirName();
        trackAllPngFiles(dir);

        ui->runBtn->setEnabled(true);
    } else {
        ui->runBtn->setEnabled(false);
    }
}

void MainWindow::trackAllPngFiles(const QDir &dir)
{
    if (dir.exists()) {

        QFileInfoList fileInfos = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        for (QFileInfo fileInfo : fileInfos)
        {
            if(fileInfo.isFile())
            {
                if (fileInfo.fileName().endsWith(".png", Qt::CaseInsensitive)) {

                    int rowCount = ui->tableWidget->rowCount();
                    ui->tableWidget->setRowCount(rowCount + 1);
                    ui->tableWidget->setItem(rowCount , 0, createTableWidgetItem(fileInfo.filePath(), Qt::AlignLeft | Qt::AlignVCenter, &(style()->standardIcon(QStyle::SP_MessageBoxQuestion))));
                    ui->tableWidget->setItem(rowCount , 1, createTableWidgetItem("N/A", Qt::AlignCenter));
                }
            }
            else
            {
                QDir childDir(fileInfo.filePath());
                trackAllPngFiles(childDir);
            }
        }
    }
    ui->statusBar->showMessage(tr("Total %1").arg(ui->tableWidget->rowCount()));
}

void MainWindow::clearTransformResult()
{
    ui->statusBar->clearMessage();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    m_successNum = 0;
}


QTableWidgetItem* MainWindow::createTableWidgetItem(const QString &text, int alignment, QIcon *icon)
{
    QTableWidgetItem* item = NULL;
    if (icon) {
        item = new QTableWidgetItem(*icon, text);
    } else {
        item = new QTableWidgetItem(text);
    }

    item->setTextAlignment(alignment);
    item->setToolTip(text);
    return item;
}






