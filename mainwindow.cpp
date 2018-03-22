#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <qDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->chooseFolderBtn, &QPushButton::clicked, this, &MainWindow::chooseFolderClicked);
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::onRun);

    connect(this, &MainWindow::transformedPng, this, &MainWindow::onTransformedPng);
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
        return;
    }

    ui->listWidget->clear();

    transformAllPngFiles(dir);
}

void MainWindow::onTransformedPng(const QString &pngFileName, bool success)
{
    if (success) {
        ui->listWidget->addItem(new QListWidgetItem(style()->standardIcon(QStyle::SP_ArrowForward), tr("%1 transforme ok").arg(pngFileName)));
    } else {
        ui->listWidget->addItem(new QListWidgetItem(style()->standardIcon(QStyle::SP_MessageBoxCritical), tr("%1 transforme error").arg(pngFileName)));
    }
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
              if (fileInfo.fileName().endsWith(".png")) {
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






