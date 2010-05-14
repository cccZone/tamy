#include "MousePropertiesView.h"
#include "core\Property.h"
#include <d3dx9.h>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class Vec3MousePropEdit : public TPropertyEditor<MousePropertiesView>,
                             public MousePropertiesView::Observer
   {
   private:
      TEditableProperty< D3DXVECTOR3 >*   m_property;

   public:
      Vec3MousePropEdit( TEditableProperty< D3DXVECTOR3 >* property )
      : m_property(property)
      {
      }

      ~Vec3MousePropEdit()
      {
         delete m_property;
         m_property = NULL;
      }

      void initialize(MousePropertiesView& view)
      {
         view.attach(*this);
      }

      void deinitialize(MousePropertiesView& view)
      {
         view.detach(*this);
      }

      void update(const Point& mousePos)
      {
         // TODO: proper movement mode
      }
   };

   // -------------------------------------------------------------------------

   class MtxMousePropEdit : public TPropertyEditor<MousePropertiesView>,
                            public MousePropertiesView::Observer
   {
   private:
      TEditableProperty< D3DXMATRIX >*    m_property;

   public:
      MtxMousePropEdit( TEditableProperty< D3DXMATRIX >* property )
         : m_property( property )
      {
      }

      ~MtxMousePropEdit()
      {
         delete m_property;
         m_property = NULL;
      }

      void initialize(MousePropertiesView& view)
      {
         view.attach(*this);
      }

      void deinitialize(MousePropertiesView& view)
      {
         view.detach(*this);
      }

      void update(const Point& mousePos)
      {
         // TODO: proper movement mode
      }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

MousePropertiesView::MousePropertiesView()
{
   associate< D3DXVECTOR3, Vec3MousePropEdit>();
   associate< D3DXMATRIX, MtxMousePropEdit>();
}

///////////////////////////////////////////////////////////////////////////////

void MousePropertiesView::update(const Point& mousePos)
{
   unsigned int count = m_observers.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_observers[i]->update(mousePos);
   }
}

///////////////////////////////////////////////////////////////////////////////

void MousePropertiesView::attach(Observer& observer)
{
   m_observers.push_back(&observer);
}

///////////////////////////////////////////////////////////////////////////////

void MousePropertiesView::detach(Observer& observer)
{
   Observers::iterator it = std::find(m_observers.begin(), 
      m_observers.end(), &observer);

   if (it != m_observers.end())
   {
      m_observers.erase(it);
   }
}

///////////////////////////////////////////////////////////////////////////////
