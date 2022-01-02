//
// Created by sinosmart-dtt on 2021/12/18.
//

#ifndef TRANSLATE_THREADFILE_H
#define TRANSLATE_THREADFILE_H

#include "ThreadFileStruct.h"

#include <QObject>
#include <QFileInfoList>
#include <QtCore5Compat/QTextCodec>


class ThreadFile : public QObject {
    Q_OBJECT
public:
    explicit ThreadFile(QObject *parent = nullptr);
    ~ThreadFile() override;

private:
    QTextCodec *GBK;
    QTextCodec *UTF8;
    QFileInfoList getFileList(const QString &path, const QStringList &filterList);
    bool transFile(const QString &abPath, const TRANS_PARAM &param);

signals:
    void signalFileTranslateStatus(bool value);

public slots:
    void onThreadFileInit();
    void onThreadFileTranslateStart(const QString &path, const TRANS_PARAM &param);

};


#endif //TRANSLATE_THREADFILE_H
