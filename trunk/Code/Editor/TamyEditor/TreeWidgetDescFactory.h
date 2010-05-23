#pragma once

/// @file   TamyEditor/TreeWidgetDescFactory.h
/// @brief  a common interface for the factories of TreeWidget 
///         item types descriptions


///////////////////////////////////////////////////////////////////////////////

class QIcon;
class QString;

///////////////////////////////////////////////////////////////////////////////

/**
 * A factory for creating the description of node types 
 * the tree will be able to create.
 */
class TreeWidgetDescFactory
{
public:
   virtual ~TreeWidgetDescFactory() {}

   virtual unsigned int typesCount() const = 0;

   virtual void getDesc( unsigned int idx, QString& outDesc, QIcon& outIcon ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
