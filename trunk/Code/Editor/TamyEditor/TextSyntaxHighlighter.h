/// @file   TamyEditor/TextSyntaxHighlighter.h
/// @brief  a custom syntax highlighter used by the TextEditWidget class
#pragma once

#include <QSyntaxHighlighter>


///////////////////////////////////////////////////////////////////////////////

/**
 * A custom syntax highlighter used by the TextEditWidget class.
 */
class TextSyntaxHighlighter : public QSyntaxHighlighter
{
   Q_OBJECT

public:
   struct HighlightingRule
   {
      QRegExp                    pattern;
      QTextCharFormat            format;
      bool                       enabled;

      HighlightingRule() : enabled( true ) {}
   };

private:
   QVector< HighlightingRule >   m_highlightingRules;

   int                           m_textInstanceHighlightRule;

public:
   /**
    * Constructor.
    */
   TextSyntaxHighlighter();
   virtual ~TextSyntaxHighlighter();


   /**
    * Highlights all instances of the specified text.
    *
    * @param text
    */
   void highlightAllInstances( const QString& text );

   /**
    * Appends a new highlighting rule.
    *
    * @return  a reference to the new rule
    */
   HighlightingRule& appendRule();

protected:
   void highlightBlock( const QString &text );
   virtual void onHighlightBlock( const QString &text ) {}

private:
   void initializeDefaultRules();
};

///////////////////////////////////////////////////////////////////////////////

