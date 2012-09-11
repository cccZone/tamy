#include "progressdialog.h"
#include "tamyeditor.h"
#include <QProgressBar>


///////////////////////////////////////////////////////////////////////////////

ProgressDialog::ProgressDialog()
{
   m_progressBar = TamyEditor::getInstance().getProgressBar();
}

///////////////////////////////////////////////////////////////////////////////

ProgressDialog::~ProgressDialog()
{
   m_progressBar->reset();
}

///////////////////////////////////////////////////////////////////////////////

void ProgressDialog::initialize( const std::string& tag, unsigned int elemsCount )
{
   m_progressBar->setRange( 0, elemsCount );
   m_progressBar->setValue( 0 );
}

///////////////////////////////////////////////////////////////////////////////

void ProgressDialog::advance()
{
   unsigned int currVal = m_progressBar->value();
   m_progressBar->setValue( currVal + 1 );
}

///////////////////////////////////////////////////////////////////////////////
