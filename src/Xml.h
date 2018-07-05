#pragma once
#include "tinyxml2.h"

namespace Nova {
	/*Simple wrapper for tinyXML2*/
	
	class XMLNode
	{		
	public:
		XMLNode() :mElement(nullptr) {}
		XMLNode(const tinyxml2::XMLElement * element) :mElement(element ? element : nullptr){};

		XMLNode getFirstChild() const;
		XMLNode getNextElement() const;
		XMLNode getChildElement(const std::string &elementName) const;
		std::string getText() const;
		std::string getName() const;
		std::string getAttributeValue(const std::string attribute) const;
		void printSubtree();
		bool isEmpty() const;

		/** 
		walk the children of this node(not recursively) and call func
		for each one of them
		*/
		void walk(const std::function<void (const XMLNode&)> func);
		//void walk(void (*func)(const XMLNode&));
		/*template <typename Func>
		void walk(void func(const XMLNode&))
		{
			XMLNode node(this->getFirstChild());
			while (!node.isEmpty()) {
				func(node);
				node = node.getNextElement();
			}
		}*/
	private:
		const tinyxml2::XMLElement* mElement;
	};

	class XMLFile
	{
	public:
		XMLFile() = delete;
		XMLFile(const std::string &filename);
		XMLNode getRootElement() { return XMLNode(mDocument.RootElement()); };			
		//void print();
	private:
		tinyxml2::XMLDocument mDocument;
	};

	/*class XML
	{
		//friend class XMLNode;
	public:
		static XMLNode loadFile(const std::string &file);

	};*/
	
}