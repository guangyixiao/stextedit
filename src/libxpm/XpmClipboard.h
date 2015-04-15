#ifndef XPMDOC_MODEL_XPMCLIPBOARD_H
#define XPMDOC_MODEL_XPMCLIPBOARD_H

#include "model/XpmTermBlock.h"

namespace xpm {
	class XpmClipboard {
	public:
		XpmClipboard() {}
		~XpmClipboard();
		TermBlocks & blocks();
		int length();
		void setLength(int len);
		int pos();
		void setPos(int pos);
		void clear();
	private:
		TermBlocks _blocks;
		int _length;
		int _pos;
	};

	inline XpmClipboard::~XpmClipboard() {
		clear();
	}

	inline TermBlocks & XpmClipboard::blocks() {
		return _blocks;
	}

	inline int XpmClipboard::length() {
		return _length;
	}

	inline void XpmClipboard::setLength(int len) {
		_length = len;
	}

	inline int XpmClipboard::pos() {
		return _pos;
	}

	inline void XpmClipboard::setPos(int pos) {
		_pos = pos;
	}
}
#endif