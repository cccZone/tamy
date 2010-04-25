#ifndef _GRAPH_BUILDER_H
#error "This file can only be included from GraphBuilder.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename NODE, typename EDGE>
GraphBuilder<NODE, EDGE>::GraphBuilder(GraphBuilderFactory<NODE, EDGE>& factory, GRAPH& graph)
: m_factory(factory)
, m_graph(graph)
{
   m_graph.addNode(m_factory.createStartNode());
}

///////////////////////////////////////////////////////////////////////////////

template<typename NODE, typename EDGE>
void GraphBuilder<NODE, EDGE>::addChunk(const PipelineChunk& chunk)
{
   GRAPH::Index mainNodeIdx = getNodeIndex(chunk.id);
   GRAPH::Index prereqNodeIdx = GRAPH::InvalidIndex;

   if (chunk.prerequisites.size() > 0)
   {
      typedef std::set<std::string>::const_iterator PereqIterator;
      for (PereqIterator prereqIt = chunk.prerequisites.begin();
         prereqIt != chunk.prerequisites.end(); ++prereqIt)
      {
         prereqNodeIdx = getNodeIndex(*prereqIt);
         m_graph.connect(prereqNodeIdx, m_factory.createEdge(mainNodeIdx));
      }
   }
   else
   {
      m_graph.connect(0, m_factory.createEdge(mainNodeIdx));
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename NODE, typename EDGE>
typename GraphBuilder<NODE, EDGE>::GRAPH::Index 
GraphBuilder<NODE, EDGE>::getNodeIndex(const std::string& id)
{
   NodesMap::iterator it = m_nodesMap.find(id);
   typename GRAPH::Index nodeIdx = GRAPH::InvalidIndex;

   if (it == m_nodesMap.end())
   {
      nodeIdx = m_graph.addNode(m_factory.createNode(id));
      m_nodesMap.insert(std::make_pair(id, nodeIdx));
   }
   else
   {
      nodeIdx = it->second;
   }

   return nodeIdx;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_H
