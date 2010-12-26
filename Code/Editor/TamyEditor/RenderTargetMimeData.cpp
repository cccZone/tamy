#include "RenderTargetMimeData.h"
#include <QMimeData>


///////////////////////////////////////////////////////////////////////////////

RenderTargetMimeData::RenderTargetMimeData( std::string& id )
   : m_id( id )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetMimeData::save( QMimeData& data )
{
   QByteArray encodedData;
   QDataStream stream( &encodedData, QIODevice::WriteOnly );

   stream << QString( m_id.c_str() );

   data.setData( "TamyEditor/RenderTargetMimeData", encodedData );
}

///////////////////////////////////////////////////////////////////////////////

void RenderTargetMimeData::load( const QMimeData& data )
{
   QByteArray encodedData = data.data( "TamyEditor/RenderTargetMimeData" );
   QDataStream stream( &encodedData, QIODevice::ReadOnly );

   while ( !stream.atEnd() ) 
   {
      QString text;
      stream >> text;
      m_id = text.toStdString();

      // we're only interested in the first item in the stream
      break;
   }
}

///////////////////////////////////////////////////////////////////////////////
