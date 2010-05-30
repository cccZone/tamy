#include "core-TestFramework\TestFramework.h"
#include "core-MVC\Model.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\DefaultAttributeSorter.h"
#include "core-Renderer\Effect.h"
#include "core-Renderer\EffectAttribute.h"
#include "core-Renderer\LeafAttributeSorter.h"
#include "core-Renderer\RenderingView.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   typedef std::vector<std::string> Log;

   // -------------------------------------------------------------------------

   class GeometryMock : public GeometryResource
   {
   private:
      std::string m_id;
      Log* m_log;

   public:
      GeometryMock() : m_log(NULL) {}
      GeometryMock(const std::string& id, Log& log)
         : m_id(id)
         , m_log(&log)
      {}

      void render()
      {
         m_log->push_back(m_id);
      }

      BoundingVolume* calculateBoundingVolume() const {return NULL;}

      void onResourceLoaded(ResourcesManager& mgr) {}
   };

   // -------------------------------------------------------------------------

   class AttribMock : public EffectAttribute
   {
   private:
      const std::string& m_name;

   public:
      AttribMock(const std::string& name)
         : m_name(name)
      {}

      bool operator<(const AttribMock& rhs) const
      {
         return m_name < rhs.m_name;
      }
   };

   // -------------------------------------------------------------------------

   class RendererView_EffectMock : public Effect
   {
      DECLARE_CLASS(RendererView_EffectMock);

   private:
      std::string m_name;

   public:
      RendererView_EffectMock(const std::string& name = "EffectMock") 
         : m_name(name)
      {
         addAttribute(new AttribMock(m_name));
      }

   protected:
      unsigned int beginRendering() {return 1;}
      void beginPass(unsigned int passIdx) {}
      void endPass(unsigned int passIdx) {}
      void endRendering() {}
   };
   BEGIN_OBJECT(RendererView_EffectMock, Effect)
   END_OBJECT()

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(RendererView, entitiesAreSorted)
{
   Log log;
   Model scene3D;
   RenderingView view;
   scene3D.attach(view);

   LeafAttributeSorter<AttribMock> sorter;
   view.setAttributeSorter( sorter );

   GeometryMock geometry1("geometry1", log);
   GeometryMock geometry2("geometry2", log);
   GeometryMock geometry3("geometry3", log);
   Renderable* entity1 = new Renderable();
   Renderable* entity2 = new Renderable();
   Renderable* entity3 = new Renderable();
   entity1->add(new Geometry(geometry1));
   entity1->add(new RendererView_EffectMock("A"));
   entity2->add(new Geometry(geometry2));
   entity2->add(new RendererView_EffectMock("C"));
   entity3->add(new Geometry(geometry3));
   entity3->add(new RendererView_EffectMock("B"));

   scene3D.add(entity1);
   scene3D.add(entity2);
   scene3D.add(entity3);
   // case 1 - rendering with all entities added
   sorter.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, log.size());
   
   Log::iterator logIt = log.begin();
   CPPUNIT_ASSERT_EQUAL(std::string("geometry1"), *(logIt++));
   CPPUNIT_ASSERT_EQUAL(std::string("geometry3"), *(logIt++));
   CPPUNIT_ASSERT_EQUAL(std::string("geometry2"), *(logIt++));
   log.clear();

   // case 2 - rendering after an entity's been removed
   scene3D.remove(*entity3);

   sorter.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, log.size());

   logIt = log.begin();
   CPPUNIT_ASSERT_EQUAL(std::string("geometry1"), *(logIt++));
   CPPUNIT_ASSERT_EQUAL(std::string("geometry2"), *(logIt++));
}

///////////////////////////////////////////////////////////////////////////////

TEST(RendererView, changingSorterAfterAddingTheEntities)
{
   Log log;
   Model scene3D;
   RenderingView view;
   scene3D.attach(view);

   GeometryMock geometry1("geometry1", log);
   GeometryMock geometry2("geometry2", log);
   GeometryMock geometry3("geometry3", log);
   Renderable* entity1 = new Renderable();
   Renderable* entity2 = new Renderable();
   Renderable* entity3 = new Renderable();
   entity1->add(new Geometry(geometry1));
   entity1->add(new RendererView_EffectMock("A"));
   entity2->add(new Geometry(geometry2));
   entity2->add(new RendererView_EffectMock("C"));
   entity3->add(new Geometry(geometry3));
   entity3->add(new RendererView_EffectMock("B"));

   scene3D.add(entity1);
   scene3D.add(entity2);
   scene3D.add(entity3);

   LeafAttributeSorter<AttribMock> sorter;
   view.setAttributeSorter( sorter );

   sorter.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, log.size());

   Log::iterator logIt = log.begin();
   CPPUNIT_ASSERT_EQUAL(std::string("geometry1"), *(logIt++));
   CPPUNIT_ASSERT_EQUAL(std::string("geometry3"), *(logIt++));
   CPPUNIT_ASSERT_EQUAL(std::string("geometry2"), *(logIt++));
}

///////////////////////////////////////////////////////////////////////////////

TEST(RendererView, renderingVisibleRenderablesOnly)
{
   Log log;
   Model scene3D;
   RenderingView view;
   scene3D.attach(view);

   DefaultAttributeSorter sorter;
   view.setAttributeSorter( sorter );

   GeometryMock geometry1("geometry1", log);
   GeometryMock geometry2("geometry2", log);
   Renderable* entity1 = new Renderable();
   Renderable* entity2 = new Renderable();
   entity1->add(new Geometry(geometry1));
   entity1->add(new RendererView_EffectMock("A"));
   entity2->add(new Geometry(geometry2));
   entity2->add(new RendererView_EffectMock("A"));

   scene3D.add(entity1);
   scene3D.add(entity2);

   // hiding an entity
   entity1->setVisible(false);
   sorter.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, log.size());

   Log::iterator logIt = log.begin();
   CPPUNIT_ASSERT_EQUAL(std::string("geometry2"), *(logIt++));
}

///////////////////////////////////////////////////////////////////////////////
