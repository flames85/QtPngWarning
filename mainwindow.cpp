#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <qDebug>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_successNum(0)
{
    ui->setupUi(this);

    connect(ui->chooseFolderBtn, &QPushButton::clicked, this, &MainWindow::chooseFolderClicked);
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::onRun);
    connect(this, &MainWindow::transformedPng, this, &MainWindow::onTransformedPng);
    connect(ui->folderLine, SIGNAL(textChanged(const QString &)), this, SLOT(onFolderLineTextChanged(const QString &)));
    ui->runBtn->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseFolderClicked()
{
    QString folderName = QFileDialog::getExistingDirectory(this, tr("Choose folder"));
    ui->folderLine->setText(folderName);
}

void MainWindow::onRun()
{
    QDir dir(ui->folderLine->text());
    if (!dir.exists()) {
        QMessageBox box(QMessageBox::Warning, tr("Warning"), tr("The folder you choose do not exist"), QMessageBox::Ok, this);
        box.exec();
        return;
    }

    clearTransformResult();

    transformAllPngFiles(dir);
}

void MainWindow::onTransformedPng(const QString &pngFileName, bool success)
{
    if (success) {
        ui->listWidget->addItem(new QListWidgetItem(style()->standardIcon(QStyle::SP_ArrowForward), tr("%1 transforme ok").arg(pngFileName)));
        ++m_successNum;
    } else {
        ui->listWidget->addItem(new QListWidgetItem(style()->standardIcon(QStyle::SP_MessageBoxCritical), tr("%1 transforme error").arg(pngFileName)));
        ui->statusBar->showMessage(tr("Total transforme %1, success %2").arg(ui->listWidget->count()).arg(m_successNum));
    }
    ui->statusBar->showMessage(tr("Total transforme %1, success %2").arg(ui->listWidget->count()).arg(m_successNum));
}

void MainWindow::onFolderLineTextChanged(const QString &)
{
    ui->runBtn->setEnabled(!ui->folderLine->text().isEmpty());
}

void MainWindow::transformAllPngFiles(const QDir &dir)
{
    qDebug() << "Folder :" << dir.dirName();

    if (dir.exists()) {

        QFileInfoList fileInfos = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        for (QFileInfo fileInfo : fileInfos)
        {
          if(fileInfo.isFile())
          {
              if (fileInfo.fileName().endsWith(".png", Qt::CaseInsensitive)) {
                  qDebug()<< "png :" << fileInfo.filePath();
                  QImage png = QImage(fileInfo.filePath());
                  if (png.save(fileInfo.filePath())) {
                      emit transformedPng(fileInfo.filePath(), true);
                  } else {
                      emit transformedPng(fileInfo.filePath(), false);
                  }
              }
          }
          else
          {
              QDir childDir(fileInfo.filePath());
              transformAllPngFiles(childDir);
          }
        }
    }
}

void MainWindow::clearTransformResult()
{
    ui->listWidget->clear();
    m_successNum = 0;
}






