/// @file   TamyEditor/ResourceDropArea.h
/// @brief  Drop area for resources
#pragma once

#include <QFrame>
#include <vector>
#include <string>
#include "core/FilePath.h"


///////////////////////////////////////////////////////////////////////////////

class DropArea;

///////////////////////////////////////////////////////////////////////////////

/**
 * Drop area for resources.
 */
class ResourceDropArea : public QFrame
{
   Q_OBJECT

private:
   std::vector< std::string >       m_paths;
   DropArea*                        m_resourceName;

   FilePath                         m_path;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    */
   ResourceDropArea( QWidget* parentWidget );

   /**
    * Returns the file path.
    */
   inline const FilePath& getFilePath() const { return m_path; }

public slots:
   void valChanged();
   void valErased( bool );

private:
   void refreshPropertyName();
};

///////////////////////////////////////////////////////////////////////////////
