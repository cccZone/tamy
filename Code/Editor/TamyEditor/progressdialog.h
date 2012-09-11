/// @file   TamyEditor\progressdialog.h
/// @brief  This class operates the main window's progress bar
#pragma once

#include "core/IProgressObserver.h"


///////////////////////////////////////////////////////////////////////////////

class QProgressBar;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class operates the main window's progress bar.
 */
class ProgressDialog :  public IProgressObserver
{
private:
   QProgressBar*     m_progressBar;

public:
   /**
    * Constructor.
    */
   ProgressDialog();
   ~ProgressDialog();

   // -------------------------------------------------------------------------
   // IProgressObserver implementation
   // -------------------------------------------------------------------------
   void initialize( const std::string& tag, unsigned int elemsCount );
   void advance();
};

///////////////////////////////////////////////////////////////////////////////
