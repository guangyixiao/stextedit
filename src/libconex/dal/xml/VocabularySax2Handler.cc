#include "VocabularySax2Handler.h"
#include <iostream>
#include "xercesc/sax2/Attributes.hpp"
#include "xercesc/util/XMLUniDefs.hpp"
#include "xercesc/framework/XMLFormatter.hpp"
#include "xercesc/util/TransService.hpp"
#include "model/Term.h"
#include "stdio.h"
#include "wchar.h"
#include "windows.h"

using  namespace std;

namespace conex {
	VocabularySax2Handler::VocabularySax2Handler(Vocabulary* voc): _voc(voc), _term(0), _element (""),_in(false) {
		stringTranslate = new XMLStringTranslate("utf-8");
	}

	VocabularySax2Handler::~VocabularySax2Handler() {
		delete stringTranslate;
	}

	void VocabularySax2Handler::startDocument() {

	}

	void VocabularySax2Handler::endDocument() {

	}

	void VocabularySax2Handler::startElement(
		const   XMLCh* const    uri,
		const   XMLCh* const    localname,
		const   XMLCh* const    qname
		, const Attributes&	attrs) {
			const static XMLCh* PREFIX = XMLString::transcode("sdf");
			const static XMLCh* VOCABULARY = XMLString::transcode("voc");
			const static XMLCh* VOC_VID = XMLString::transcode("vid");
			const static XMLCh* VOC_TAG = XMLString::transcode("tag");
			const static XMLCh* VOC_VI = XMLString::transcode("vi");
			const static XMLCh* VI_SUI = XMLString::transcode("sui");
			const static XMLCh* VI_AUTHOR = XMLString::transcode("author");
			const static XMLCh* VOC_VX = XMLString::transcode("vx");
			const static XMLCh* VX_LANG = XMLString::transcode("lang");
			const static XMLCh* VX_TIME = XMLString::transcode("time");
			const static XMLCh* TERM = XMLString::transcode("term");
			const static XMLCh* POS = XMLString::transcode("sdf:pos");
			const static XMLCh* STATU = XMLString::transcode("sdf:state");
			const static XMLCh* ID = XMLString::transcode("id");
			const static XMLCh* ID_IID = XMLString::transcode("iid");
			const static XMLCh* ID_WORD = XMLString::transcode("word");
			const static XMLCh* ID_PRONUNCIATION = XMLString::transcode("pronunciation");
			const static XMLCh* STRUCTURE = XMLString::transcode("structure");
			const static XMLCh* STR_IIDS = XMLString::transcode("iids");
			const static XMLCh* STR_SOUND = XMLString::transcode("sound");
			const static XMLCh* STR_GRAPH = XMLString::transcode("graph");
			const static XMLCh* STR_VIDEO = XMLString::transcode("video");
			const static XMLCh* CONCEPT = XMLString::transcode("concept");			
			const static XMLCh* CON_TEXT = XMLString::transcode("text");
			const static XMLCh* CON_PHONETIC = XMLString::transcode("phonetic");
			const static XMLCh* CON_PICTURE = XMLString::transcode("picture");
			const static XMLCh* CON_FILM = XMLString::transcode("film");
			const static XMLCh* CONTEXT = XMLString::transcode("context");
			const static XMLCh* TX_SYNONYMS = XMLString::transcode("synonyms");
			const static XMLCh* TX_SIMINYMS = XMLString::transcode("siminyms");
			const static XMLCh* TX_CATEGORIES = XMLString::transcode("categories");
			const static XMLCh* TX_ANTONYMS = XMLString::transcode("antonyms");
			const static XMLCh* TX_HYPERNYMS = XMLString::transcode("hypernyms");
			const static XMLCh* TX_DERIVATION = XMLString::transcode("derivation");
			const static XMLCh* TX_EXAMPLES = XMLString::transcode("examples");

			char tempStr[500];
			_in = true;
			if(XMLString::equals(VOCABULARY, localname)) {	
				_element = "voc";
			}else if(XMLString::equals(VOC_VID, localname)) {	
				_element = "vid";
			}else if(XMLString::equals(VOC_TAG, localname)) {	
				_element = "tag";
			}else if(XMLString::equals(VI_SUI, localname)) {	
				_element = "sui";
			}else if(XMLString::equals(VI_AUTHOR, localname)) {	
				_element = "author";
			}else if(XMLString::equals(VX_LANG, localname)) {	
				_element = "lang";
			}else if(XMLString::equals(VX_TIME, localname)) {	
				_element = "time";
			}else if(XMLString::equals(TERM, localname)) {				
				_element = "term";
				const XMLCh* pos = attrs.getValue(POS);
				if(pos != 0 && *pos != 0) {
					XMLString::transcode(pos, tempStr, 499);
					_pos = tempStr;
				}
				const XMLCh* statu = attrs.getValue(STATU);
				if(statu != 0 && *statu != 0) {
					XMLString::transcode(statu, tempStr, 499);
					_state = tempStr;
				}
			}else if(XMLString::equals(ID_IID, localname)) {
				_element = "iid";				
			}else if(XMLString::equals(ID_WORD, localname)) {
				_element = "word";				
			}else if(XMLString::equals(ID_PRONUNCIATION, localname)) {
				_element = "pronunciation";				
			}else if(XMLString::equals(STR_IIDS, localname)) {
				_element = "iids";				
			}else if(XMLString::equals(STR_SOUND, localname)) {
				_element = "sound";				
			}else if(XMLString::equals(STR_GRAPH, localname)) {
				_element = "graph";				
			}else if(XMLString::equals(STR_VIDEO, localname)) {
				_element = "video";				
			}else if(XMLString::equals(CON_TEXT, localname)) {
				_element = "text";				
			}else if(XMLString::equals(CON_PHONETIC, localname)) {
				_element = "phonetic";				
			}else if(XMLString::equals(CON_PICTURE, localname)) {
				_element = "picture";				
			}else if(XMLString::equals(CON_FILM, localname)) {
				_element = "film";				
			}else if(XMLString::equals(TX_SYNONYMS, localname)) {
				_element = "synonyms";				
			}else if(XMLString::equals(TX_SIMINYMS, localname)) {
				_element = "siminyms";				
			}else if(XMLString::equals(TX_CATEGORIES, localname)) {
				_element = "categories";				
			}else if(XMLString::equals(TX_ANTONYMS, localname)) {
				_element = "antonyms";				
			}else if(XMLString::equals(TX_HYPERNYMS, localname)) {
				_element = "hypernyms";				
			}else if(XMLString::equals(TX_DERIVATION, localname)) {
				_element = "derivation";				
			}else if(XMLString::equals(TX_EXAMPLES, localname)) {
				_element = "examples";				
			}else {
				_in = false;
			}
	}

	void VocabularySax2Handler::endElement(
		const XMLCh* const uri,
		const XMLCh* const localname,
		const XMLCh* const qname) {
			_in = false;
	}

	void VocabularySax2Handler::characters (
		const   XMLCh* const    chars
		, const XMLSize_t       length ) {
			if(_in && chars != 0 && *chars != 0) {
				char tempStr[500];
				XMLString::transcode(chars, tempStr, 499);
				if(_element == "vid") {
					_voc->setVid(tempStr);
				}else if(_element == "tag") {
					_voc->setTag(tempStr);
				}else if(_element == "sui") {
					_voc->setSui(tempStr);
				}else if(_element == "author") {
					_voc->setAuthor(tempStr);
				}else if(_element == "lang") {
					_voc->setLang(tempStr);
				}else if(_element == "time") {
					_voc->setTime(tempStr);
				}else if(_element == "iid") {
					_term = _voc->CreateTerm(tempStr);
					/*if(_term == 0)
						_term = _voc->CreateTerm(tempStr);*/
					_term->setPos(_pos);
					_term->setState(_state);
				}else if(_element == "word") {
					_term->id()->setWord(chars);
				}else if(_element == "pronunciation") {
					_term->id()->setPronunciation(tempStr);
				}else if(_element == "text") {
					_term->setText(chars);
				}else if(_element == "phonetic") {
					_term->setPhonetic(tempStr);
				}else if(_element == "picture") {
					_term->setPicturePath(tempStr);
				}else if(_element == "film") {
					_term->setFilmPath(tempStr);
				}else if(_element == "iids") {
					if(strlen(tempStr) > 0)
						_term->structure()->setIids(tempStr);
				}else if(_element == "sound") {
					if(strlen(tempStr) > 0)
						_term->structure()->setSoundPath(tempStr);
				}else if(_element == "graph") {
					if(strlen(tempStr) > 0)
						_term->structure()->setGraphPath(tempStr);
				}else if(_element == "video") {
					if(strlen(tempStr) > 0)
						_term->structure()->setVideoPath(tempStr);
				}else if(_element == "synonyms") {
					// 1. split the synonyms to a vector
					if(strlen(tempStr) >= 14) {
						vector<string> tokens;
						tokenize(tempStr, tokens, ",");					
						// 2. Add each iterator of synonyms to the term contexts
						for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
							_term->context()->AddRelation(SYNONYM, it->c_str());
						}
					}
				}else if(_element == "siminyms") {
					// 1. split the synonyms to a vector
					if(strlen(tempStr) >= 14) {
						vector<string> tokens;
						tokenize(tempStr, tokens, ",");					
						// 2. Add each iterator of synonyms to the term contexts
						for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
							_term->context()->AddRelation(SIMINYM, it->c_str());
						}
					}
				}else if(_element == "categories") {
					// 1. split the synonyms to a vector
					if(strlen(tempStr) >= 14) {
						vector<string> tokens;
						tokenize(tempStr, tokens, ",");					
						// 2. Add each iterator of synonyms to the term contexts
						for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
							_term->context()->AddRelation(CATEGORY, it->c_str());
						}
					}
				}else if(_element == "hypernyms") {
					// 1. split the synonyms to a vector
					if(strlen(tempStr) >= 14) {
						vector<string> tokens;
						tokenize(tempStr, tokens, ",");					
						// 2. Add each iterator of synonyms to the term contexts
						for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
							_term->context()->AddRelation(HYPERNYM, it->c_str());
						}
					}
				}else if(_element == "antonyms") {
					// 1. split the synonyms to a vector
					if(strlen(tempStr) >= 14) {
						vector<string> tokens;
						tokenize(tempStr, tokens, ",");					
						// 2. Add each iterator of synonyms to the term contexts
						for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
							_term->context()->AddRelation(ANTONYM, it->c_str());
						}
					}
				}else if(_element == "derivation") {
					if(strlen(tempStr) > 0) {
						_term->context()->setDerivation(tempStr);
					}
					// 1. split the synonyms to a vector
					// 2. Add each iterator of synonyms to the term contexts
				}else if(_element == "examples") {
					if(strlen(tempStr) > 0) {
						_term->context()->setExamples(tempStr);
					}
					// 1. split the synonyms to a vector
					// 2. Add each iterator of synonyms to the term contexts
				}
			}
	}

	void VocabularySax2Handler::tokenize(const string& str,
		vector<string>& tokens,
		const string& delimiters)
	{
		// Skip delimiters at beginning.
		string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		string::size_type pos     = str.find_first_of(delimiters, lastPos);

		while (string::npos != pos || string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiters, lastPos);
		}
	}

	void XMLStringTranslate::UTF_8ToUnicode(wchar_t* pOut,char *pText)  
	{     
		char* uchar = (char *)pOut;   
		uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);  
		uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);   
	}   
	void XMLStringTranslate::UnicodeToUTF_8(char* pOut,wchar_t* pText)   
	{   
		char* pchar = (char *)pText;   
		pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));  
		pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);  
		pOut[2] = (0x80 | (pchar[0] & 0x3F));  
	}   
	void XMLStringTranslate::UnicodeToGB2312(char* pOut,wchar_t uData)  
	{   
		WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);   
	}  
	void XMLStringTranslate::Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer)   
	{     
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);  
	}   
	void XMLStringTranslate::GB2312ToUTF_8(string& pOut,char *pText, int pLen)  
	{    
		char buf[4];  
		int nLength = pLen* 3;  
		char* rst = new char[nLength];  

		memset(buf,0,4);  
		memset(rst,0,nLength);  

		int i = 0;  
		int j = 0;        
		while(i < pLen)  
		{  
			if( *(pText + i) >= 0)  
			{  
				rst[j++] = pText[i++];  
			}  
			else
			{  
				wchar_t pbuffer;  
				Gb2312ToUnicode(&pbuffer,pText+i);  

				UnicodeToUTF_8(buf,&pbuffer);  

				unsigned short int tmp = 0;  
				tmp = rst[j] = buf[0];  
				tmp = rst[j+1] = buf[1];  
				tmp = rst[j+2] = buf[2];      

				j += 3;  
				i += 2;  
			}  
		}  
		rst[j] = '\0';  

		pOut = rst;               
		delete []rst;     

		return;      
	}     
	void XMLStringTranslate::UTF_8ToGB2312(char*pOut, char *pText, int pLen)    
	{     
		char * newBuf = new char[pLen];  
		char Ctemp[4];  
		memset(Ctemp,0,4);  

		int i =0;  
		int j = 0;  

		while(i < pLen)  
		{  
			if(pText[i] > 0)  
			{  
				newBuf[j++] = pText[i++];                         
			}  
			else                   
			{  
				WCHAR Wtemp;  
				UTF_8ToUnicode(&Wtemp,pText + i);  

				UnicodeToGB2312(Ctemp,Wtemp);  

				newBuf[j] = Ctemp[0];  
				newBuf[j + 1] = Ctemp[1];  

				i += 3;      
				j += 2;     
			}  
		}  
		newBuf[j] = '\0';  

		pOut = newBuf;  
		delete []newBuf;  

		return;  
	} 	

	XMLStringTranslate::XMLStringTranslate(const char * const encoding):fFormatter(0),
		m_value(0),fEncodingUsed(0),toFill(0)
	{
		XMLFormatTarget * myFormTarget = this;
		fEncodingUsed=XMLString::transcode(encoding);
		fFormatter = new XMLFormatter(fEncodingUsed
			,myFormTarget
			,XMLFormatter::NoEscapes
			,XMLFormatter::UnRep_CharRef);
		toFill=new XMLCh[kTmpBufSize];
		clearbuffer();
	}
	XMLStringTranslate::~XMLStringTranslate()
	{
		if(fFormatter)
			delete fFormatter;
		if(fEncodingUsed)
			delete [] fEncodingUsed;
		if(m_value)
			free(m_value);
		if(toFill)
			free(toFill);

		fFormatter=0;
		fEncodingUsed=0;
		m_value=0;
		toFill=0;
	}
	void XMLStringTranslate::writeChars(const XMLByte* const  toWrite
		, const XMLSize_t    count
		, XMLFormatter* const   formatter)
	{
		if(m_value)
			free(m_value);
		m_value=0;
		m_value=new char[count+1];
		memset(m_value,0,count+1);
		memcpy(m_value,(char *)toWrite,count+1);
	}
	void XMLStringTranslate::clearbuffer()
	{
		if(!toFill)
			return;
		for(int i=0;i<kTmpBufSize;i++)
			toFill[i]=0;
	}
	string XMLStringTranslate::translate(const XMLCh* const value)   
	{
		*fFormatter<<value;
		string strValue=string(m_value);
		return strValue;
	}
	const XMLCh * const XMLStringTranslate::translate(const char * const value)
	{
		clearbuffer();
		const unsigned int  srcCount=XMLString::stringLen(value);
		unsigned char fCharSizeBuf[kCharBufSize];
		XMLTranscoder * pTranscoder=(XMLTranscoder *)fFormatter->getTranscoder(); 
		XMLSize_t bytesEaten;
		unsigned int size=pTranscoder->transcodeFrom(
			(XMLByte *)value,
			srcCount,
			toFill,
			kTmpBufSize,
			bytesEaten,
			fCharSizeBuf
			);
		toFill[size]=0;
		string t1=string(value);
		string t2=translate(toFill);
		assert(t1==t2);
		return toFill;
	}
	bool XMLStringTranslate::TranslatorUTF8ToChinese(string &strTranslatorMsg)     
	{
		char* pstrSource = const_cast<char*>(strTranslatorMsg.c_str());
		int len = strTranslatorMsg.length()*2+1;
		char* pstrDestination = new char[len];  
		memset(pstrDestination, '/0', strTranslatorMsg.length()*2+1);
		UTF_8ToGB2312(pstrDestination, pstrSource, strTranslatorMsg.length());		
		strTranslatorMsg = pstrDestination; 
		delete[] pstrDestination;
		return true;
	}

	void XMLStringTranslate::flush() {

	}
}
