#include "VocabularyControl.h"
#include "dal/VocabularyAccess.h"
#include "dal/VocabularyAccessFactory.h"

namespace conex {
	VocabularyControl::VocabularyControl(VocabularyAccess* vocAccess) {
		m_vocAccess = vocAccess;
	}

	VocabularyControl::VocabularyControl(Model model) {
		int type = 1;
		switch(model) {
		case XML_DOM:
			type = 2;
			break;
		case DB:
			type = 3;
			break;
		}
		VocabularyAccessFactory factory;
		m_vocAccess =  factory.GetAccess(type);
	}

	void VocabularyControl::Load(Vocabulary& voc) {
		m_vocAccess->Load(voc);
	}

	void VocabularyControl::Save(Vocabulary& voc) {
		if(voc.GetTerms().size() > 0)
		    m_vocAccess->Save(voc);
	}

	void VocabularyControl::Clear(Vocabulary& voc) {
		voc.ClearTerms();
	}
}