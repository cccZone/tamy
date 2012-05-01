#include "core-TestFramework\TestFramework.h"
#include "core-MVC\Model.h"
#include "core-MVC\Entity.h"
#include "core\Component.h"
#include "core-MVC\ModelView.h"
#include <d3dx9.h>



///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   class EntityAMock : public Entity
   {
      DECLARE_CLASS()

   private:
      int m_index;

   public:
      EntityAMock() : m_index( -1 ) {}
      EntityAMock( int idx ) : m_index( idx ) {}

      int getIndex() const { return m_index; }

   protected:
      Entity* cloneSelf() const
      {
         return new EntityAMock( m_index );
      }
   };
   BEGIN_OBJECT(EntityAMock);
      PARENT(Entity);
      PROPERTY_EDIT("index", int, m_index);
   END_OBJECT();

   // -------------------------------------------------------------------------

   class EntityBMock : public Entity
   {
      DECLARE_CLASS()

   private:
      int m_index;

   public:
      EntityBMock() : m_index( -1 ) {}
      EntityBMock( int idx ) : m_index( idx ) {}

      int getIndex() const { return m_index; }

   protected:
      Entity* cloneSelf() const
      {
         return new EntityBMock( m_index );
      }
   };
   BEGIN_OBJECT(EntityBMock);
      PARENT(Entity);
      PROPERTY_EDIT("index", int, m_index);
   END_OBJECT();

   // -------------------------------------------------------------------------

   class UpdatedEntityMock : public Entity
   {
   private:
      float m_value;

   public:
      UpdatedEntityMock() : m_value( 0.f ) {}
      UpdatedEntityMock( int value ) : m_value( value ) {}

      void onUpdate( float timeElapsed ) { m_value = timeElapsed; }

      float getValue() const { return m_value; }

   protected:
      Entity* cloneSelf() const
      {
         return new UpdatedEntityMock( m_value );
      }
   };

   // -------------------------------------------------------------------------

   class MockComponentA : public Component<Model>
   {
   public:
      void initialize(Model& mgr) {}
   };

   // -------------------------------------------------------------------------

   class MockComponentB : public Component<Model>
   {
   public:
      void initialize(Model& mgr) {}
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

      void onEntityChanged( Entity& entity )
      {
      }

      void resetContents()
      {
         m_entitiesCount = 0;
      }
   };

   // -------------------------------------------------------------------------

   class ViewsCreatingViewMock : public ModelView
   {
   private:
      typedef std::map<Entity*, ModelView*> Views;
      Views m_views;

      Model& m_model;

   public:
      ViewsCreatingViewMock(Model& model) : m_model(model) {}

      ~ViewsCreatingViewMock()
      {
         resetContents();
      }

      void onEntityAdded(Entity& entity) 
      {
         Views::iterator it = m_views.find(&entity);
         if (it == m_views.end())
         {
            ModelView* newView = new ViewMock();
            m_model.attach(*newView);

            m_views.insert(std::make_pair(&entity, newView));
         }
      }

      void onEntityRemoved(Entity& entity) 
      {
         Views::iterator it = m_views.find(&entity);
         if (it != m_views.end())
         {
            delete it->second;
            m_views.erase(it);
         }
      }

      void onEntityChanged( Entity& entity )
      {
      }

   protected:
      void resetContents()
      {
         for (Views::iterator it = m_views.begin();
            it != m_views.end(); ++it)
         {
            delete it->second;
         }
         m_views.clear();
      }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

DEFINE_TYPE_ID( EntityAMock );
DEFINE_TYPE_ID( EntityBMock );

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
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, model->getViewsCount());

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
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, model->getViewsCount());

   delete model;
   CPPUNIT_ASSERT_EQUAL(false, view->isAttachedToModel());

   delete view;
   
   // if it hasn't crashed till this point, means that every thing's a-OK :).
}

///////////////////////////////////////////////////////////////////////////////

TEST(Model, attachingToMultipleModels)
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
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, model1.getViewsCount());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, model2.getViewsCount());
   CPPUNIT_ASSERT_EQUAL(true, view.isAttachedToModel());
   CPPUNIT_ASSERT_EQUAL(3, view.getEntitiesObserved());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Model, nestedViews)
{
   Model* model = new Model();
   
   ViewsCreatingViewMock view(*model);
   model->attach(view);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, model->getViewsCount());

   EntityAMock entity1(10);
   model->add(&entity1, false);
   model->add(new EntityAMock(10));
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, model->getViewsCount());

   model->remove(entity1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, model->getViewsCount());
   
   delete model;
}

///////////////////////////////////////////////////////////////////////////////

TEST( Model, stateUpdate )
{
   Model model;

   UpdatedEntityMock* parentEntity = new UpdatedEntityMock();
   UpdatedEntityMock* child1Entity = new UpdatedEntityMock();
   UpdatedEntityMock* child2Entity = new UpdatedEntityMock();
   model.add( parentEntity );
   parentEntity->add( child1Entity );
   parentEntity->add( child2Entity );

   // the entities are gonna memorize the last tileElapsed value - so we can check that

   model.update( 1.f );
   CPPUNIT_ASSERT_EQUAL( 1.f, parentEntity->getValue() );
   CPPUNIT_ASSERT_EQUAL( 1.f, child1Entity->getValue() );
   CPPUNIT_ASSERT_EQUAL( 1.f, child2Entity->getValue() );
  
   model.update( 5.f );
   CPPUNIT_ASSERT_EQUAL( 5.f, parentEntity->getValue() );
   CPPUNIT_ASSERT_EQUAL( 5.f, child1Entity->getValue() );
   CPPUNIT_ASSERT_EQUAL( 5.f, child2Entity->getValue() );
}

///////////////////////////////////////////////////////////////////////////////

