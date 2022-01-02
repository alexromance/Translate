//
// Created by sinosmart-dtt on 2021/12/18.
//

#include "ThreadFile.h"

#include <QDebug>
#include <QDir>

ThreadFile::ThreadFile(QObject *parent) :
    QObject(parent),
    GBK(nullptr),
    UTF8(nullptr) {
    GBK = QTextCodec::codecForName("GBK");
    UTF8 = QTextCodec::codecForName("UTF-8");
}

ThreadFile::~ThreadFile() {
    qDebug()<<"~~~~~ ThreadFile";
}

void ThreadFile::onThreadFileInit() {

}

void ThreadFile::onThreadFileTranslateStart(const QString &path, const TRANS_PARAM &param) {
    qDebug()<<" file path is "<<path<<" BOM, recurse, cover, coverDir, successOpen"
    <<param.bBOM<<param.bRecuse<<param.bCoverFile<<param.bCoverDir<<param.bSuccessOpen;
    QDir dir(path);
    if (dir.exists()) {
        QStringList nameFilters;
        nameFilters<<"*.c"<<"*.h"<<"*.cpp"<<"*.hpp"<<"*.s"<<"*.S";
//        nameFilters<<"*.h";
        if (param.bRecuse) {
            qDebug()<<"file list -R size is "<<getFileList(path, nameFilters).size()<<getFileList(path, nameFilters);
            auto dataList = getFileList(path, nameFilters);
            for (int i = 0; i < dataList.size(); ++i) {
                const auto &data = dataList.at(i);
                qDebug()<<" data ab path is "<<data.absoluteFilePath();
                transFile(data.absoluteFilePath(), param);
            }
            emit signalFileTranslateStatus(true);

        } else {
            QFileInfoList resultList = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
            qDebug()<<"file list is "<<resultList;
            qDebug()<<"file list size is "<<resultList.size();
//            for (int i = 0; i < resultList.size(); ++i) {
//                fileInfoList.append(QFileInfo(resultList.at(i)));
//            }

            for (int i = 0; i < resultList.size(); ++i) {
                qDebug()<<" no re file "<<resultList.at(i).absoluteFilePath();
                transFile(QFileInfo(resultList.at(i)).absoluteFilePath(), param);
            }
            emit signalFileTranslateStatus(true);
        }
    } else {
        QFileInfo fileInfo(path);
        if (fileInfo.exists()) {
            qDebug()<<" this is a file "<<fileInfo.absoluteFilePath();
            transFile(fileInfo.absoluteFilePath(), param);
            emit signalFileTranslateStatus(true);
            qDebug()<<" return from dir null & file null";
            return;
        }
        emit signalFileTranslateStatus(false);
    }
}

QFileInfoList ThreadFile::getFileList(const QString &path, const QStringList &filterList) {
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++) {
        QString name = folder_list.at(i).absoluteFilePath();
        qDebug()<<"folder is "<<name<<i;
        QFileInfoList child_file_list = getFileList(name, filterList);
        file_list.append(child_file_list);
    }

    return file_list;
}

bool ThreadFile::transFile(const QString &abPath, const TRANS_PARAM &param) {
    QFile file(abPath);
    if (file.open(QFile::ReadWrite)) {
        QTextCodec::ConverterState state;
        const QByteArray &array = file.readAll();
        const QString &text = GBK->toUnicode(array, array.size(), &state);
        QString fileName = (param.bCoverFile ? abPath : QString(abPath).append(".New"));
        QFile fileNew(fileName);
        qDebug()<<" file new is "<<fileName;
        if (fileNew.open(QFile::ReadWrite)) {
            qint64 ret = fileNew.write(UTF8->fromUnicode(GBK->toUnicode(text.toLocal8Bit()).data()));
            if (ret > 0) {
                qDebug()<<" write success";
            }
            fileNew.close();
        } else {
            return false;
        }
        file.close();
    } else {
        return false;
    }
    return true;
}

