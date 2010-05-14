#include "FSNodeMimeData.h"
#include <QMimeData>


///////////////////////////////////////////////////////////////////////////////

FSNodeMimeData::FSNodeMimeData( std::vector< std::string >& paths )
: m_paths( paths )
{
}

///////////////////////////////////////////////////////////////////////////////

void FSNodeMimeData::save( QMimeData& data )
{
   QByteArray encodedData;
   QDataStream stream( &encodedData, QIODevice::WriteOnly );

   unsigned int count = m_paths.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      stream << QString( m_paths[ i ].c_str() );
   }

   data.setData( "TamyEditor/FSNodeMimeData", encodedData );
}

///////////////////////////////////////////////////////////////////////////////

void FSNodeMimeData::load( const QMimeData& data )
{
   QByteArray encodedData = data.data( "TamyEditor/FSNodeMimeData" );
   QDataStream stream( &encodedData, QIODevice::ReadOnly );

   while ( !stream.atEnd() ) 
   {
      QString text;
      stream >> text;
      m_paths.push_back( text.toStdString() );
   }
}

///////////////////////////////////////////////////////////////////////////////
