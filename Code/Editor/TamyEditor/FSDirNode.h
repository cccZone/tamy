#pragma 

/// @file   TamyEditor\FSDirNode.h
/// @brief  A node describing a directory in a file system tree structure.

#include "FSTreeNode.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A node describing a directory in a file system tree structure.
 */
class FSDirNode : public FSTreeNode
{
private:
   std::string          m_fsNodeName;

public:
   /**
    * Constructor.
    *
    * @param parent
    * @param nodeName
    * @param fs         file system with the editor icons
    */
   FSDirNode( FSTreeNode* parent, const std::string& nodeName, const Filesystem& fs );

   // -------------------------------------------------------------------------
   // FSTreeNode implementation
   // -------------------------------------------------------------------------
   bool compareNodeName( const std::string& name ) const;
   std::string getRelativePath() const;
   TreeWidgetDescFactory* getDescFactory( ResourcesBrowser& resourcesFactory );
   void addNode( unsigned int typeIdx, ResourcesBrowser& resourcesFactory );
   void editResource( ResourcesBrowser& editorsFactory );

};

///////////////////////////////////////////////////////////////////////////////
