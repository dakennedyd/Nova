
// The MIT License (MIT)

// Copyright (c) 2018 David Kennedy

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

#include "Xml.h"
#include "logger/Logger.h"
#include <functional>
#include <string>

namespace Nova
{
XMLFile::XMLFile(const std::string &filename)
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
XMLNode XMLNode::getChildElement(const std::string &elementName) const
{
    if (mElement)
        return XMLNode(mElement->FirstChildElement(elementName.c_str()));
    else
        return XMLNode();
}
std::string XMLNode::getText() const
{
    std::string s("NO TEXT");
    if (mElement && mElement->GetText()) s = mElement->GetText();
    return s;
}
std::string XMLNode::getName() const
{
    std::string s("NO NAME");
    if (mElement && mElement->Name()) s = mElement->Name();
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
    class Visitor : public tinyxml2::XMLVisitor
    {
        bool VisitEnter(const tinyxml2::XMLElement &e, const tinyxml2::XMLAttribute *) override
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
    if (mElement)
        return false;
    else
        return true;
}

/*void XMLNode::walk(void(*func)(const XMLNode & ))
{
        XMLNode node(this->getFirstChild());
        while (!node.isEmpty()) {
                (*func)(node);
                node = node.getNextElement();
        }

}*/

void XMLNode::walk(const std::function<void(const XMLNode &)> func)
// void XMLNode::walk(void(*func)(const XMLNode&))
{
    XMLNode node(this->getFirstChild());
    while (!node.isEmpty())
    {
        func(node);
        node = node.getNextElement();
    }
}

} // namespace Nova