/// @file   TamyEditor\ObjectPropertyEditor.h
/// @brief  object property editor
#pragma once

#include "core\ReflectionProperty.h"
#include "QPropertyEditor.h"
#include "core\ReflectionObject.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class QComboBox;
class QHBoxLayout;
class QPropertiesView;

///////////////////////////////////////////////////////////////////////////////

/**
 * Editor widget that will edit object properties ( except resources ) - will allow to instantiate
 * them in place etc.
 */
class ObjectPropertyEditor : public QPropertyEditor
{
   Q_OBJECT

private:
   TEditableReflectionProperty< ReflectionObject* >*     m_property;
   QComboBox*                                            m_typeSelectionBox;
   QVBoxLayout*                                          m_layout;
   QPropertiesView*                                      m_propertiesView;

public:
   /**
    * Constructor.
    *
    * @param property         property this editor edits
    */
   ObjectPropertyEditor( TEditableReflectionProperty< ReflectionObject* >* property );
   ~ObjectPropertyEditor();

public slots:
   void valChanged( const QString& text );

protected:
   void onInitialize();

private:
   void refresh();
};

///////////////////////////////////////////////////////////////////////////////
