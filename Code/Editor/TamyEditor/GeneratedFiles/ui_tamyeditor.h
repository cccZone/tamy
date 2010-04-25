/********************************************************************************
** Form generated from reading ui file 'tamyeditor.ui'
**
** Created: Sat 24. Apr 20:33:02 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TAMYEDITOR_H
#define UI_TAMYEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TamyEditorClass
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSelectAssetsDir;
    QAction *actionQuit;
    QAction *actionProperties;
    QAction *actionFrom_IWF;
    QAction *actionFrom_XML;
    QAction *actionImportFromIWF;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QFrame *renderWindow;
    QVBoxLayout *verticalLayout_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuImport;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TamyEditorClass)
    {
        if (TamyEditorClass->objectName().isEmpty())
            TamyEditorClass->setObjectName(QString::fromUtf8("TamyEditorClass"));
        TamyEditorClass->resize(564, 368);
        TamyEditorClass->setContextMenuPolicy(Qt::NoContextMenu);
        actionOpen = new QAction(TamyEditorClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(TamyEditorClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSelectAssetsDir = new QAction(TamyEditorClass);
        actionSelectAssetsDir->setObjectName(QString::fromUtf8("actionSelectAssetsDir"));
        actionQuit = new QAction(TamyEditorClass);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionProperties = new QAction(TamyEditorClass);
        actionProperties->setObjectName(QString::fromUtf8("actionProperties"));
        actionFrom_IWF = new QAction(TamyEditorClass);
        actionFrom_IWF->setObjectName(QString::fromUtf8("actionFrom_IWF"));
        actionFrom_XML = new QAction(TamyEditorClass);
        actionFrom_XML->setObjectName(QString::fromUtf8("actionFrom_XML"));
        actionImportFromIWF = new QAction(TamyEditorClass);
        actionImportFromIWF->setObjectName(QString::fromUtf8("actionImportFromIWF"));
        centralWidget = new QWidget(TamyEditorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setEnabled(true);
        centralWidget->setContextMenuPolicy(Qt::NoContextMenu);
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setMargin(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        renderWindow = new QFrame(centralWidget);
        renderWindow->setObjectName(QString::fromUtf8("renderWindow"));
        renderWindow->setEnabled(true);
        renderWindow->setMouseTracking(true);
        renderWindow->setContextMenuPolicy(Qt::NoContextMenu);
        renderWindow->setFrameShape(QFrame::StyledPanel);
        renderWindow->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(renderWindow);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setMargin(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        horizontalLayout->addWidget(renderWindow);

        TamyEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TamyEditorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 564, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuImport = new QMenu(menuFile);
        menuImport->setObjectName(QString::fromUtf8("menuImport"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        TamyEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TamyEditorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        TamyEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TamyEditorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        TamyEditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(menuImport->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuImport->addAction(actionImportFromIWF);

        retranslateUi(TamyEditorClass);

        QMetaObject::connectSlotsByName(TamyEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *TamyEditorClass)
    {
        TamyEditorClass->setWindowTitle(QApplication::translate("TamyEditorClass", "TamyEditor", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("TamyEditorClass", "Open", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("TamyEditorClass", "Save", 0, QApplication::UnicodeUTF8));
        actionSelectAssetsDir->setText(QApplication::translate("TamyEditorClass", "Select assets dir", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("TamyEditorClass", "Quit", 0, QApplication::UnicodeUTF8));
        actionProperties->setText(QApplication::translate("TamyEditorClass", "Properties", 0, QApplication::UnicodeUTF8));
        actionFrom_IWF->setText(QApplication::translate("TamyEditorClass", "From IWF", 0, QApplication::UnicodeUTF8));
        actionFrom_XML->setText(QApplication::translate("TamyEditorClass", "From XML", 0, QApplication::UnicodeUTF8));
        actionImportFromIWF->setText(QApplication::translate("TamyEditorClass", "From IWF", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("TamyEditorClass", "File", 0, QApplication::UnicodeUTF8));
        menuImport->setTitle(QApplication::translate("TamyEditorClass", "Import", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("TamyEditorClass", "View", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TamyEditorClass: public Ui_TamyEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAMYEDITOR_H
