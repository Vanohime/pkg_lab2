#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QStringList>
#include <QVector>
#include <QDebug>
QVector<QString> getFilesAndFolders(const QString& folderPath) {
    QVector<QString> items;
    QDir dir(folderPath);

    // Проверяем, существует ли директория
    if (!dir.exists()) {
        return items; // Возвращаем пустой массив, если директория не существует
    }

    // Получаем список всех файлов и папок
    QStringList filters;
    filters << "*"; // Все файлы и папки
    dir.setNameFilters(filters);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    // Получаем список и добавляем в вектор
    QFileInfoList fileInfoList = dir.entryInfoList();
    for (const QFileInfo& fileInfo : fileInfoList) {
        items.append(fileInfo.absoluteFilePath());
    }

    return items;
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setFilter(QDir::QDir::AllEntries);
    model->setRootPath("");
    ui->listView->setModel(model);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_editingFinished()
{
    QVector<QString> arr = getFilesAndFolders(ui->lineEdit->text());
    for(auto a : arr)
        qDebug() << a << '\n';

}


void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QListView* listView = (QListView*)sender();
    QFileInfo fileInfo = model->fileInfo(index);
    ui->lineEdit->setText(model->filePath(index));
    if(fileInfo.fileName() == "..")
    {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        model->index(dir.absolutePath());
        listView->setRootIndex(model->index(dir.absolutePath()));
    }
    else if (fileInfo.fileName() == ".")
    {
        listView->setRootIndex(model->index(""));
    }

    else if(fileInfo.isDir())
    {
        listView->setRootIndex(index);
    }
}

