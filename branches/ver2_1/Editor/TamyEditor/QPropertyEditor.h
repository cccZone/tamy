#pragma once

/// @file   TamyEditor\QPropertyEditor.h
/// @brief  base class for all qt property editors

#include "core-Scene\PropertyEditor.h"
#include <QFrame>
#include <Qstring>


///////////////////////////////////////////////////////////////////////////////

class PropertiesEditor;
class QVBoxLayout;

///////////////////////////////////////////////////////////////////////////////

/**
 * This base class will provide all editors with a uniform look & feel.
 */
class QPropertyEditor : public QFrame,
                        public TPropertyEditor<PropertiesEditor>
{
   Q_OBJECT

private:
   QVBoxLayout* m_layout;

public:
   virtual ~QPropertyEditor();

   // -------------------------------------------------------------------------
   // TPropertyEditor implementation
   // -------------------------------------------------------------------------
   void initialize(PropertiesEditor& view);
   void deinitialize(PropertiesEditor& view);

protected:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   QPropertyEditor(const QString& propertyLabel);

   /**
    * Adds a new widget.
    *
    * @param widget
    */
   void addWidget(QWidget* widget);

};

///////////////////////////////////////////////////////////////////////////////
