#include "FSLeafNode.h"
#include "ResourcesBrowser.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

FSLeafNode::FSLeafNode( FSTreeNode* parent, 
                        const std::string& nodeName, 
                        const Filesystem& fs, 
                        TypeDescFactory< Resource >& itemsFactory )
: FSTreeNode( parent, nodeName, fs )
{
   // set icon
   setEntryIcon( fs, itemsFactory );

   // set description
   setText( 0, m_fsNodeName.c_str() );
   setEntrySize( fs );
}

///////////////////////////////////////////////////////////////////////////////

void FSLeafNode::setEntryIcon( const Filesystem& fs, TypeDescFactory< Resource >& itemsFactory )
{
   QString iconsDir = fs.getShortcut( "editorIcons" ).c_str();
   std::string extension = fs.extractExtension( m_fsNodeName ).c_str();

   Class resourceType = Resource::findResourceClass( extension );
   if ( resourceType.isValid() )
   {
      QString typeName;
      QIcon icon;
      itemsFactory.getDesc( resourceType, typeName, icon );
      setIcon( 0, icon );
   }
   else
   {
      setIcon( 0, QIcon( iconsDir + "unknownFileIcon.png" ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void FSLeafNode::setEntrySize( const Filesystem& fs )
{
   std::string relativePath = getRelativePath();

   std::size_t fileSize = 0;
   try
   {
      File* file = fs.open( relativePath );
      fileSize = file->size();
      delete file;
   }
   catch ( std::exception& ex )
   {
      fileSize = 0;
   }

   char fileSizeStr[ 32 ];
   sprintf_s( fileSizeStr, "%d", fileSize );
   setText( 1, fileSizeStr );
}

///////////////////////////////////////////////////////////////////////////////

std::string FSLeafNode::getRelativePath() const
{
   ASSERT_MSG ( parent(), "Leaf node has to have a parent" );

   std::string path = dynamic_cast< FSTreeNode* >( parent() )->getRelativePath();
   path += m_fsNodeName;

   return path;
}

///////////////////////////////////////////////////////////////////////////////

void FSLeafNode::editResource( ResourcesBrowser& editorsFactory )
{
   std::string pathToResource = getRelativePath();
   editorsFactory.editResource( pathToResource );
}

///////////////////////////////////////////////////////////////////////////////

TreeWidgetDescFactory* FSLeafNode::getDescFactory( ResourcesBrowser& resourcesFactory )
{
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void FSLeafNode::addNode( unsigned int typeIdx, ResourcesBrowser& resourcesFactory )
{
   throw std::logic_error( "This operation should never be performed" );
}

///////////////////////////////////////////////////////////////////////////////
