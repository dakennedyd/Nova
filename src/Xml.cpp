#include <string>
#include <functional>
#include "logger/Logger.h"
#include "Xml.h"

namespace Nova {	
	XMLFile::XMLFile(const std::string & filename)
	{
		LOG_DEBUG("loading XML file:" << filename);
		if (mDocument.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
		{
			LOG_WARNING("Unable to load XML file:" << filename);
		}		
	}	
	XMLNode XMLNode::getFirstChild() const
	{
		if (mElement)
			return XMLNode(mElement->FirstChildElement());
		else
			return XMLNode();
	}
	XMLNode XMLNode::getNextElement() const
	{
		if (mElement)
			return XMLNode(mElement->NextSiblingElement());
		else
			return XMLNode();		
	}
	XMLNode XMLNode::getChildElement(const std::string & elementName) const
	{
		if (mElement)
			return XMLNode(mElement->FirstChildElement(elementName.c_str()));
		else
			return XMLNode();		
	}
	std::string XMLNode::getText() const
	{
		std::string s("NO TEXT");		
		if (mElement && mElement->GetText())			
			s = mElement->GetText();			
		return s;
	}
	std::string XMLNode::getName() const
	{
		std::string s("NO NAME");
		if (mElement && mElement->Name())
			s = mElement->Name();
		return s;
		
	}
	std::string XMLNode::getAttributeValue(const std::string attribute) const
	{		
		std::string s("NO ATTRIBUTE");
		if (mElement && mElement->Attribute(attribute.c_str()))
			s = mElement->Attribute(attribute.c_str());
		return s;		
	}
	void XMLNode::printSubtree()
	{				
		class Visitor :public tinyxml2::XMLVisitor
		{
			bool VisitEnter(const tinyxml2::XMLElement& e, const tinyxml2::XMLAttribute*) override
			{				
				LOG_DEBUG(e.Name());
				return true;
			}
		};
		Visitor visitor;
		mElement->Accept(&visitor);
		
	}

	bool XMLNode::isEmpty() const
	{
		if (mElement) return false;
		else return true;
	}

	/*void XMLNode::walk(void(*func)(const XMLNode & ))
	{
		XMLNode node(this->getFirstChild());
		while (!node.isEmpty()) {
			(*func)(node);
			node = node.getNextElement();
		}

	}*/


	void XMLNode::walk(const std::function<void(const XMLNode&)> func)
	//void XMLNode::walk(void(*func)(const XMLNode&))
	{
		XMLNode node(this->getFirstChild());
		while (!node.isEmpty()) {
			func(node);
			node = node.getNextElement();
		}
	}

}