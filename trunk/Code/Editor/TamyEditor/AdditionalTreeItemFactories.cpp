#include "AdditionalTreeItemFactories.h"
#include <QIcon>


///////////////////////////////////////////////////////////////////////////////

DirectoriesFactoryWrapper::DirectoriesFactoryWrapper( TreeWidgetDescFactory* wrappedFactory, const QString& iconsDir )
   : m_wrappedFactory( wrappedFactory )
   , m_iconsDir( iconsDir )
{
}

///////////////////////////////////////////////////////////////////////////////

DirectoriesFactoryWrapper::~DirectoriesFactoryWrapper()
{
   delete m_wrappedFactory;
   m_wrappedFactory = NULL;
}

///////////////////////////////////////////////////////////////////////////////

unsigned int DirectoriesFactoryWrapper::typesCount() const
{
   return m_wrappedFactory->typesCount() + 1;
}

///////////////////////////////////////////////////////////////////////////////

void DirectoriesFactoryWrapper::getDesc( unsigned int idx, QString& outDesc, QString& outGroup, QIcon& outIcon ) const
{
   if ( idx == 0 )
   {
      // a directory
      outDesc = "Directory";
      outGroup = "";
      outIcon = QIcon( m_iconsDir + "dirIcon.png" );
   }
   else
   {
      // a resource
      m_wrappedFactory->getDesc( idx - 1, outDesc, outGroup, outIcon );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DirectoriesFactoryWrapper::getDesc( const SerializableReflectionType& type, QString& outDesc, QString& outGroup, QIcon& outIcon ) const
{
   m_wrappedFactory->getDesc( type, outDesc, outGroup, outIcon );
}

///////////////////////////////////////////////////////////////////////////////

const SerializableReflectionType* DirectoriesFactoryWrapper::getClass( unsigned int idx ) const
{
   if ( idx == 0 )
   {
      // a directory
      return NULL;
   }
   else
   {
      // a resource
      return m_wrappedFactory->getClass( idx - 1 );
   }
}

///////////////////////////////////////////////////////////////////////////////

