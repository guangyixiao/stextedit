#ifndef CONEX_MODEL_ID_H
#define CONEX_MODEL_ID_H
#include <string>

namespace conex {
	using namespace std;
	class Id {
	public:
		Id() {}
		Id(std::string iid):_iid(iid) {}
		std::string iid();
		void setIid(std::string iid);
		std::wstring word();
		void setWord(std::wstring word);
		std::string pronunciation();
		void setPronunciation(std::string pron);
	private:
		std::string _iid;
		std::wstring _word;
		std::string _pronunciation;
	};

	inline std::string Id::iid() {
		return _iid;
	}

	inline void Id::setIid(std::string iid) {
		_iid = iid;
	}

	inline std::wstring Id::word() {
		return _word;
	}
	inline void Id::setWord(std::wstring word) {
		_word = word;
	}
	inline std::string Id::pronunciation() {
		return _pronunciation;
	}
	inline void Id::setPronunciation(std::string pron) {
		_pronunciation = pron;
	}
}
#endif