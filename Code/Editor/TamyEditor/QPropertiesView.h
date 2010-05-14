#pragma once

/// @file   TamyEditor\QPropertiecView.h
/// @brief  properties browser view

#include "core\Component.h"
#include "core\PropertiesView.h"
#include "QPropertyEditor.h"

///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class QLayout;
class QWidget;
class QSpacerItem;

///////////////////////////////////////////////////////////////////////////////

class QPropertiesView : public QPropertyEditor, 
                        public TPropertiesView<QPropertiesView>
{
   Q_OBJECT

private:
   TamyEditor&    m_mgr;
   QLayout*       m_propertiesLayout;

public:
   QPropertiesView( TamyEditor& mgr );
   QPropertiesView( TamyEditor& mgr, VectorProperty& property );
   ~QPropertiesView();

   /**
    * Returns the working instance of the main services manager.
    */
   inline TamyEditor& getServicesMgr() { return m_mgr; }

   // -------------------------------------------------------------------------
   // Editor widgets management
   // -------------------------------------------------------------------------
   /**
    * This method adds a new single-property editor widget to the properties
    * editor window.
    *
    * @param editorWidget  widget of the editor we want to add
    */
   void addPropertyEditor(QWidget* editorWidget);

   /**
    * This method removes a single-property editor widget from the properties
    * editor window.
    *
    * @param editorWidget  widget of the editor we want to remove
    */
   void removePropertyEditor(QWidget& editorWidget);

protected:
   void onSet(Properties& properties);

private:
   void initUI();
   void initFactory();
};

///////////////////////////////////////////////////////////////////////////////
