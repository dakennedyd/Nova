
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

#pragma once
#include <functional>
#include <tinyxml2/tinyxml2.h>

namespace Nova
{
/*Simple wrapper for tinyXML2*/

class XMLNode
{
  public:
    XMLNode() : mElement(nullptr) {}
    XMLNode(const tinyxml2::XMLElement *element) : mElement(element ? element : nullptr){};

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
    void walk(const std::function<void(const XMLNode &)> func);
    // void walk(void (*func)(const XMLNode&));
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
    const tinyxml2::XMLElement *mElement;
};

class XMLFile
{
  public:
    XMLFile() = delete;
    XMLFile(const std::string &filename);
    XMLNode getRootElement() { return XMLNode(mDocument.RootElement()); };
    // void print();
  private:
    tinyxml2::XMLDocument mDocument;
};

/*class XML
{
        //friend class XMLNode;
public:
        static XMLNode loadFile(const std::string &file);

};*/

} // namespace Nova