#include "ml-Blender/DataSource.h"
#include "tinyxml/tinyxml.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class DSFloatArray : public IDSArrayType
   {
   private:
      unsigned int                  m_stride;
      std::vector< float* >         m_elems;

   public:
      DSFloatArray( TiXmlElement& elem, unsigned int count, unsigned int stride )
         : m_stride( stride )
      {
         std::string floatArrayData = elem.GetText();
         std::stringstream floatArrayStr( floatArrayData );

         m_elems.resize( count, NULL );
         for ( unsigned int i = 0; i < count; ++i )
         {
            m_elems[i] = new float[stride];
            for ( unsigned int j = 0; j < stride; ++j )
            {
               floatArrayStr >> m_elems[i][j];
            }
         }
      }

      ~DSFloatArray()
      {
         unsigned int count = m_elems.size();
         for ( unsigned int i = 0; i < count; ++i )
         {
            delete [] m_elems[i];
         }
      }
      void readData( int dataOffset, void* data )
      {
         float* floatData = static_cast< float* >( data );
         memcpy( floatData, m_elems[dataOffset], m_stride * sizeof( float ) );
      }
   };

   // -------------------------------------------------------------------------

   class DSNamesArray : public IDSArrayType
   {
   private:
      std::vector< std::string >      m_elems;

   public:
      DSNamesArray( TiXmlElement& elem, unsigned int count )
      {
         std::string strArrayData = elem.GetText();
         std::stringstream strArrayStr( strArrayData );

         m_elems.resize( count, "" );
         for ( unsigned int i = 0; i < count; ++i )
         {
            strArrayStr >> m_elems[i];
         }
      }

      void readData( int dataOffset, void* data )
      {
         std::string& str = *( static_cast< std::string* >( data ) );
         str = m_elems[dataOffset];
      }
   };
} // anonymous

///////////////////////////////////////////////////////////////////////////////

DSArray::DSArray( TiXmlElement& elem )
   : m_type( NULL )
{
   TiXmlElement* techniqueElem = elem.FirstChildElement( "technique_common" );
   ASSERT( techniqueElem != NULL );

   TiXmlElement* accessorElem = techniqueElem->FirstChildElement( "accessor" );
   ASSERT( techniqueElem != NULL );

   accessorElem->Attribute( "count", (int*)&m_count );

   int stride = 0;
   accessorElem->Attribute( "stride", &stride );

   // create a proper type of element
   TiXmlElement* typeElem = NULL;

   if ( typeElem = elem.FirstChildElement( "float_array" ) )
   {
      m_type = new DSFloatArray( *typeElem, m_count, stride );
   }
   else if ( typeElem = elem.FirstChildElement( "Name_array" ) )
   {
      m_type = new DSNamesArray( *typeElem, m_count );
   }
   
}

///////////////////////////////////////////////////////////////////////////////

DSArray::~DSArray()
{
   delete m_type;
   m_type = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DSArray::readData( int dataOffset, void* data )
{
   if ( m_type )
   {
      m_type->readData( dataOffset, data );
   }
}

///////////////////////////////////////////////////////////////////////////////
