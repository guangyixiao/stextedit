#include "VocabularyXmlControl.h"
#include "dal/VocabularyAccessFactory.h"
#include "dal/xml/VocabularyXmlAccess.h"

namespace conex {
	VocabularyXmlControl::VocabularyXmlControl(VocabularyXmlAccess* vocAccess, std::string file) : VocabularyControl(vocAccess) {
		SetFile(file);
	}

	VocabularyXmlControl::VocabularyXmlControl(Model model) {
		bool isSax2 = model == VocabularyControl::XML_SAX2 ? true : false;
		VocabularyAccessFactory factory;
		m_vocAccess =  factory.GetXmlAccess(isSax2);
	}

	void VocabularyXmlControl::SetFile(std::string file) {
		VocabularyXmlAccess* xmlAccess = dynamic_cast<VocabularyXmlAccess*>(m_vocAccess);
		xmlAccess->SetFilename(file.c_str());
	}

	std::string VocabularyXmlControl::GetFile() {
		VocabularyXmlAccess* xmlAccess = dynamic_cast<VocabularyXmlAccess*>(m_vocAccess);
		return xmlAccess->GetFilename();
	}
}