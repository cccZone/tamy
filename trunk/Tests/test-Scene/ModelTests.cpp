#include "core-TestFramework\TestFramework.h"
#include "core-Scene\Model.h"
#include "core-Scene\Entity.h"
#include "core-Scene\ModelSerializer.h"
#include "core-Scene\ModelView.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   class EntityAMock : public Entity
   {
   private:
      int m_index;

   public:
      EntityAMock() : INIT_ENTITY(EntityAMock), m_index(-1) {}
      EntityAMock(int idx) : INIT_ENTITY(EntityAMock), m_index(idx) {}

      void save(ModelSerializer& serializer)
      {
         serializer.saveInt(m_index);
      }

      void load(ModelSerializer& serializer)
      {
         m_index = serializer.loadInt();
      }

      int getIndex() const {return m_index;}
   };

   // -------------------------------------------------------------------------

   class EntityBMock : public Entity
   {
   private:
      int m_index;

   public:
      EntityBMock() : INIT_ENTITY(EntityBMock), m_index(-1) {}
      EntityBMock(int idx) : INIT_ENTITY(EntityBMock), m_index(idx) {}

      void save(ModelSerializer& serializer)
      {
         serializer.saveInt(m_index);
      }

      void load(ModelSerializer& serializer)
      {
         m_index = serializer.loadInt();
      }

      int getIndex() const {return m_index;}
   };

   // -------------------------------------------------------------------------

   class ModelSerializerMock : public ModelSerializer
   {
   private:
      std::vector<int>& m_storage;

   public:
      ModelSerializerMock(std::vector<int>& storage)
         : m_storage(storage)
      {}

      void saveInt(int val)
      {
         m_storage.push_back(val);
      }

      int loadInt()
      {
         int val = m_storage.front();
         m_storage.erase(m_storage.begin());

         return val;
      }
   };

   // -------------------------------------------------------------------------

   class ViewMock : public ModelView
   {
   private:
      int m_entitiesCount;

   public:
      ViewMock() : m_entitiesCount(0) {}

      int getEntitiesObserved() const 
      {
         return m_entitiesCount;
      }

      void onEntityAdded(Entity& entity) 
      {
         m_entitiesCount++;
      }
      void onEntityRemoved(Entity& entity) 
      {
         m_entitiesCount--;
      }

      void resetContents()
      {
         m_entitiesCount = 0;
      }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(Model, serialization)
{
   Model model;

   model.add(new EntityAMock(5));
   model.add(new EntityBMock(10));

   std::vector<int> storage;
   ModelSerializerMock serializer(storage);
   model.save(serializer);

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, model.getEntitiesCount());
   CPPUNIT_ASSERT(NULL != dynamic_cast<EntityAMock*> (&model.getEntity(0)));
   CPPUNIT_ASSERT(NULL != dynamic_cast<EntityBMock*> (&model.getEntity(1)));

   model.clear();

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, model.getEntitiesCount());

   model.load(serializer);

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, model.getEntitiesCount());
   CPPUNIT_ASSERT_EQUAL(5,  dynamic_cast<EntityAMock*> (&model.getEntity(0))->getIndex());
   CPPUNIT_ASSERT_EQUAL(10, dynamic_cast<EntityBMock*> (&model.getEntity(1))->getIndex());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Model, viewingEntities)
{
   Model model;

   ViewMock view1;
   model.attach(view1);

   Entity* entity = new EntityAMock(5);

   CPPUNIT_ASSERT_EQUAL(0, view1.getEntitiesObserved());
   model.add(entity);
   CPPUNIT_ASSERT_EQUAL(1, view1.getEntitiesObserved());
   model.add(new EntityBMock(10));
   CPPUNIT_ASSERT_EQUAL(2, view1.getEntitiesObserved());

   ViewMock view2;
   model.attach(view2);
   CPPUNIT_ASSERT_EQUAL(2, view2.getEntitiesObserved());

   model.remove(*entity);
   CPPUNIT_ASSERT_EQUAL(1, view1.getEntitiesObserved());
   CPPUNIT_ASSERT_EQUAL(1, view2.getEntitiesObserved());

   model.add(new EntityBMock(5));
   CPPUNIT_ASSERT_EQUAL(2, view1.getEntitiesObserved());
   CPPUNIT_ASSERT_EQUAL(2, view2.getEntitiesObserved());

   model.clear();
   CPPUNIT_ASSERT_EQUAL(0, view1.getEntitiesObserved());
   CPPUNIT_ASSERT_EQUAL(0, view2.getEntitiesObserved());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Model, attachingAndDetachingViews)
{
   Model* model = new Model();
   ViewMock* view = new ViewMock();

   model->attach(*view);

   // let's remove the view without explicitly detaching it
   // from the model and see if model-view link is properly
   // cleaned up
   delete view; 
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, model->getViewsCount());
   model->add(new EntityAMock(0));

   // if it hasn't crashed till now, means that that link
   // was cleaned up ok.

   // Now let's attach a new view, and let's delete a model to which
   // the view is attached. Later on - let's delete the view
   // and see if it was properly informed that it's model was 
   // deleted ok.
   view = new ViewMock();
   model->attach(*view);
   delete model;
   CPPUNIT_ASSERT_EQUAL(false, view->isAttachedToModel());
   delete view;
   
   // if it hasn't crashed till this point, means that every thing's a-OK :).
}

///////////////////////////////////////////////////////////////////////////////

TEST(Model, reattachingViewBetweenModels)
{
   Model model1;
   Model model2;
   ViewMock view;

   model1.add(new EntityAMock(10));

   model2.add(new EntityBMock(10));
   model2.add(new EntityBMock(10));

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, model1.getViewsCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, model2.getViewsCount());
   CPPUNIT_ASSERT_EQUAL(false, view.isAttachedToModel());
   CPPUNIT_ASSERT_EQUAL(0, view.getEntitiesObserved());

   model1.attach(view);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, model1.getViewsCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, model2.getViewsCount());
   CPPUNIT_ASSERT_EQUAL(true, view.isAttachedToModel());
   CPPUNIT_ASSERT_EQUAL(1, view.getEntitiesObserved());

   model2.attach(view);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, model1.getViewsCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, model2.getViewsCount());
   CPPUNIT_ASSERT_EQUAL(true, view.isAttachedToModel());
   CPPUNIT_ASSERT_EQUAL(2, view.getEntitiesObserved());
}

///////////////////////////////////////////////////////////////////////////////

/*
Scena moze:
1.) obiekty sa kontrolowane przez przypisane kontrolery
2.) jest wyswietlana przez widoki, a kazdy obiekt ma odpowiednia reprezentacje
*/
