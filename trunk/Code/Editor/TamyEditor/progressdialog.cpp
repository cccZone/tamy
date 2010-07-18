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

void ProgressDialog::initialize( const std::string& tag, unsigned int elemsCount )
{
   m_progressDialogUI.caption->setText( tag.c_str() );
   m_progressDialogUI.progressBar->setMaximum( elemsCount );
   m_progressDialogUI.progressBar->setValue( 0 );
}

///////////////////////////////////////////////////////////////////////////////

void ProgressDialog::advance()
{
   unsigned int currVal = m_progressDialogUI.progressBar->value();
   m_progressDialogUI.progressBar->setValue( currVal + 1 );
}

///////////////////////////////////////////////////////////////////////////////
