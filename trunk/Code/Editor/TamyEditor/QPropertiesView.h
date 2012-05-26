/// @file   TamyEditor\QPropertiecView.h
/// @brief  properties browser widget
#pragma once


#include "core\Component.h"
#include "core\ReflectionPropertiesView.h"
#include <QFrame>


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class QVBoxLayout;

///////////////////////////////////////////////////////////////////////////////

/**
 * Properties browser widget.
 */
class QPropertiesView : public QFrame, public ReflectionPropertiesView
{
   Q_OBJECT

private:
   QVBoxLayout*    m_layout;

public:
   QPropertiesView();
   ~QPropertiesView();

protected:
   void onSet( ReflectionObjectEditor* rootEditor );

private:
   void initFactory();
};

///////////////////////////////////////////////////////////////////////////////
