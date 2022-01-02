//
// Created by sinosmart-dtt on 2021/12/18.
//

#ifndef TRANSLATE_MAINWINDOW_H
#define TRANSLATE_MAINWINDOW_H

#include "ThreadFileStruct.h"

#include <QMainWindow>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QThread mFileThread;
    QString mStringPath;

    typedef enum E_TRANS_TYPE {
        TRANS_TYPE_DIR,
        TRANS_TYPE_FILE,
        TRANS_TYPE_FILE_BUTT,
    }TRANS_TYPE;

    TRANS_TYPE mTransType;
    TRANS_PARAM mTransParam;
    void initData();
    void initViews();
    void initEvents();
    void initThreadFile();
    void initCheckBoxEvent();
    void initButtonEvent();
    void initLineEditEvent();
    void initComboBoxEvent();

signals:
    void signalTranslateFiles(const QString &path, const TRANS_PARAM &param);

private slots:
    void onPushButton();
    void onCheckBox();
    void onLineEdit();
    void onComboBox(int index);
};


#endif //TRANSLATE_MAINWINDOW_H
