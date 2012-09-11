/// @file   TamyEditor/FSTreeItemsFactory.h
/// @brief  items factory that creates new items that can populate the filesystem tree
#pragma once

#include "TreeWidgetDescFactory.h"
#include <QString>


///////////////////////////////////////////////////////////////////////////////

enum FSTreeItemType
{
   FS_ITEM_DIR,
   FS_ITEM_PROJECT,
};

///////////////////////////////////////////////////////////////////////////////

class FSTreeItemsFactory : public TreeWidgetDescFactory
{
private:
   QString                    m_iconsDir;

public:
   /**
    * Constructor.
    *
    * @param iconsDir
    */ 
   FSTreeItemsFactory( const QString& iconsDir );

   // -------------------------------------------------------------------------
   // TreeWidgetDescFactory implementation
   // -------------------------------------------------------------------------
   unsigned int typesCount() const;
   void getDesc( unsigned int idx, QString& outDesc, QString& outGroup, QIcon& outIcon ) const;
   void getDesc( const SerializableReflectionType& type, QString& outDesc, QString& outGroup, QIcon& outIcon ) const;
   const SerializableReflectionType* getClass( unsigned int idx ) const;

};

///////////////////////////////////////////////////////////////////////////////
