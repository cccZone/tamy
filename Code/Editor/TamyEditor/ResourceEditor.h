/// @file   TamyEditor\ResourceEditor.h
/// @brief  common interface for all abstract editors
#pragma once

#include <QFrame>
#include <QIcon>

///////////////////////////////////////////////////////////////////////////////

class TamyEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Common interface for all abstract editors.
 */
class ResourceEditor : public QFrame
{
private:
   QString     m_label;
   QIcon       m_icon;

public:
   /**
    * Constructor.
    */
   ResourceEditor() : QFrame( NULL ) {}
   virtual ~ResourceEditor() {}

   /**
    * This method will be called once the editor is created to initialize
    * and start it up.
    */
   void initialize( const QString& label, const QIcon& icon )
   {
      m_label = label;
      m_icon = icon;
      onInitialize();
   }

   /**
    * Called when the editor is about to be destroyed, or when we generally
    * want it to save its state, 'cause it may be killed.
    *
    * @param saveProgress     is it safe for the editor to save its contents?
    */
   void deinitialize( bool saveProgress ) 
   {
      onDeinitialize( saveProgress );
   }

   /**
    * Returns an icon assigned to the editor.
    */
   inline QIcon getIcon() const { return m_icon; }

   /**
    * Returns the editor's label.
    */
   inline QString getLabel() const { return m_label; }

protected:
   /**
    * Editor specific initialization.
    */
   virtual void onInitialize() = 0;

   /**
    * Editor specific deinitialization.
    */
   virtual void onDeinitialize( bool saveProgress ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
