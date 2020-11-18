//    Copyright 2020 Mayank Mathur (Mynk-9 at Github)

//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at

//        http://www.apache.org/licenses/LICENSE-2.0

//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

/*
THIS FILE IS FOR TESTING PURPOSES ONLY,
AND DOES NOT CONTRIBUTE TO THE LIBRARY.
THE CODE HERE IS NOT DOCUMENTED.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stack>

#include "XMLparser.hpp"

using namespace std;

int main()
{
    xml_parser::XMLtree tree("root");
    xml_parser::XMLnodeUID uid;

    uid = tree.addNode(0, "child1");
    uid = tree.addNode(uid, "child1child1");
    tree.getNode(uid).setAttribute("attr1", "val1");
    uid = tree.getNode(uid).getParent();
    uid = tree.getNode(uid).getParent(); // now at root node
    uid = tree.addNode(uid, "child2");
    tree.getNode(uid).setAttribute("abc", "xyz");
    cout << tree.getNode(uid).getAttribute("abc") << "\n";

    cout << "\nParser:\n";
    // xml_parser::XMLparser parser;
    ifstream fin("testing.xml");
    string data = "", _data;
    string _tag;
    list<xml_parser::XMLnodeUID> _children;
    xml_parser::XMLparser parser;
    while (fin >> _data)
        data += _data + " ";
    cout << data << "\n";
    parser.loadTree(data);

    uid = 0; // root uid
    _tag = parser.getTree().getNode(uid).getTagName();
    cout << _tag << "\n";
    _children = parser.getTree().getNode(uid).getChildrenUID();
    while (!_children.empty()) // basic dfs
    {
        auto x = _children.front();
        cout << parser.getTree().getNode(x).getTagName() << "\n";
        cout << "\n\tAttributes:\n";
        for (auto y : parser.getTree().getNode(x).getAllAttributes())
            cout << "\t" << y.first << "=" << y.second << '\n';
        _children.pop_front();
        auto chldrn = parser.getTree().getNode(x).getChildrenUID();
        for (auto y = chldrn.rbegin(); y != chldrn.rend(); ++y)
            _children.push_front(*y);
    }

    return 0;
}

// this file is for testing purposes