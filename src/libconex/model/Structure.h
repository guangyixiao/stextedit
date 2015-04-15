#ifndef CONEX_MODEL_STRUCTURE_H
#define CONEX_MODEL_STRUCTURE_H
#include <string>
namespace conex {
	using namespace std;
	class Structure {
	public:
		Structure() {}
		string iids();
		void setIids(string iids);
		string soundPath();
		void setSoundPath(string path);
		string graphPath();
		void setGraphPath(string path);
		string videoPath();
		void setVideoPath(string path);
	private:
		string _iids;
		string _soundPath;
		string _graphPath;
		string _videoPath;
	};

	inline string Structure::iids() {
		return _iids;
	}

	inline void Structure::setIids(string iids) {
		_iids = iids;
	}

	inline std::string Structure::soundPath() {
		return _soundPath;
	}

	inline void Structure::setSoundPath(std::string path) {
		_soundPath = path;
	}

	inline std::string Structure::graphPath() {
		return _graphPath;
	}

	inline void Structure::setGraphPath(std::string path) {
		_graphPath = path;
	}

	inline std::string Structure::videoPath() {
		return _videoPath;
	}

	inline void Structure::setVideoPath(std::string path) {
		_videoPath = path;
	}
}
#endif