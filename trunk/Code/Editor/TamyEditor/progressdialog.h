#pragma once

/// @file   TamyEditor\progressdialog.h
/// @brief  editor's main window class


#include <QtGui/QDialog>
#include "ui_progressdialog.h"
#include "core/IProgressObserver.h"


///////////////////////////////////////////////////////////////////////////////

/**
* This is a dialog that shows the progress of an operation.
*/
class ProgressDialog : public QDialog, public IProgressObserver
{
   Q_OBJECT

private:
   Ui::ProgressDialog   m_progressDialogUI;

public:
   /**
    * Constructor.
    */
   ProgressDialog( QWidget *parent = 0, Qt::WFlags flags = 0 );

   // -------------------------------------------------------------------------
   // IProgressObserver implementation
   // -------------------------------------------------------------------------
   void initialize( const std::string& tag, unsigned int elemsCount );
   void advance();
};

///////////////////////////////////////////////////////////////////////////////
