#include "XpmClipboard.h"

namespace xpm {
	void XpmClipboard::clear() {
		_length = 0;
		_pos = 0;
		for(TermBlocksIter it = _blocks.begin(); it != _blocks.end(); ++it)  {
			delete *it;
			*it = 0;
		}
		_blocks.clear();
	}

}