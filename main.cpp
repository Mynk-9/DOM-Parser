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

#include <iostream>
#include <string>
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
    

    return 0;
}

// this file is for testing purposes