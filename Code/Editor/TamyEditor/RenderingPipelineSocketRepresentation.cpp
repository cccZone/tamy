#include "RenderingPipelineSocketRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSVoid );
   PARENT( GraphBlockSocket );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSVec4 );
   PARENT( GraphBlockSocket );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSTexture );
   PARENT( GraphBlockSocket );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSString );
   PARENT( GraphBlockSocket );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSMatrix );
   PARENT( GraphBlockSocket );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSInt );
   PARENT( GraphBlockSocket );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSFloat );
   PARENT( GraphBlockSocket );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSBool );
   PARENT( GraphBlockSocket );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
