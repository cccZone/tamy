/// @file   TamyEditor/RenderTargetDescriptorDialog.h
/// @brief  an editor for the render target descriptor
#pragma once

#include <QDialog>
#include "ui_rendertargetdescriptordialog.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTargetDescriptor;

///////////////////////////////////////////////////////////////////////////////

/**
 * An editor for the render target descriptor.
 */
class RenderTargetDescriptorDialog : public QDialog
{
   Q_OBJECT

private:
   Ui::RenderTargetDescriptorDialog             m_ui;

   RenderTargetDescriptor&                      m_descriptor;

public:
   RenderTargetDescriptorDialog( QWidget* parent, RenderTargetDescriptor& descriptor, bool canChangeName = true );

public slots:
   void changeTargetID( const QString& value );
   void changeReadability( int isReadable );
   void usageChanged( const QString& usage );
   void changeTargetType( int isDynamic );
   void dynamicSizeChanged( double val );
   void staticSizeChanged( int val );

private:
   void updateTargetType();
};

///////////////////////////////////////////////////////////////////////////////
