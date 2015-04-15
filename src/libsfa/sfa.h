#ifndef _LIBSFA_H
#define _LIBSFA_H
#include <string>
#include <list>
#include <vector>

#define LIBSFA_VERSION 100

using namespace std;

// a range on a text
struct sfa_range {
	int pos;
	int len;
};
// the element of word sign with word, word index, and a universal identifier
struct sfa_map {
	wstring word;
	int word_ix;
	string id;
};
// the current select of a substr on a text
struct sfa_select {
	int word_ix;
	int block_ix;
	wstring word;
};
// the current select of a substr on a text
struct sfa_select_map {
	int word_ix;
	int block_ix;
	wstring word;
	string id;
};
// a source text can be seperated to a pair of text content and  the maps
struct sfa_pair {
	string content;
	string maps;
};
// the editing function may generate two text ranges from broken word maps, one is on the left of the editing position, and the other is on the rifht of the position
typedef pair<sfa_range, sfa_range> sfa_ranges;

// block definition
// an list of word maps
typedef list<sfa_map> sfa_maps;

// the maps block involves an word maps list and the block index
class sfa_block {
public:
	sfa_block(sfa_maps& maps, int ix = 0);
	sfa_block(int ix = 0):_ix(ix) {}
	void push_back(const sfa_map& map);
	int size();
	int ix();
	void setIx(int ix);
	int end();
	int start();
	sfa_maps& maps();
	// important process
	sfa_ranges add_map(int word_ix, wstring word, string id); 
	sfa_ranges insert_chars(int pos, int len);
	sfa_ranges delete_chars(int pos, int len);
	// for translate
	void insert_before(int pos, int len);
	void delete_befoer(int pos, int len);
private:
	sfa_maps _maps;
	int _ix;
};

// model definition
// an list of blocks
typedef list<sfa_block*> sfa_blocks;

// the maps model involves an list of map blocks
class sfa_model {
public:
	sfa_model(sfa_blocks& blocks, string lang = "en");
	sfa_model(string lang = "en"):_lang(lang) {};
	~sfa_model();
	void push_back(sfa_block* block);
	sfa_ranges add_map(int word_ix, int block_ix, wstring word, string id); 
	sfa_range remove_map(int word_ix, int block_ix);
	sfa_ranges insert_chars(int pos, int len);
	sfa_ranges delete_chars(int pos, int len);
	sfa_map* current_map(int pos);	
	vector<sfa_map> find_maps(int pos, int len);
	sfa_blocks& blocks();
	int total_maps();
	string lang();
	void set_lang(string lang);
	// operator assign
	sfa_model& operator=(sfa_model& m);
	// for translate
	void insert_before(int pos, int len);
	void delete_befoer(int pos, int len);
	void clear();
private:
	sfa_blocks _blocks;
	string _lang;
};

// interface functions
class sfa {
public:
	sfa();
	~sfa();	
	// Interface functions
	// parse a string of source to one string of content and a string of maps
	sfa_pair parse_source(string source);
	// parse a string of maps to the sfa model
	sfa_model* parse_model(const string& maps);
	// write a sfa model to a string of maps
	string write_model(sfa_model* model = 0);
	// write a string of content and sfa model to a string of target
	string write_target(string content, sfa_model* model = 0);
	// write a string of content and sfa model to a file with filename
	int write_file(string file, string content, sfa_model* model = 0);
	// add a map of a word in a block with word index and block index and an universal identifier
	sfa_ranges add_map(int word_ix, wstring word, string id, int block_ix = 0, sfa_model *m = 0); 
	// remove a map with word index and block index
	sfa_range remove_map(int word_ix, int block_ix = 0, sfa_model *m = 0);
	// an callback function to trigle the insert chars function when a set of chars with position and length is inserted
	sfa_ranges insert_chars(int pos, int len, sfa_model *m = 0);
	// an callback function to trigle the delete chars function when a set of chars with position and length is deleted
	sfa_ranges delete_chars(int pos, int len, sfa_model *m = 0);
	// get the current map in the current position
	sfa_map* current_map(int pos, sfa_model *m = 0);	
	// get the number of maps on sfa model
	int total_maps(sfa_model *m = 0);
	// get the language of model
	string lang(sfa_model *m = 0);
	// get the cache model
	sfa_model* model();
	// get the safe model
	sfa_model* safe_model(sfa_model* m);
	// set the cache model
	void set_model(sfa_model* m);
	// get the current select in the content
	// get the cache select
	sfa_select& select();
	// set cache select
	void set_select(int word_ix, wstring word, int block_ix = 0);
private:
	sfa_model* _model;
	sfa_select _select;
};

inline sfa_model* sfa::safe_model(sfa_model* m) {
	if( m == 0) {
		_model = _model == 0 ? _model = new sfa_model() : _model;
		m = _model;
	}
	return m;
}
inline sfa_model* sfa::model() {
	return _model;
}

inline void sfa::set_model(sfa_model *m) {
	_model = m;
}

inline sfa_select& sfa::select() {
	return _select;
}
inline void sfa::set_select(int word_ix, wstring word, int block_ix) {
	_select.word_ix = word_ix;
	_select.block_ix = block_ix;
	_select.word = word;
}

#endif