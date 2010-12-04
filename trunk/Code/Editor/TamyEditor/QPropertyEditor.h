#pragma once

/// @file   TamyEditor\QPropertyEditor.h
/// @brief  base class for all qt property editors

#include "core\PropertyEditor.h"
#include <QFrame>
#include <Qstring>


///////////////////////////////////////////////////////////////////////////////

class QPropertiesView;
class QVBoxLayout;
class QSpacerItem;
class TamyEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * This base class will provide all editors with a uniform look & feel.
 */
class QPropertyEditor : public QFrame,
                        public TPropertyEditor<QPropertiesView>
{
   Q_OBJECT

private:
   QVBoxLayout*      m_layout;
   QSpacerItem*      m_spacer;

public:
   virtual ~QPropertyEditor();

   // -------------------------------------------------------------------------
   // TPropertyEditor implementation
   // -------------------------------------------------------------------------
   void initialize( QPropertiesView& view );
   void deinitialize( QPropertiesView& view );

protected:
   /**
    * Constructor.
    *
    * @param property   property this editor edits
    */
   QPropertyEditor(const QString& propertyLabel);

   /**
    * Called when the editor gets initialized.
    */
   virtual void onInitialize() {}

   /**
    * Adds a new widget.
    *
    * @param widget
    */
   void addWidget(QWidget* widget);

   /**
    * Adds an existing widget.
    *
    * @param widget
    */
   void removeWidget(QWidget& widget);
};

///////////////////////////////////////////////////////////////////////////////
