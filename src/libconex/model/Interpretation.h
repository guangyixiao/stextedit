#ifndef CONEX_MODEL_INTERPRETATION_H
#define CONEX_MODEL_INTERPRETATION_H

#include <string>

namespace conex {
	class Interpretation {
	public:
		Interpretation() {}
		~Interpretation() {}
		std::wstring GetTextExplanation() {
			return _textExplanation;
		}
		void SetTextExplanation(std::wstring textEx) {
			_textExplanation = textEx;
		}
		std::string GetIidExplanation() {
			return _iidExplanation;
		}
		void SetIidExplanation(std::string iidEx) {
			_iidExplanation = iidEx;
		}
		std::string GetLogicalExpressiion() {
			return _logicalExpression;
		}
		void SetLogicalExpression(std::string logicalEx) {
			_logicalExpression = logicalEx;
		}
	private:
		std::wstring _textExplanation;
		std::string _iidExplanation;
		std::string _logicalExpression;
	};
}
#endif