/// @file   ml-Blender/BlenderSceneExporter.h
/// @brief  this class exports blender scenes to Tamy
#pragma once


///////////////////////////////////////////////////////////////////////////////

class BlenderSceneExporter
{
private:
   static BlenderSceneExporter         s_theIntance;

public:
   /**
    * Returns the singleton instance of the class.
    */
   static BlenderSceneExporter& getInstance() { return s_theIntance; }

   void printMsg( const char* msg );

private:
   /**
    * Constructor.
    */
   BlenderSceneExporter();
};

///////////////////////////////////////////////////////////////////////////////
