#include "TextSyntaxHighlighter.h"


///////////////////////////////////////////////////////////////////////////////

TextSyntaxHighlighter::TextSyntaxHighlighter()
   : QSyntaxHighlighter( (QTextDocument*)NULL )
{
   initializeDefaultRules();
}

///////////////////////////////////////////////////////////////////////////////

TextSyntaxHighlighter::~TextSyntaxHighlighter()
{
}

///////////////////////////////////////////////////////////////////////////////

void TextSyntaxHighlighter::initializeDefaultRules()
{
   // selected text instances highlight
   m_textInstanceHighlightRule = 0;
   HighlightingRule& selectedTextHighlight = appendRule();
   selectedTextHighlight.format.setBackground( QColor( 91, 203, 255 ) );
   selectedTextHighlight.format.setForeground( QColor( 255, 255, 255 ) );
   selectedTextHighlight.enabled = false;
}

///////////////////////////////////////////////////////////////////////////////

void TextSyntaxHighlighter::highlightAllInstances( const QString& text )
{
   QString pattern = tr( "\\b" ) + text + tr( "\\b" );

   HighlightingRule& rule = m_highlightingRules[m_textInstanceHighlightRule];
   rule.pattern = QRegExp( pattern );
   rule.enabled = text.length() > 0;

   rehighlight();
}

///////////////////////////////////////////////////////////////////////////////

TextSyntaxHighlighter::HighlightingRule& TextSyntaxHighlighter::appendRule()
{
   m_highlightingRules.append( HighlightingRule() );
   return m_highlightingRules.back();
}

///////////////////////////////////////////////////////////////////////////////

void TextSyntaxHighlighter::highlightBlock( const QString &text )
{
   // highlight keywords
   foreach ( const HighlightingRule &rule, m_highlightingRules ) 
   {
      if ( !rule.enabled )
      {
         continue;
      }

      QRegExp expression( rule.pattern );
      int index = expression.indexIn( text );
      while ( index >= 0 ) 
      {
         int length = expression.matchedLength();
         if ( length <= 0 )
         {
            break;
         }
         setFormat( index, length, rule.format );
         index = expression.indexIn( text, index + length );
      }
   }

   onHighlightBlock( text );
}

///////////////////////////////////////////////////////////////////////////////
