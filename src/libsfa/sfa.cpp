#include <iostream>
#include <fstream>
#include <streambuf>
#include "sfa.h"
#include "libjson/libjson.h"
// blocks
sfa_block::sfa_block(sfa_maps& maps, int ix) : _ix(ix) {
	_maps.swap(maps);
}

void sfa_block::push_back(const sfa_map& map) {
	_maps.push_back(map);
}

void sfa_block::clear() {
	_ix = 0;
	_maps.clear();
}

int sfa_block::size() {
	return _maps.size();
}

void sfa_block::setIx(int ix) {
	_ix = ix;
}

int sfa_block::ix() {
	return _ix;
}

int sfa_block::end() {
	if(_maps.size() > 0) {
		sfa_map map = *_maps.rbegin();
		return _ix + map.word_ix + map.word.length();
	}else
		return 0;
}

int sfa_block::start() {
	if(_maps.size() > 0) {
		return _ix + _maps.begin()->word_ix;
	}else
		return _ix;
}

sfa_maps& sfa_block::maps() {
	return _maps;
}

sfa_ranges sfa_block::add_map(int word_ix, wstring word, string id) {
	sfa_ranges ranges;
	int len = word.length();
	word_ix -= _ix;
	int end = word_ix + len;
	sfa_maps::iterator b = _maps.end();
	sfa_maps::iterator c = _maps.end();
	bool bRemove = false;
	// 1. find the before iterator
	for(sfa_maps::iterator i = _maps.begin(); i != _maps.end(); ++i) {
		int i_end = i->word_ix + i->word.length();
		if(i->word_ix < word_ix && i_end  > word_ix) {
			sfa_range l = {_ix + i->word_ix, _ix + word_ix - i->word_ix };
			sfa_range r = {_ix + end, _ix + i_end - word_ix};
			ranges.first = l;
			ranges.second = r;
			bRemove = true;
			c = i;
			b = i;
			if( b != _maps.end())
				++b;
		}else if(i->word_ix >= word_ix) {
			if(b == _maps.end())
				b = i;
		}
	}
	// 2. find current iterator, in order to replace current one
	if( bRemove && c != _maps.end()) {
		_maps.erase(c);
	}
	// 3. insert current map
	if(id.size() > 0) {
		sfa_map map = {word, word_ix, id};
		_maps.insert(b, map);
		wcout << L"inserted for map :" << map.word << L" in position :" << word_ix << L"with length :" << len << endl;
    }
	return ranges;
}

sfa_ranges sfa_block::insert_chars(int pos, int len) {
	sfa_ranges ranges;
	sfa_maps::iterator c = _maps.end();
	pos -= _ix;
	int end = pos + len;
	int bRemove = false;
	for(sfa_maps::iterator i = _maps.begin(); i != _maps.end();) {
		// case 3
		if( pos >= int(i->word_ix + i->word.length()))
			++i;
		// case 1
		else if( pos <= i->word_ix) {
			i->word_ix = i->word_ix + len;
			++i;
		}
		// case 2
		else {
			sfa_range l = {_ix + i->word_ix, i->word.length() + len};
			ranges.first = l;
			i = _maps.erase(i);
		}
	}
	return ranges;
}

sfa_ranges sfa_block::delete_chars(int pos, int len) {
	//1. need to adjust the block index
	if( pos < _ix) {
		// adjust the number of chars removed
		len = pos + len - _ix;
		// move the block index to the pos
		_ix = pos;
	}
	sfa_ranges ranges;
	int end = pos + len;
	//2. adjust the index in the block
	pos -= _ix;
	end -= _ix;
	//int end = pos + len;
	
	//3. only one iteraty is needed to solve the problem
	for(sfa_maps::iterator i = _maps.begin(); i != _maps.end(); ) {
		int i_end = i->word_ix + i->word.length();
		// find the start map and the end map
		// case 1
		if( end <= i->word_ix) {
			i->word_ix = i->word_ix - len;
			++i;
		}
		// case 3.3
		else if( pos >= i_end)
			++i;
		// case 2, 3.1, 3.2
		else {
			// case 3.1, 3.2
			if( pos > i->word_ix) {
				sfa_range l = {_ix + i->word_ix, pos - i->word_ix};
				ranges.first = l;
			}
			// case 2
			if( end < i_end) {
				sfa_range r = {_ix + pos, i_end - end};
				ranges.second = r;
			}
			// need to erase
			i = _maps.erase(i);
		}
	}
	return ranges;
}

void sfa_block::insert_before(int pos, int len) {
	pos -= _ix;
	int end = pos + len;
	for(sfa_maps::iterator i = _maps.begin(); i != _maps.end(); ++i) {
		if(pos <= i->word_ix)
			i->word_ix += len;
	}
}

void sfa_block::delete_befoer(int pos, int len) {
	// need to adjust the position
	if( pos < _ix) {
		// adjust the number of chars deleted
		len += pos - _ix;
		// move the index of block to the pos
		_ix = pos;
	}
	int end = pos + len;
	// 2. adjust the local position
	pos -= _ix;
	end -= _ix;
	// 3. only one iteration
	for(sfa_maps::iterator i = _maps.begin(); i != _maps.end(); ++i) {
		if( end <= i->word_ix )
			i->word_ix -= len;
	}
}

// model


sfa_model::sfa_model(sfa_blocks& blocks, string lang) : _lang(lang) {
	_blocks.swap(blocks);
}

sfa_model::~sfa_model() {
	clear();
}

void sfa_model::clear() {
	for(sfa_blocks::iterator i = _blocks.begin(); i != _blocks.end(); ++i) {
		delete *i;
	}
	_blocks.clear();
}

void sfa_model::push_back(sfa_block* block) {
	_blocks.push_back(block);
}

sfa_ranges sfa_model::add_map(int word_ix, int block_ix, wstring word, string id) {
	sfa_blocks::iterator next = _blocks.end();
	for(sfa_blocks::iterator i = _blocks.begin(); i != _blocks.end(); ++i) {
		if((*i)->ix() == block_ix) {
			return (*i)->add_map(word_ix, word, id);
		}else if((*i)->ix() > block_ix) {
			next = i;
			--next;
		}
	}
	sfa_block *block = new sfa_block(block_ix);
	_blocks.insert(next, block);
	return block->add_map(word_ix, word, id);
}
sfa_range sfa_model::remove_map(int word_ix, int block_ix) {
	sfa_range range = {0, 0};
	for(sfa_blocks::iterator i = _blocks.begin(); i != _blocks.end(); ++i) {
		if((*i)->ix() == block_ix) {
			for(sfa_maps::iterator t = (*i)->maps().begin(); t != (*i)->maps().end(); ++t) {
				if(t->word_ix = word_ix - block_ix) {
					range.len = t->word.length();
					range.pos = t->word_ix + (*i)->ix();
					(*i)->maps().erase(t);
					if ((*i)->maps().size() == 0)
						_blocks.erase(i);
					return range;
				}
			}
		}
	}
	return range;
}
sfa_ranges sfa_model::insert_chars(int pos, int len) {
	sfa_ranges ranges;
	// process
	for(sfa_blocks::iterator i = _blocks.begin(); i != _blocks.end(); ++i) {
		// case 3
		if( pos >= (*i)->end()) 
			continue;
		// case 1
		else if( pos < (*i)->ix()) {
			(*i)->setIx((*i)->ix() + len);
		}
		// case 2
		else {
			ranges = (*i)->insert_chars(pos, len);
		}
	}
	return ranges;
}
sfa_ranges sfa_model::delete_chars(int pos, int len) {
	sfa_ranges ranges;
	int end = pos + len;
	for(sfa_blocks::iterator i = _blocks.begin(); i != _blocks.end(); ) {
		// case 1
		if( end <= (*i)->ix()) {
			(*i)->setIx((*i)->ix() - len);
			++i;
		}
		// case 3.1
		else if( end >= (*i)->end() && pos <= (*i)->start()) {
			delete *i;
			*i = 0;
			i = _blocks.erase(i);
		}
		// case 3.3
		else if( end >= (*i)->end() && pos >= (*i)->end()) {
			++i;
		}
		// case 2, 3.2
		else {
			ranges = (*i)->delete_chars(pos, len);
			if((*i)->size() == 0) {
				delete *i;
				*i = 0;
				i = _blocks.erase(i);
			}else {
				++i;
			}
		}
	}
	return ranges;
}
sfa_map* sfa_model::current_map(int pos) {
	if(pos > 0) {
		for(sfa_blocks::iterator i = _blocks.begin(); i != _blocks.end(); ++i) {
			if((*i)->ix() <= pos && (*i)->end() >= pos) {
				for(sfa_maps::iterator t = (*i)->maps().begin(); t != (*i)->maps().end(); ++t) {
					int bpos = pos - (*i)->ix();
					if(bpos >= t->word_ix && bpos < int(t->word_ix + t->word.length()))
						return &*t;
				}
			}
		}
	}
	return 0;
}

vector<sfa_map> sfa_model::find_maps(int pos, int len) {
	vector<sfa_map> maps;
	for(sfa_blocks::const_iterator i = _blocks.cbegin(); i != _blocks.cend(); ++i) {
		if ( ! ( pos >= (*i)->end() || (*i)->ix() >= (pos + len) )) {
			int ipos = pos;
			int ilen = len;
			if( ipos <= (*i)->ix()) {
				ipos = 0;
				ilen = pos + len -(*i)->ix();
			}else
				ipos -= (*i)->ix();
			for(sfa_maps::iterator j = (*i)->maps().begin(); j != (*i)->maps().end(); ++j) {
				if( ipos <= j->word_ix && (ipos + ilen) >= int(j->word_ix + j->word.length())) {
					sfa_map map = {j->word, (*i)->ix() + j->word_ix, j->id};
					maps.push_back(map);
				}
			}
		}
	}
	return maps;
}

sfa_blocks& sfa_model::blocks() {
	return _blocks;
}

int sfa_model::total_maps() {
	int total = 0;
	for(sfa_blocks::iterator it = _blocks.begin(); it != _blocks.end(); ++it) {
		total += (*it)->size();
	}
	return total;
}

string sfa_model::lang() {
	return _lang;
}

void sfa_model::set_lang(string lang) {
	_lang = lang;
}

sfa_model& sfa_model::operator=(sfa_model& m) {
	for(sfa_blocks::const_iterator i = m.blocks().cbegin(); i != m.blocks().cend(); ++i) {
		sfa_block* b = new sfa_block(**i);
		_blocks.push_back(b);
	}
	_lang = m._lang;
	return *this;
}

void sfa_model::insert_before(int pos, int len) {
	for(sfa_blocks::iterator i = _blocks.begin(); i != _blocks.end(); ++i) {
		// we are also insert in the insert chars
		if( pos <= (*i)->ix()) {
			// case 3
			(*i)->setIx((*i)->ix() + len);
		}
	}
}

void sfa_model::delete_befoer(int pos, int len) {
	int end = pos + len;
	for(sfa_blocks::iterator i = _blocks.begin(); i != _blocks.end(); ++i) {
		if( end <= (*i)->ix())
			(*i)->setIx((*i)->ix() - len);
	}
}

// dal

class sfa_dal {
public:
	sfa_model* parse_model(string model);	
	string write_target(string content, sfa_model *model);
	int write_file(string file, string content, sfa_model *model);
private:
	void parse_blocks(JSONNode n, sfa_model* m);
	void parse_maps(JSONNode n, sfa_block* b);
};

bool block_compare(sfa_block* b1, sfa_block* b2) {
	return b1->ix() < b2->ix();
}

sfa_model *sfa_dal::parse_model(string model) {
	//cout << "data:" << model << endl;
	sfa_model *m = 0;
	JSONNode n = libjson::parse(libjson::to_json_string(model));
	if( !n.empty() ) {
		JSONNode::json_iterator i = n.begin();
		while( i != n.end() ) {
			if(json_strcmp((*i).name().c_str(), L"_lang")==0) {
				string lang = libjson::to_std_string((*i).as_string());
				cout << "lang on parse is :" << lang << endl;
				m = new sfa_model();
				m->set_lang(lang);
			}else if(json_strcmp((*i).name().c_str(), L"signs blocks")==0) {
				parse_blocks(*i, m);
			}
			++i;
		}
	}
	if( m && m->blocks().size() > 0)
		m->blocks().sort(block_compare);
	return m;
}

void sfa_dal::parse_blocks(JSONNode n, sfa_model* m) {
	JSONNode::json_iterator i = n.begin();
	//std::cout << "parse with block (1)" << endl;
	while ( i != n.end()) {			
		sfa_block* b = new sfa_block();
		m->push_back(b);
		JSONNode::json_iterator bi = (*i).begin();
		while ( bi != (*i).end()) {
			if(json_strcmp((*bi).name().c_str(), L"bp")== 0) {	
				int ix = (*bi).as_int();
				//std::cout << "ix :" << ix << endl;
				b->setIx(ix);
			}else if ( json_strcmp((*bi).name().c_str(), L"signs")==0 ) {
				parse_maps(*bi, b);
			}
			++bi;
		}
		++i;
	}
	//std::cout << "parse with block (2)" << endl;
}

void sfa_dal::parse_maps(JSONNode n, sfa_block* b) {
	JSONNode::json_iterator i =  n.begin();
	//std::cout << "parse with signs (1)" << endl;
	while ( i != n.end()) {			
		JSONNode::json_iterator ti= (*i).begin();
		int pos = 0;
		string iid = "";
		wstring signifier = L"";
		while ( ti != (*i).end()) {
			wstring node_name = (*ti).name().c_str();
			if( node_name == L"pos") {				
				pos = (*ti).as_int();
			}else if ( node_name == L"signifier" ) {
				signifier = libjson::to_std_wstring ( (*ti).as_string());
				string signifier2 = libjson::to_std_string( (*ti).as_string() );
				//cout << "signifier in string: " << signifier2 << endl;
				//wcout << "signifier in wstring: " << signifier << endl;
			}else if ( node_name == L"signified" ) {
				iid = libjson::to_std_string( (*ti).as_string());
			}
			++ti;
		}
		sfa_map map = {signifier, pos, iid};
		b->push_back(map);
		++i;
	}
	//std::cout << "parse with signs (2)" << endl;
}

string sfa_dal::write_target(string content, sfa_model* m) {
	// get syntax node
	if ( content.size() > 0 ) {
		//json_push_back(syntaxn, json_new_a("test", "test"));
		// get signs node
		string output = content;
		if( m->blocks().size() > 0) {
			//json_string output_j = libjson::to_json_string(output);
			JSONNode n(JSON_NODE);
			//JSONNode blocks(JSON_ARRAY);	
			JSONNode lang(L"_lang", libjson::to_json_string(m->lang()));
			n.push_back(lang);
			JSONNode blocks(JSON_ARRAY);
			blocks.set_name( L"signs blocks");
			for(sfa_blocks::iterator i = m->blocks().begin(); i != m->blocks().end(); ++i) {
				if ( (*i)->size() > 0 ) {
					JSONNode block(JSON_NODE);
					block.push_back(JSONNode(L"bp", (*i)->ix()));
					JSONNode terms(JSON_ARRAY);
					terms.set_name( L"signs");
					for(sfa_maps::iterator j = (*i)->maps().begin(); j != (*i)->maps().end(); ++j) {
						//std::cout << "write terms (1)" << endl;
						JSONNode term(JSON_NODE);	

						term.push_back(JSONNode(L"pos", j->word_ix));
						term.push_back(JSONNode(L"signifier", libjson::to_json_string( j->word)));
						term.push_back(JSONNode(L"signified", libjson::to_json_string( j->id )));
						terms.push_back(term);

						//std::cout << "write terms (2)" << endl;
					}
					block.push_back(terms);
					blocks.push_back( block);
				}
			}
			n.push_back( blocks);
			json_string signsstr = n.write_formatted() ;
			output += '\n' + libjson::to_std_string( signsstr) ;
			//json_push_back(syntaxn, blocks);
		}
		//write file
		//std::cout << output << endl;
		return output;
	}
	return "";
}

int sfa_dal::write_file(string file, string content, sfa_model* m) {
	string output = write_target(content, m);
	ofstream outfile;
	outfile.open(file);
	if(outfile.is_open()) {
		outfile << output.c_str();
		outfile.close();
		return true;
	}
	return false;
}

// sfa api functions
sfa::sfa() {
	_model = new sfa_model();
}
sfa::~sfa() {
	delete _model;
}
sfa_pair sfa::parse_source(string source) {
	// get the syntax
	sfa_pair pair;
	std::size_t pos = source.find("\"_lang");
	std::size_t spos = string::npos;
	string content = source;
	if ( pos != string::npos ) {
		content = source.substr(0, pos) ;
		spos = content.find_last_of("{")-1;
		content = content.substr(0, spos);
	}
	pair.content = content;
	pair.maps = "";
	// get the model maps
	if ( spos != string::npos ) {
		string maps = source.substr(spos, source.size()-spos) ;
		//cout << "maps:" << maps << endl;
		pair.maps = maps;
		//parse signs to model
	}
	return pair;
}
sfa_model* sfa::parse_model(const string& maps) {
	if(_model) {
		delete _model;
		_model = 0;
	}
	sfa_dal dal;
	_model = dal.parse_model(maps);
	return _model;
}
string sfa::write_target(string content, sfa_model* m) {
	m = safe_model(m);
	sfa_dal dal;
	return dal.write_target(content, m);
}
int sfa::write_file(string file, string content, sfa_model* m) {
	m = safe_model(m);
	sfa_dal dal;
	return dal.write_file(file, content, m);
}

sfa_ranges sfa::add_map(int word_ix, wstring word, string id, int block_ix, sfa_model *m) {
	m = safe_model(m);
	return m->add_map(word_ix, block_ix, word, id);
}
sfa_range sfa::remove_map(int word_ix, int block_ix, sfa_model *m) {
	m = safe_model(m);
	return m->remove_map(word_ix, block_ix);
}
sfa_ranges sfa::insert_chars(int pos, int len, sfa_model *m) {
	m = safe_model(m);
	return m->insert_chars(pos, len);
}
sfa_ranges sfa::delete_chars(int pos, int len, sfa_model *m) {
	m = safe_model(m);
	return m->delete_chars(pos, len);
}

int sfa::total_maps(sfa_model *m) {
	m = safe_model(m);
	return m->total_maps();
}

string sfa::lang(sfa_model *m) {
	m = safe_model(m);
	return m->lang();
}

sfa_map* sfa::current_map(int pos, sfa_model *m) {
	m = safe_model(m);
	return m->current_map(pos);
}
