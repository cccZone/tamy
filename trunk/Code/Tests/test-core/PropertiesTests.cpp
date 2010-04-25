#include "core-TestFramework\TestFramework.h"
#include "core\Property.h"
#include "core\Properties.h"
#include "core\PropertyEditor.h"
#include "core\PropertiesView.h"
#include "core\MatrixWriter.h"
#include "core\Serializable.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class PropertiesViewMock : public TPropertiesView<PropertiesViewMock>
   {
   private:
      class VecEditorMock : public TPropertyEditor<PropertiesViewMock>
      {
      public:
         VecEditorMock(TProperty<D3DXVECTOR3>& property)
         {
         }

         void initialize(PropertiesViewMock& view) {}

         void deinitialize(PropertiesViewMock& view) {}
      };

   public:
      PropertiesViewMock()
      {
         associate<TProperty<D3DXVECTOR3>, VecEditorMock>();
      }
   };

   // -------------------------------------------------------------------------

   template<typename T>
   class EditorMock : public PropertyEditor
   {
   public:
      EditorMock(TProperty<T>& val) {}
   };

   // -------------------------------------------------------------------------

   class SerializableMock : public Serializable
   {
   private:
      int m_handle;

   public:
      SerializableMock()
      {
         m_handle = getClassesRegistry().defineSolid<SerializableMock>("SerializableMock");
      }

      void onSave(Serializer& serializer) {}
      void onLoad(Serializer& serializer) {}
      
      int getClassID() const { return m_handle; }
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
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, view.getEditorsCount());

   view.set(properties);

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, view.getEditorsCount());

   view.reset();

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, view.getEditorsCount());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Properties, dynamicEditorCreation)
{
   GenericFactory<Property, PropertyEditor> factory;
   factory.associate<TProperty<int>, EditorMock<int> >()
      .associate<TProperty<float>, EditorMock<float> >()
      .associate<TProperty<SerializableMock*>, EditorMock<SerializableMock*> >();
   
   int intVal;
   float floatVal;
   SerializableMock* ptr = new SerializableMock();
   TProperty<int> intProperty(&intVal, "intVal", "intVal");
   TProperty<float> floatProperty(&floatVal, "floatVal", "floatVal");
   TProperty<SerializableMock*> floatPtrProperty(&ptr, "ptrProperty", "ptrProperty");

   PropertyEditor* editor = factory.create(intProperty);
   CPPUNIT_ASSERT(NULL != editor);
   CPPUNIT_ASSERT(NULL != dynamic_cast<EditorMock<int>*> (editor));
   delete editor;

   editor = factory.create(floatProperty);
   CPPUNIT_ASSERT(NULL != editor);
   CPPUNIT_ASSERT(NULL != dynamic_cast<EditorMock<float>*> (editor));
   delete editor;

   editor = factory.create(floatPtrProperty);
   CPPUNIT_ASSERT(NULL != editor);
   CPPUNIT_ASSERT(NULL != dynamic_cast<EditorMock<SerializableMock*>*> (editor));
   delete editor;
   delete ptr;
}

///////////////////////////////////////////////////////////////////////////////
