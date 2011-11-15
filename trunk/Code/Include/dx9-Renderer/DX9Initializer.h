#pragma once

#include <windows.h>
#include <d3d9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;
enum VERTEXPROCESSING_TYPE;
struct RenderingDevice;
struct DX9Settings;

///////////////////////////////////////////////////////////////////////////////

/**
 * Allows to create a graphics display suitable to our needs.
 */
class DX9Initializer
{
private:
   IDirect3D9& m_d3d9;

public:
   /**
    * Constructor.
    */
   DX9Initializer( IDirect3D9& d3d9 );

   /**
    * Creates a DX9 testing device.
    */
   IDirect3DDevice9* createNullDevice();

   /**
    * Instantiates a DX9 renderer.
    */ 
   DX9Renderer* createDisplay( DX9Settings& settings, HWND hWnd, ULONG Flags = 0 );
};

///////////////////////////////////////////////////////////////////////////////
