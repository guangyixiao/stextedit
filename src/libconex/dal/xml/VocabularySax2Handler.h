#ifndef CONEX_DAL_VOCABULARY_SAX2_HANDLER_H
#define CONEX_DAL_VOCABULARY_SAX2_HANDLER_H
#include "xercesc/sax2/DefaultHandler.hpp"
#include "xercesc/framework/XMLFormatter.hpp"
#include "model/Vocabulary.h"
#include <string>

namespace conex {
	using namespace xercesc;
	using namespace std;

	class XMLStringTranslate  : public XMLFormatTarget
	{
	public:
		XMLStringTranslate(const char * const encoding);
		bool TranslatorUTF8ToChinese(string &strTranslatorMsg);
		string translate(const XMLCh* const value);
		const XMLCh * const translate(const char * const value);
		virtual ~XMLStringTranslate();
		virtual void flush();
		// translate
		void UTF_8ToUnicode(wchar_t* pOut,char *pText) ;
		void UnicodeToUTF_8(char* pOut,wchar_t* pText) ;
		void UnicodeToGB2312(char* pOut,wchar_t uData) ;
		void Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer) ;
		void GB2312ToUTF_8(string& pOut,char *pText, int pLen) ;
		void UTF_8ToGB2312(char*pOut, char *pText, int pLen);
		

	protected:
		XMLFormatter * fFormatter;
		XMLCh        *  fEncodingUsed;
		XMLCh        *  toFill; 
		char *  m_value;
	protected:
		enum Constants
		{
			kTmpBufSize     = 16 * 1024,
			kCharBufSize    = 16 * 1024
		};
		void clearbuffer();
		virtual void writeChars(const XMLByte* const toWrite
			, const XMLSize_t   count
			, XMLFormatter* const  formatter);
	};

	class VocabularySax2Handler : public DefaultHandler {
	public:
		VocabularySax2Handler (Vocabulary* voc);
		~VocabularySax2Handler();
		/** @name Default handlers for the DocumentHandler interface */
		//@{
		/**
		* Receive notification of character data inside an element.
		*
		* <p>By default, do nothing.  Application writers may override this
		* method to take specific actions for each chunk of character data
		* (such as adding the data to a node or buffer, or printing it to
		* a file).</p>
		*
		* @param chars The characters.
		* @param length The number of characters to use from the
		*               character array.
		* @exception SAXException Any SAX exception, possibly
		*            wrapping another exception.
		* @see DocumentHandler#characters
		*/
		virtual void characters
			(
			const   XMLCh* const    chars
			, const XMLSize_t       length
			);

		/**
		* Receive notification of the end of the document.
		*
		* <p>By default, do nothing.  Application writers may override this
		* method in a subclass to take specific actions at the beginning
		* of a document (such as finalising a tree or closing an output
		* file).</p>
		*
		* @exception SAXException Any SAX exception, possibly
		*            wrapping another exception.
		* @see DocumentHandler#endDocument
		*/
		virtual void endDocument();

		/**
		* Receive notification of the end of an element.
		*
		* <p>By default, do nothing.  Application writers may override this
		* method in a subclass to take specific actions at the end of
		* each element (such as finalising a tree node or writing
		* output to a file).</p>
		*
		* @param uri The URI of the associated namespace for this element
		* @param localname The local part of the element name
		* @param qname The QName of this element
		* @exception SAXException Any SAX exception, possibly
		*            wrapping another exception.
		* @see DocumentHandler#endElement
		*/
		virtual void endElement
			(
			const XMLCh* const uri,
			const XMLCh* const localname,
			const XMLCh* const qname
			);
		/**
		* Receive notification of the beginning of the document.
		*
		* <p>By default, do nothing.  Application writers may override this
		* method in a subclass to take specific actions at the beginning
		* of a document (such as allocating the root node of a tree or
		* creating an output file).</p>
		*
		* @exception SAXException Any SAX exception, possibly
		*            wrapping another exception.
		* @see DocumentHandler#startDocument
		*/
		virtual void startDocument();

		/**
		* Receive notification of the start of an element.
		*
		* <p>By default, do nothing.  Application writers may override this
		* method in a subclass to take specific actions at the start of
		* each element (such as allocating a new tree node or writing
		* output to a file).</p>
		*
		* @param uri The URI of the associated namespace for this element
		* @param localname the local part of the element name
		* @param qname the QName of this element
		* @param attrs The specified or defaulted attributes.
		* @exception SAXException Any SAX exception, possibly
		*            wrapping another exception.
		* @see DocumentHandler#startElement
		*/
		virtual void startElement
			(
			const   XMLCh* const    uri,
			const   XMLCh* const    localname,
			const   XMLCh* const    qname
			, const Attributes&	attrs
			);

		/**
		* Receive notification of the start of an namespace prefix mapping.
		*
		* <p>By default, do nothing.  Application writers may override this
		* method in a subclass to take specific actions at the start of
		* each namespace prefix mapping.</p>
		*
		* @param prefix The namespace prefix used
		* @param uri The namespace URI used.
		* @exception SAXException Any SAX exception, possibly
		*            wrapping another exception.
		* @see DocumentHandler#startPrefixMapping
		*/
		void tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ");
	private:
		bool _in;
		Vocabulary* _voc;
		string _pos;
		string _state;
		string _element;
		Term* _term;
		XMLStringTranslate* stringTranslate;
	};
}
#endif