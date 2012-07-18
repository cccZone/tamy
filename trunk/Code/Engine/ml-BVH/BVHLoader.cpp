#include "ml-BVH/BVHLoader.h"
#include "core.h"
#include "core-MVC.h"
#include "core-AI.h"
#include "core-Renderer/Skeleton.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class BVHNode
   {
   public:
      virtual ~BVHNode() {}

      virtual void parse( std::stringstream& inStream ) = 0;

      virtual void parseAnimationFrames( std::stringstream& inStream, float frameTime ) = 0;

      virtual SpatialEntity* instantiate( SkeletonAnimation& animation ) = 0;

   protected:
      void validateLabel( std::stringstream& inStream, const char* label )
      {
         std::string tempText;
         inStream >> tempText;
         if ( tempText != label )
         {
            char tmpStr[128];
            sprintf_s( tmpStr, "'%s' scope begin sign expected", label );
            ASSERT_MSG( false, tmpStr );
         }
      }
   };

   // -----------------------------------------------------------------------------------

   class BVHEndSite : public BVHNode
   {
   private:
      Vector                m_offset;

   public:
      void parse( std::stringstream& inStream )
      {
         validateLabel( inStream, "{" );

         std::string variable;
         inStream >> variable;

         if ( variable == "OFFSET" )
         {
            inStream >> m_offset.x >> m_offset.y >> m_offset.z;
         }

         validateLabel( inStream, "}" );
      }

      void parseAnimationFrames( std::stringstream& inStream, float frameTime )
      {
         // nothing to do here - end sites don't have any keyframes defined
      }

      SpatialEntity* instantiate( SkeletonAnimation& animation )
      {
         return NULL;
      }
   };

   // -----------------------------------------------------------------------------------

   class BVHBone : public BVHNode
   {
   private:
      std::string                         m_boneName;
      Vector										m_offset;

      std::vector< std::string >          m_channels;

      BoneSRTAnimation*                   m_keyframes;
      std::vector< BVHNode* >             m_children;

   public:
      BVHBone()
         : m_keyframes( NULL )
      {
      }

      ~BVHBone()
      {
         delete m_keyframes;
         m_keyframes = NULL;

         unsigned int count = m_children.size();
         for ( unsigned int i = 0; i < count; ++i )
         {
            delete m_children[i];
         }
         m_children.clear();
      }

      void parse( std::stringstream& inStream )
      {
         // extract the bone name
         inStream >> m_boneName;

         // create a bone animation track
         delete m_keyframes;
         m_keyframes = new BoneSRTAnimation( m_boneName );

         // parse the structure
         validateLabel( inStream, "{" );
         
         std::string variable;
         while( true )
         {
            inStream >> variable;

            if ( variable == "}" )
            {
               // end of scope
               break;
            }
            else if ( variable == "OFFSET" )
            {
               inStream >> m_offset.x >> m_offset.y >> m_offset.z;
            }
            else if ( variable == "CHANNELS" )
            {
               unsigned int channelsCount;
               inStream >> channelsCount;

               std::string channelName;
               for ( unsigned int i = 0; i < channelsCount; ++i )
               {
                  inStream >> channelName;
                  m_channels.push_back( channelName );
               }
            }
            else if ( variable == "JOINT" )
            {
               BVHNode* child = new BVHBone();
               m_children.push_back( child );
               child->parse( inStream );
            }
            else if ( variable == "End" )
            {
               validateLabel( inStream, "Site" );

               BVHNode* child = new BVHEndSite();
               m_children.push_back( child );
               child->parse( inStream );
            }
         }
      }

      void parseAnimationFrames( std::stringstream& inStream, float frameTime )
      {
         // add a keyframe
         Vector trans( 0, 0, 0 );
         bool hasTranslation = false;

         float rotAngle;
         Quaternion orientation( 0, 0, 0, 1 );
         Quaternion tmpOrientation( 0, 0, 0, 1 );
         bool hasOrientation = false;

         for ( std::vector< std::string >::const_iterator it = m_channels.begin(); it != m_channels.end(); ++it )
         {
            const std::string& channelId = *it;
            if ( channelId == "Xposition" )
            {
               inStream >> trans.x;
               hasTranslation = true;
            }
            else if ( channelId == "Yposition" )
            {
               inStream >> trans.y;
               hasTranslation = true;
            }
            else if ( channelId == "Zposition" )
            {
               inStream >> trans.z;
               hasTranslation = true;
            }
            else if ( channelId == "Xrotation" )
            {
               inStream >> rotAngle;
               tmpOrientation.setAxisAngle( Vector::OX, DEG2RAD( rotAngle ) );
               tmpOrientation.mul( orientation );
               orientation = tmpOrientation;
               hasOrientation = true;
            }
            else if ( channelId == "Yrotation" )
            {
               inStream >> rotAngle;
               tmpOrientation.setAxisAngle( Vector::OY, DEG2RAD( rotAngle ) );
               tmpOrientation.mul( orientation );
               orientation = tmpOrientation;
               hasOrientation = true;
            }
            else if ( channelId == "Zrotation" )
            {
               inStream >> rotAngle;
               tmpOrientation.setAxisAngle( Vector::OZ, DEG2RAD( rotAngle ) );
               tmpOrientation.mul( orientation );
               orientation = tmpOrientation;
               hasOrientation = true;
            }
         }

         // add a keyframe
         if ( hasOrientation )
         {
            m_keyframes->addOrientationKey( frameTime, orientation );
         }
         if ( hasTranslation )
         {
            m_keyframes->addTranslationKey( frameTime, trans );
         }

         // pass the parsing to the children
         unsigned int count = m_children.size();
         for ( unsigned int i = 0; i < count; ++i )
         {
            m_children[i]->parseAnimationFrames( inStream, frameTime );
         }
      }

      SpatialEntity* instantiate( SkeletonAnimation& animation )
      {
         BoneEntity* entity = new BoneEntity( m_boneName );
         entity->accessLocalMtx().setTranslation( Vector( m_offset.x, m_offset.y, m_offset.z ) );

         unsigned int count = m_children.size();
         for ( unsigned int i = 0; i < count; ++i )
         {
            SpatialEntity* child = m_children[i]->instantiate( animation );
            if ( child != NULL )
            {
               entity->add( child );
            }
         }

         // fill the animation controller with animation keyframes
         animation.addKeys( m_keyframes );
         m_keyframes = NULL;

         return entity;
      }
   };

   // -----------------------------------------------------------------------------------

   class BVHHierarchy
   {
   private:
      BVHBone*                m_skeletonRoot;
      ResourcesManager&       m_rm;
      IProgressObserver&      m_observer;
     
   public:
      BVHHierarchy( ResourcesManager& rm, IProgressObserver& observer )
         : m_skeletonRoot( NULL )
         , m_rm( rm )
         , m_observer( observer )
      {
      }

      ~BVHHierarchy()
      {
         delete m_skeletonRoot;
         m_skeletonRoot = NULL;
      }

      void parse( const char* animationName, std::stringstream& inStream, SkeletonAnimation& animation, Model& scene )
      {
         // parse the skeleton
         m_observer.initialize( "Parsing the skeleton structure", 1 );
         std::string hierarchyLabel, rootLabel;
         inStream >> hierarchyLabel >> rootLabel;
         if ( hierarchyLabel != "HIERARCHY" || rootLabel != "ROOT" )
         {
            ASSERT_MSG( false, "Invalid BVH file - missing skeleton definition" );
            return;
         }

         BVHBone* skeletonRoot = new BVHBone();
         skeletonRoot->parse( inStream );
         m_observer.advance();

         // parse the motion
         std::string motionLabel;
         inStream >> motionLabel;
         if ( motionLabel != "MOTION" )
         {
            ASSERT_MSG( false, "Invalid BVH file - missing motion definition" );
            return;
         }

         std::string framesLabel, frameTimeLabel0, frameTimeLabel1;
         unsigned int framesCount = 0;
         float frameTimeDelta = 0.f;

         inStream >> framesLabel >> framesCount >> frameTimeLabel0 >> frameTimeLabel1 >> frameTimeDelta;
         if ( framesLabel != "Frames:" || frameTimeLabel0 != "Frame" || frameTimeLabel1 != "Time:" )
         {
            ASSERT_MSG( false, "Invalid BVH file - missing frames definition" );
            return;
         }

         // load the frames
         m_observer.initialize( "Parsing motion data", framesCount );
         float currTime = 0.f;
         for ( unsigned int frameIdx = 0; frameIdx < framesCount; ++frameIdx )
         {
            skeletonRoot->parseAnimationFrames( inStream, currTime );
            currTime += frameTimeDelta;
            m_observer.advance();
         }

         // create an animation resource
         m_observer.initialize( "Initializing scene", 1 );
         
         SpatialEntity* skeletonRootEntity = skeletonRoot->instantiate( animation );
         scene.add( skeletonRootEntity );

         m_observer.advance();
      }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

BVHModelLoader::BVHModelLoader( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer )
   : TResourceImporter< Model >( path, rm, observer )
{
}

///////////////////////////////////////////////////////////////////////////////

void BVHModelLoader::import( Model& scene )
{
   const Filesystem& fs = m_rm.getFilesystem();

   // load the file contents into a string
   File* bvhFile = fs.open( m_loadedFileName );
   StreamBuffer< char > fileReader( *bvhFile );
   std::string fileContents = fileReader.getBuffer();
   delete bvhFile;

   // parse the bone structure
   std::stringstream inStream( fileContents );
   BVHHierarchy hierarchy( m_rm, *m_observer );

   SkeletonAnimation* animation = new SkeletonAnimation();
   hierarchy.parse( m_loadedFileName.c_str(), inStream, *animation, scene );

   delete animation;
   animation = NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BVHSkeletonAnimationLoader::BVHSkeletonAnimationLoader( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer )
   : TResourceImporter< SkeletonAnimation >( path, rm, observer )
{
}

///////////////////////////////////////////////////////////////////////////////

void BVHSkeletonAnimationLoader::import( SkeletonAnimation& animation )
{
   const Filesystem& fs = m_rm.getFilesystem();

   // load the file contents into a string
   File* bvhFile = fs.open( m_loadedFileName );
   StreamBuffer< char > fileReader( *bvhFile );
   std::string fileContents = fileReader.getBuffer();
   delete bvhFile;

   // parse the bone structure
   std::stringstream inStream( fileContents );
   BVHHierarchy hierarchy( m_rm, *m_observer );

   Model* scene = new Model();
   hierarchy.parse( m_loadedFileName.c_str(), inStream, animation, *scene );
   delete scene;
}

///////////////////////////////////////////////////////////////////////////////
