#include "MaterialSyntaxHighlighter.h"
#include <QTextDocument>


///////////////////////////////////////////////////////////////////////////////

MaterialSyntaxHighlighter::MaterialSyntaxHighlighter( QTextDocument* editedDoc )
   : QSyntaxHighlighter( editedDoc )
{
   HighlightingRule rule;

   m_keywordFormat.setForeground( Qt::darkBlue );
   m_keywordFormat.setFontWeight( QFont::Bold );
   QStringList keywordPatterns;
   keywordPatterns << "\\bstruct\\b" << "\\bclass\\b" << "\\bconst\\b" << "\\bbool\\b"
      << "\\bfloat\\b" << "\\bfloat2\\b" << "\\bfloat3\\b" << "\\bfloat4\\b"
      << "\\bfloat3x3\\b" << "\\bfloat3x4\\b" << "\\bfloat4x3\\b" << "\\bfloat4x4\\b"
      << "\\bint\\b" << "\\bint2\\b" << "\\bint3\\b" << "\\bint4\\b"
      << "\\btexture\\b" << "\\bvoid\\b" << "\\bsampler\\b" << "\\bsampler_state\\b";
   foreach ( const QString &pattern, keywordPatterns ) 
   {
      rule.pattern = QRegExp( pattern );
      rule.format = m_keywordFormat;
      m_highlightingRules.append( rule );
   }

   m_classFormat.setFontWeight( QFont::Bold );
   m_classFormat.setForeground( Qt::darkMagenta );
   rule.pattern = QRegExp( "\\bQ[A-Za-z]+\\b" );
   rule.format = m_classFormat;
   m_highlightingRules.append( rule );

   m_quotationFormat.setForeground( Qt::darkGreen );
   rule.pattern = QRegExp( "\".*\"" );
   rule.format = m_quotationFormat;
   m_highlightingRules.append( rule );

   m_functionFormat.setFontItalic( true );
   m_functionFormat.setForeground( Qt::blue );
   rule.pattern = QRegExp( "\\b[A-Za-z0-9_]+(?=\\()" );
   rule.format = m_functionFormat;
   m_highlightingRules.append( rule );

   QBrush commentsBrush( QColor( 99, 128, 52 ) );
   m_singleLineCommentFormat.setForeground( commentsBrush );
   rule.pattern = QRegExp( "//[^\n]*" );
   rule.format = m_singleLineCommentFormat;
   m_highlightingRules.append( rule );

   m_multiLineCommentFormat.setForeground( commentsBrush );

   m_commentStartExpression = QRegExp( "/\\*" );
   m_commentEndExpression = QRegExp( "\\*/" );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialSyntaxHighlighter::highlightBlock( const QString &text )
{
   // highlight keywords
   foreach ( const HighlightingRule &rule, m_highlightingRules ) 
   {
      QRegExp expression( rule.pattern );
      int index = expression.indexIn( text );
      while ( index >= 0 ) 
      {
         int length = expression.matchedLength();
         setFormat( index, length, rule.format );
         index = expression.indexIn( text, index + length );
      }
   }

   // highlight multiline comments
   int startIndex = 0;
   if ( previousBlockState() != 1 )
   {
      startIndex = m_commentStartExpression.indexIn( text );
   }

   while ( startIndex >= 0 ) 
   {
      int endIndex = m_commentEndExpression.indexIn( text, startIndex );
      int commentLength;
      if ( endIndex == -1 ) 
      {
         setCurrentBlockState( 1 );
         commentLength = text.length() - startIndex;
      } 
      else 
      {
         commentLength = endIndex - startIndex + m_commentEndExpression.matchedLength();
      }
      setFormat( startIndex, commentLength, m_multiLineCommentFormat );
      startIndex = m_commentStartExpression.indexIn( text, startIndex + commentLength );
   }
}

///////////////////////////////////////////////////////////////////////////////
