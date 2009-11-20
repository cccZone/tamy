#include "core-TestFramework\TestFramework.h"
#include "core-Scene\Property.h"
#include "core-Scene\Properties.h"
#include "core-Scene\PropertyEditor.h"
#include "core-Scene\PropertiesView.h"
#include "core\MatrixWriter.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class PropertiesViewMock : public TPropertiesView<PropertiesViewMock>
   {
   };

   // -------------------------------------------------------------------------

   class VecEditorMock : public TPropertyEditor<PropertiesViewMock>
   {
   public:
      VecEditorMock(TProperty<D3DXVECTOR3>& property)
      {
      }

      void initialize(PropertiesViewMock& view) {}

      void deinitialize(PropertiesViewMock& view) {}
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(Properties, settingProperties)
{
   D3DXVECTOR3 position(0, 0, 0);
   D3DXVECTOR3 speed(0, 0, 0);

   Properties properties("");
   properties.add<D3DXVECTOR3>("Pos", "PosLabel", position);
   properties.add<D3DXVECTOR3>("Speed", "SpeedLabel", speed);

   CPPUNIT_ASSERT_EQUAL(true, properties.has("Pos"));
   CPPUNIT_ASSERT_EQUAL(true, properties.has("Speed"));
   CPPUNIT_ASSERT_EQUAL(false, properties.has("Orientation"));
}

///////////////////////////////////////////////////////////////////////////////

TEST(Properties, changingPropertyValue)
{
   D3DXVECTOR3 position(0, 0, 0);

   Properties properties("");
   properties.add<D3DXVECTOR3>("Pos", "PosLabel", position);

   D3DXVECTOR3 newPosition(10, 20, 30);
   properties.get<D3DXVECTOR3>("Pos").set(D3DXVECTOR3(10, 20, 30));
   CPPUNIT_ASSERT_EQUAL(newPosition, position);
}

///////////////////////////////////////////////////////////////////////////////

TEST(TPropertiesView, editors)
{
   D3DXVECTOR3 position(0, 0, 0);
   D3DXVECTOR3 speed(0, 0, 0);

   Properties properties("");
   properties.add<D3DXVECTOR3>("position", "positionLabel", position);
   properties.add<D3DXVECTOR3>("speed", "speedLabel", speed);

   PropertiesViewMock view;
   view.associate<TProperty<D3DXVECTOR3>, VecEditorMock>();

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, view.getEditorsCount());

   view.set(properties);

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, view.getEditorsCount());

   view.reset();

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, view.getEditorsCount());
}

///////////////////////////////////////////////////////////////////////////////
