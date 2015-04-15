#include "VocabularyDbAccess.h"
#include "model/Term.h"
#include <map>

namespace conex {
	VocabularyDbAccess::VocabularyDbAccess() {
		otl_connect::otl_initialize(); // initialize ODBC environment
		try{
			//string connstring = "Server=XIAO-PC//SQLEXPRESS;Database=Vocabulary;Trusted_Connection=True;";

			//db.rlogon("SERVER=XIAO-PC/SQLEXPRESS;UID=admin;PWD=admin"); // connect to ODBC
			//db.rlogon("Server=XIAO-PC//SQLEXPRESS;Database=Vocabulary;Trusted_Connection=True;", 0);        
			//或者使用下面的连接语句方式。 
			db.rlogon("sa/easy2hearfrom@XiaoCOPEX"); // connect to ODBC, alternative format
			cout << "We have login the database !" << endl;
		}

		catch(otl_exception& p){ // longercept OTL exceptions
			cerr<<p.msg<<endl; // prlong out error message
			cerr<<p.stm_text<<endl; // prlong out SQL that caused the error
			cerr<<p.sqlstate<<endl; // prlong out SQLSTATE message
			cerr<<p.var_info<<endl; // prlong out the variable that caused the error
		}
	}

	VocabularyDbAccess::~VocabularyDbAccess() {
		db.logoff();
		cout << "We have logoff the database !" << endl;
	}

	void VocabularyDbAccess::Load(Vocabulary& voc) {
		LoadTerms(voc);
	}

	void VocabularyDbAccess::DropAndCreateTable() {
		try{
			otl_cursor::direct_exec
				(
				db,
				"drop table En_Vocabulary_Xml_Out",
				otl_exception::disabled // disable OTL exceptions
				); // drop table

			otl_cursor::direct_exec
				(
				db,
				"create table En_Vocabulary_Xml_Out(iid varchar(100), text varchar(100), pos varchar(100), textExplanation text)"
				);  // create table
		}

		catch(otl_exception& p){ // intercept OTL exceptions
			cerr<<p.msg<<endl; // print out error message
			cerr<<p.stm_text<<endl; // print out SQL that caused the error
			cerr<<p.sqlstate<<endl; // print out SQLSTATE message
			cerr<<p.var_info<<endl; // print out the variable that caused the error
		}
	}

	void VocabularyDbAccess::Save(Vocabulary& voc) {
		DropAndCreateTable();
		try {			
			otl_stream o;
			o.set_lob_stream_mode(true);
			o.open(1, // buffer size may be > 1
				"insert into En_Vocabulary_Xml_Out values (:iid<char[500]>, :text<char[500]>, :pos<char[500]>, :textExpla<varchar_long>)", 
				// SELECT statement
				db // connect object
				); 
			o.set_commit(0);
			otl_lob_stream lob;
			map<string, Term*> terms = voc.GetTerms();
			for(map<string, Term*>::iterator it = terms.begin(); it !=  terms.end(); ++it)
			{
				Term* term = it->second;
				//i >>iid;
				string iid = term->GetIid()->GetIid();
				o << iid;
				string text = term->GetStructure()->GetText();
				o << text;
				string pos = term->GetStructure()->GetPos();
				o << pos;				
				o << lob;				
				string textExpla = term->GetInterpretation()->GetTextExplanation();
				lob.set_len(textExpla.length());
				lob << textExpla;
				lob.close();
				//o << term->GetInterpretation()->GetTextExplanation();
				//term->GetIid()->SetIid(iid);
			}
			db.commit();
		}catch(otl_exception& p){ // longercept OTL exceptions
			cerr<<p.msg<<endl; // prlong out error message
			cerr<<p.stm_text<<endl; // prlong out SQL that caused the error
			cerr<<p.sqlstate<<endl; // prlong out SQLSTATE message
			cerr<<p.var_info<<endl; // prlong out the variable that caused the error
		}
	}

	void VocabularyDbAccess::LoadTerms(Vocabulary& voc) {
		try {
			otl_stream i(4000, // buffer size may be > 1
				"select iid, text, pos, textExplanation from En_Vocabulary_Xml ", 
				// SELECT statement
				db // connect object
				); 
			std::string  iid, text, pos, textExpla;
			db.set_max_long_size(7000);
			while (!i.eof())
			{				
				i >>iid;
				Term* term = voc.CreateTerm(iid);
				i >>text;
				i >>pos;
				i >>textExpla;
				//term->GetIid()->SetIid(iid);
				term->GetStructure()->SetText(text);
				term->GetStructure()->SetPos(pos);
				term->GetInterpretation()->SetTextExplanation(textExpla.c_str());
			}
		}catch(otl_exception& p){ // longercept OTL exceptions
			cerr<<p.msg<<endl; // prlong out error message
			cerr<<p.stm_text<<endl; // prlong out SQL that caused the error
			cerr<<p.sqlstate<<endl; // prlong out SQLSTATE message
			cerr<<p.var_info<<endl; // prlong out the variable that caused the error
		}
	}
}