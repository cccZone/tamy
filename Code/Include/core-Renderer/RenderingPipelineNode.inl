#ifndef _RENDERING_PIPELINE_NODE_H
#error "This file can only be included from RenderingPipelineNode.h"
#else

#include <stdexcept>
#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

template< typename T >
const T& RenderingPipelineNode::getInput( const std::string& inputName ) const
{
   RPNodeInput* input = findInput( inputName );
   if ( input == NULL)
   {
      char tmp[128];
      sprintf_s( tmp, "Unable to find input '%s'", inputName.c_str() );
      throw std::runtime_error( tmp );
   }

   T* dataSource = dynamic_cast< T* >( input );
   if ( dataSource == NULL )
   {
      char tmp[128];
      sprintf_s( tmp, "Input's '%s' type doesn't match the specified one", inputName.c_str() );
      throw std::runtime_error( tmp );
   }

   return *dataSource;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const T& RenderingPipelineNode::getOutput( const std::string& outputName ) const
{
   RPNodeOutput* output = findOutput( outputName );
   if ( output == NULL )
   {
      char tmp[128];
      sprintf_s( tmp, "Unable to find output '%s'", outputName.c_str() );
      throw std::runtime_error( tmp );
   }

   T* castOutput = dynamic_cast< T* >( output );
   if ( castOutput == NULL )
   {
      char tmp[128];
      sprintf_s( tmp, "Output's '%s' type doesn't match the specified one", outputName.c_str() );
      throw std::runtime_error( tmp );
   }

   return *castOutput;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RENDERING_PIPELINE_NODE_H
