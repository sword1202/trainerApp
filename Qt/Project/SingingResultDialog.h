//
// Created by Semyon Tikhonenko on 2018-12-16.
//

#ifndef VOCALTRAINER_SINGINGRESULTDIALOG_H
#define VOCALTRAINER_SINGINGRESULTDIALOG_H

#include <QDialog>

class SingingResultDialog : public QDialog {
    Q_OBJECT
public:
    explicit SingingResultDialog(QWidget *parent);
    Q_INVOKABLE void save();
    Q_INVOKABLE void again();
};


#endif //VOCALTRAINER_SINGINGRESULTDIALOG_H
