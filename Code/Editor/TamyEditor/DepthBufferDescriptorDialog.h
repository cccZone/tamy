/// @file   TamyEditor/DepthBufferDescriptorDialog.h
/// @brief  an editor for depth buffer descriptors
#pragma once

#include <QDialog>
#include "ui_depthbufferdescriptordialog.h"


///////////////////////////////////////////////////////////////////////////////

class DepthBufferDescriptor;

///////////////////////////////////////////////////////////////////////////////

/**
 * An editor for depth buffer descriptors.
 */
class DepthBufferDescriptorDialog : public QDialog
{
   Q_OBJECT

private:
   Ui::DepthBufferDescriptorDialog              m_ui;

   DepthBufferDescriptor&                      m_descriptor;

public:
   DepthBufferDescriptorDialog( QWidget* parent, DepthBufferDescriptor& descriptor, bool canChangeName = true );

public slots:
   void changeBufferID( const QString& value );
   void sizeChanged( int val );
};

///////////////////////////////////////////////////////////////////////////////
