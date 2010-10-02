/// @file   TamyEditor/MaterialSyntaxHighlighter.h
/// @brief  material syntax highlighter

#pragma once

#include <QSyntaxHighlighter>


///////////////////////////////////////////////////////////////////////////////

/**
 * Material syntax highlighter.
 */
class MaterialSyntaxHighlighter : public QSyntaxHighlighter
{
   Q_OBJECT

private:
   struct HighlightingRule
   {
      QRegExp                    pattern;
      QTextCharFormat            format;
   };
   QVector< HighlightingRule >   m_highlightingRules;

   QRegExp                       m_commentStartExpression;
   QRegExp                       m_commentEndExpression;

   QTextCharFormat               m_keywordFormat;
   QTextCharFormat               m_classFormat;
   QTextCharFormat               m_singleLineCommentFormat;
   QTextCharFormat               m_multiLineCommentFormat;
   QTextCharFormat               m_quotationFormat;
   QTextCharFormat               m_functionFormat;

public:
   /**
    * Constructor.
    *
    * @param editedDoc
    */
   MaterialSyntaxHighlighter( QTextDocument* editedDoc = 0 );

protected:
   void highlightBlock( const QString &text );
};

///////////////////////////////////////////////////////////////////////////////
