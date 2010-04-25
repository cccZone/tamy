#ifndef _MODEL_H
#error "This file can only be included in Model.h"
#else

///////////////////////////////////////////////////////////////////////////////

template<typename COMPONENT_TYPE>
COMPONENT_TYPE* Model::getComponent()
{
   COMPONENT_TYPE* result = NULL;

   unsigned int componentsCount = getComponentsCount();
   for (unsigned int compIdx = 0; compIdx < componentsCount; ++compIdx)
   {
      result = dynamic_cast<COMPONENT_TYPE*> (ComponentsManager::getComponent(compIdx));
      if (result != NULL)
      {
         break;
      }
   }

   return result;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _MODEL_H
