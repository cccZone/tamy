/********************************************************************************
** Form generated from reading ui file 'progressdialog.ui'
**
** Created: Sat 24. Apr 18:21:09 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PROGRESSDIALOG_H
#define UI_PROGRESSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QProgressBar>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProgressDialog
{
public:
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;

    void setupUi(QDialog *ProgressDialog)
    {
        if (ProgressDialog->objectName().isEmpty())
            ProgressDialog->setObjectName(QString::fromUtf8("ProgressDialog"));
        ProgressDialog->setWindowModality(Qt::ApplicationModal);
        ProgressDialog->resize(435, 45);
        ProgressDialog->setMinimumSize(QSize(435, 45));
        ProgressDialog->setMaximumSize(QSize(435, 45));
        ProgressDialog->setModal(true);
        verticalLayout = new QVBoxLayout(ProgressDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        progressBar = new QProgressBar(ProgressDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);


        retranslateUi(ProgressDialog);

        QMetaObject::connectSlotsByName(ProgressDialog);
    } // setupUi

    void retranslateUi(QDialog *ProgressDialog)
    {
        ProgressDialog->setWindowTitle(QApplication::translate("ProgressDialog", "Operation in progress...", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ProgressDialog);
    } // retranslateUi

};

namespace Ui {
    class ProgressDialog: public Ui_ProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSDIALOG_H
