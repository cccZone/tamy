#pragma once

/// @file   TamyEditor\progressdialog.h
/// @brief  editor's main window class


#include <QtGui/QDialog>
#include "ui_progressdialog.h"


///////////////////////////////////////////////////////////////////////////////

/**
* This is a dialog that shows the progress of an operation.
*/
class ProgressDialog : public QDialog
{
   Q_OBJECT

private:
   Ui::ProgressDialog m_progressDialogUI;
   float m_progressPercentage;

public:
   /**
    * Constructor.
    */
   ProgressDialog( QWidget *parent = 0, Qt::WFlags flags = 0 );

   /**
    * Sets the progress on the bar.
    *
    * @param percentage    value in range <0, 1>
    */
   void setProgress( float percentage );
};

///////////////////////////////////////////////////////////////////////////////
