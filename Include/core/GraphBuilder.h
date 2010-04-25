#ifndef _GRAPH_BUILDER_H
#define _GRAPH_BUILDER_H

/// @file   core/GraphBuilder.h
/// @brief  utility for automated graph creation

#include <set>
#include <map>
#include <string>
#include "core\GraphBuilderFactory.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A helper structure that contains the definition of a graph node
 * and the nodes it's dependent on.
 */
struct PipelineChunk
{
   std::string id;
   std::set<std::string> prerequisites;

   PipelineChunk(const std::string& _id)
      : id(_id)
   {}

   void definePrerequisite(const std::string& name)
   {
      prerequisites.insert(name);
   }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A utility for automated graph creation.
 */
template<typename NODE, typename EDGE>
class GraphBuilder
{
private:
   typedef Graph<NODE, EDGE> GRAPH;

   typedef std::map<std::string, typename GRAPH::Index> NodesMap;
   NodesMap m_nodesMap;

   GraphBuilderFactory<NODE, EDGE>& m_factory;
   GRAPH& m_graph;

public:
   /**
    * Constructor.
    *
    * @param factory    factory that will create nodes and edges
    * @param graph      an empty graph we want to fill with contents
    */
   GraphBuilder(GraphBuilderFactory<NODE, EDGE>& factory, GRAPH& graph);

   /** 
    * Adds a new piece of information about the graph.
    *
    * @param chunk      piece of data describing our graph
    */ 
   void addChunk(const PipelineChunk& chunk);

private:
   typename GRAPH::Index getNodeIndex(const std::string& id);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\GraphBuilder.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_H
