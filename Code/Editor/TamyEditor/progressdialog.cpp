#include "progressdialog.h"


///////////////////////////////////////////////////////////////////////////////

ProgressDialog::ProgressDialog( QWidget *parent, Qt::WFlags flags )
: QDialog(parent, flags)
{
   m_progressDialogUI.setupUi(this);

   m_progressDialogUI.progressBar->setMaximum(INT_MAX);
   m_progressDialogUI.progressBar->setMinimum(0);
   m_progressDialogUI.progressBar->setValue(0);

   show();
}

///////////////////////////////////////////////////////////////////////////////

void ProgressDialog::setProgress( float percentage )
{
   int val = ( int )( INT_MAX * percentage );
   m_progressDialogUI.progressBar->setValue( val );
}

///////////////////////////////////////////////////////////////////////////////
