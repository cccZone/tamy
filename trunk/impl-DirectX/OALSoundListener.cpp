#include "impl-DirectX\OALSoundListener.h"
#include <al.h>
#include <alc.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

void OALSoundListener::update()
{
   D3DXMATRIX mtx = getGlobalMtx();

   ALfloat listenerOri[] = {mtx._31, mtx._32, mtx._33, 
                            mtx._21, mtx._22, mtx._23};

   alListener3f(AL_POSITION, mtx._41, mtx._42, mtx._42);
   alListenerfv(AL_ORIENTATION, listenerOri);
}

///////////////////////////////////////////////////////////////////////////////
