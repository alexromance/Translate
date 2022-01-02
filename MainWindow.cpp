//
// Created by sinosmart-dtt on 2021/12/18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ThreadFile.h"

#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow),
        mTransType(TRANS_TYPE_FILE_BUTT) {
    ui->setupUi(this);
    initData();
    initViews();
    initEvents();
//    emit signalTranslateFiles("./test/kf32a156_bkp.c");
}

MainWindow::~MainWindow() {
    mFileThread.quit();
    mFileThread.wait();
    delete ui;
    qDebug()<<"~~~~~ MainWindow";
}

void MainWindow::initData() {
    mTransType = TRANS_TYPE_DIR;
    mStringPath.clear();
    qRegisterMetaType<TRANS_PARAM>("TRANS_PARAM");
    memset(&mTransParam, 0, sizeof(mTransParam));
}

void MainWindow::initViews() {

}

void MainWindow::initEvents() {
    initCheckBoxEvent();
    initButtonEvent();
    initLineEditEvent();
    initComboBoxEvent();
    initThreadFile();
}

void MainWindow::initThreadFile() {
    auto fileTask = new ThreadFile;
    fileTask->moveToThread(&mFileThread);

    connect(&mFileThread, &QThread::started, fileTask, &ThreadFile::onThreadFileInit);
    connect(fileTask, &ThreadFile::signalFileTranslateStatus, this, [] (bool value) {
        qDebug()<<" status is "<<value;
//        QMessageBox::information(nullptr,
//                                          trUtf8("转换结果"),
//                                          trUtf8(value ? "转换成功" : "转换失败"),
//                                          QMessageBox::Yes);
    });
    connect(&mFileThread, &QThread::finished, fileTask, &ThreadFile::deleteLater);
    connect(this, &MainWindow::signalTranslateFiles, fileTask, &ThreadFile::onThreadFileTranslateStart);


    mFileThread.start();
}

void MainWindow::initCheckBoxEvent() {
    auto dataList = findChildren<QCheckBox*>();
    qDebug()<<"check box is "<<dataList;
    for (int i = 0; i < dataList.size(); ++i) {
        auto data = qobject_cast<QCheckBox*>(dataList.at(i));
        if (data) {
            connect(data, &QCheckBox::stateChanged, this, &MainWindow::onCheckBox);
        }
    }
}

void MainWindow::initButtonEvent() {
    auto dataList = findChildren<QPushButton*>();
    qDebug()<<"button is "<<dataList;
    for (int i = 0; i < dataList.size(); ++i) {
        auto data = qobject_cast<QPushButton*>(dataList.at(i));
        if (data) {
            connect(data, &QPushButton::clicked, this, &MainWindow::onPushButton);
        }
    }
}

void MainWindow::initLineEditEvent() {
    auto dataList = findChildren<QLineEdit*>();
    qDebug()<<"line edit is "<<dataList;
    for (int i = 0; i < dataList.size(); ++i) {
        auto data = qobject_cast<QLineEdit*>(dataList.at(i));
        if (data) {
            connect(data, &QLineEdit::textChanged, this, &MainWindow::onLineEdit);
        }
    }
}

void MainWindow::initComboBoxEvent() {
    auto dataList = findChildren<QComboBox*>();
    qDebug()<<"comboBox is "<<dataList;
    for (int i = 0; i < dataList.size(); ++i) {
        auto data = qobject_cast<QComboBox*>(dataList.at(i));
        if (data) {
//            connect(data, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::onCheckBox);
            connect(data, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBox);
        }
    }
}

void MainWindow::onPushButton() {
    auto sends = sender();
    if (ui->pushButtonType == sends) {
        QString string;
        switch (mTransType) {
            case TRANS_TYPE_DIR:
                string = QFileDialog::getExistingDirectory(this, "打开文件夹", ".");
                break;
            case TRANS_TYPE_FILE:
                string = QFileDialog::getOpenFileName(this, "打开文件", ".");
                break;
            case TRANS_TYPE_FILE_BUTT:
                break;
        }
        qDebug()<<"type is "<<string;
        if (!string.isNull()) {
            ui->lineEditType->setText(string);
            mStringPath = string;
            ui->lineEditOutput->setText(mTransParam.bCoverFile ? string : nullptr);
        }
    } else if (ui->pushButtonTranslate == sends) {
        if (!mStringPath.isNull()) {
            emit signalTranslateFiles(mStringPath, mTransParam);
        }
    }
}

void MainWindow::onCheckBox() {
    auto sends = sender();
    if (ui->checkBoxCover == sends) {
        bool value = ui->checkBoxCover->isChecked();
        mTransParam.bCoverFile = value;
        ui->checkBoxCoverDirectory->setCheckState(value ? Qt::Checked : Qt::Unchecked);
        ui->lineEditOutput->setText(value ? mStringPath : nullptr);
    } else if (ui->checkBoxWithBOM == sends) {
        bool value = ui->checkBoxWithBOM->isChecked();
        mTransParam.bBOM = value;
    } else if (ui->checkBoxRecurseDirectory == sends) {
        bool value = ui->checkBoxRecurseDirectory->isChecked();
        mTransParam.bRecuse = value;
    } else if (ui->checkBoxCoverDirectory == sends) {
        bool value = ui->checkBoxCoverDirectory->isChecked();
        mTransParam.bCoverDir = value;
    } else if (ui->checkBoxSuccessOpen == sends) {
        bool value = ui->checkBoxSuccessOpen->isChecked();
        mTransParam.bSuccessOpen = value;
    }
}

void MainWindow::onLineEdit() {

}

void MainWindow::onComboBox(int index) {
    qDebug()<<" current is "<<index;
    switch (index) {
        case TRANS_TYPE_FILE:
            qDebug()<<"file";
            break;
        case TRANS_TYPE_DIR:
            qDebug()<<"file dir";
            break;
        default:
            break;
    }
    mTransType = static_cast<TRANS_TYPE>(index);
}

