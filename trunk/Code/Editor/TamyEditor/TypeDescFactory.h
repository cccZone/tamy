#ifndef _TYPE_DESC_FACTORY_H
#define _TYPE_DESC_FACTORY_H

/// @file   TamyEditor/TypeDescFactory.h
/// @brief  Factory creating descriptions of types.

#include "TreeWidgetDescFactory.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;

///////////////////////////////////////////////////////////////////////////////

/**
 * The factory provides a description of the specified type descendants
 * in the form of a name and an icon.
 *
 * It can be used with a TreeWidget as the items descriptions factory.
 */
template< typename T >
class TypeDescFactory : public TreeWidgetDescFactory
{
private:
   QString                 m_iconsDir;
   const Filesystem&       m_fs;
   QString                 m_defaultIcon;
   std::vector< Class >    m_classes;

public:
   /**
    * Constructor.
    *
    * @param iconsDir      directory with editor icons
    * @param fs            file system
    * @param defaultIcon   icon used when no other matching icon is found
    */
   TypeDescFactory( const QString& iconsDir, const Filesystem& fs, const QString& defaultIcon );

   /**
    * Returns the specified type.
    *
    * @param idx     type index.
    */
   const Class& getClass( unsigned int idx ) const;

   /**
    * Returns the description for the specified type.
    *
    * @param type       type we want the description of
    * @param outDesc    textual description
    * @param outIcon    visual description
    */
   void getDesc( const Class& type, QString& outDesc, QIcon& outIcon ) const;

   // -------------------------------------------------------------------------
   // TreeWidgetDescFactory implementation
   // -------------------------------------------------------------------------
   unsigned int typesCount() const;
   void getDesc( unsigned int idx, QString& outDesc, QIcon& outIcon ) const;

private:
   QString getIconName( const Class& type ) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "TypeDescFactory.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _TYPE_DESC_FACTORY_H
