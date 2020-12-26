//    Copyright 2020 Mayank Mathur (mynk-9 at Github)

//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at

//        http://www.apache.org/licenses/LICENSE-2.0

//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#ifndef DOM_PARSER_DOM_PARSER
#define DOM_PARSER_DOM_PARSER

#include <string>
#include <vector>
#include <map>
#include <stack>

#include "DOMtree.hpp"

namespace dom_parser
{
    class DOMparser
    {
    private:
        DOMtree tree;

        /**
         * @brief   loads tree from the data
         */
        int _parser(const std::string &data)
        {
            std::stack<DOMnodeUID> element_stack;
            auto i = data.begin();

            // add the root
            {
                std::string tag_name;
                std::map<std::string, std::string> attr;
                int res;
                // ignore white-space
                // for (; i != data.end(); ++i)
                //     if (*i != ' ')
                //         break;
                // scan the tag
                if (*i == '<')
                {
                    res = _data_scan_tag(i, data.end(), tag_name, attr);
                    // return fail message is error
                    if (res != 1)
                        return (i - data.begin());

                    // set the root
                    DOMtree _tree(tag_name);
                    // std::cout << "\n\tdebug: "
                    //           << "root tag=" << tag_name << "\n";
                    DOMnodeUID uid = 0; // uid is 0 for root
                    tree = _tree;
                    element_stack.push(uid);
                    for (auto const &attribute : attr)
                        tree.getNode(uid).setAttribute(attribute.first, attribute.second);
                }
                else
                    return (i - data.begin());
            }

            // scan the rest
            for (; i != data.end(); ++i)
            {
                std::string tag_name, inner_data;
                std::map<std::string, std::string> attr;
                int res;
                if (*i == '<')
                {
                    // std::cout << "\n\tdebug: tick; current_char=" << *i << "\n";
                    res = _data_scan_tag(i, data.end(), tag_name, attr);
                    // std::cout << "\n\tdebug: res=" << res << " tag_name=" << tag_name << "\n";
                    DOMnodeUID uid;
                    switch (res)
                    {
                    case 0: // fail
                        return (i - data.begin());
                    case -1: // closing tag
                        element_stack.pop();
                        break;
                    case 1: // success
                        uid = tree.addNode(element_stack.top(), tag_name);
                        element_stack.push(uid);
                        for (auto const &attribute : attr)
                            tree.getNode(uid).setAttribute(attribute.first, attribute.second);
                        break;
                    case -2: // self closing tag
                        uid = tree.addNode(element_stack.top(), tag_name);
                        for (auto const &attribute : attr)
                            tree.getNode(uid).setAttribute(attribute.first, attribute.second);
                        break;
                    }
                }
                else if (*i == '>')
                    continue;
                else
                {
                    while ((*i) != '<' && i != data.end())
                    {
                        inner_data += *i;
                        ++i;
                    }
                    --i;

                    if (inner_data != " " && inner_data != "\n" && inner_data != "\r\n")
                    {
                        tree.addInnerDataNode(element_stack.top(), inner_data);
                        // std::cout << "\n\tdebug: inner_data=\"" << inner_data << "\"\n";
                    }
                }

                if (element_stack.empty())
                    break;
            }

            return 0;
        }

        /**
         * @brief   scans tag data
         * @return  0   fail
         *          1   success
         *          -1  closing tag
         *          -2  self closing tag
         * */
        int _data_scan_tag(std::string::const_iterator &i, const std::string::const_iterator &end,
                           std::string &tag, std::map<std::string, std::string> &attributes)
        {
            if (*i != '<')
                return 0;
            ++i;

            // scan tag name
            while (i != end && (*i) != ' ' && (*i) != '>')
            {
                tag += *i;
                ++i;
            }
            // error if data ends abruptly
            if (i == end)
                return 0;
            // check if the tag was a closing tag
            if (tag[0] == '/')
                return -1;

            // scan attributes
            while (i != end && (*i) != '>')
            {
                // ignore white space
                while ((*i) == ' ')
                    ++i;
                // check if tag closed
                if (*i == '>')
                    break;
                // check if self closing tag
                if (*i == '/')
                {
                    ++i;
                    if (*i == '>')
                        return -2;
                }

                std::string attribute;
                std::string value;

                // scan attribute name
                while ((*i) != '=' && (*i) != ' ' && (*i) != '>')
                {
                    attribute += *i;
                    ++i;
                }
                if ((*i) == ' ')
                {
                    attributes[attribute] = "";
                    continue;
                }

                // check if value wrapped in quotes
                ++i;
                char q = ' ';
                if ((*i) == '\'' || (*i) == '\"')
                {
                    q = *i;
                    ++i;
                }

                // scan attribute value
                while (i != end && (*i) != q && (*i) != '>')
                {
                    value += *i;
                    ++i;
                }
                ++i;

                attributes[attribute] = value;
            }
            return 1;
        }

        /**
         * @brief   Helper function, generates output for the tree.
         * @param   _node       Initial node
         * @param   indent      Indentation string
         * @param   indentation Current indentation at the node
         * @param   _newline    Char to be used as newline char
         * */
        std::string _process_output_for_node(DOMnodeUID _node, const std::string &indent,
                                             std::string indentation, std::string _newline)
        {
            auto node = tree.getNode(_node);
            std::string s;

            // set indentation
            s += indentation;

            // check if node is innerData node
            if (node.isInnerDataNode())
            {
                s += node.getInnerData() + _newline;
                return s;
            }

            // opening and closing tags
            s += "<" + node.getTagName();
            for (auto i : node.getAllAttributes())
            {
                s += " " + i.first;
                if (!i.second.empty())
                    s += "=\"" + i.second + "\"";
            }
            if (node.getChildrenUID().empty()) // if no child nodes
                s += " />" + _newline;         // closing tags
            else
            {
                // close opening tag
                s += ">" + _newline;
                // add children nodes
                for (auto i : node.getChildrenUID())
                    s += _process_output_for_node(i, indent, indentation + indent, _newline);
                // closing tag
                s += indentation + "</" + node.getTagName() + ">" + _newline;
            }

            return s;
        }

    public:
        /**
         * @brief Default constructor.
         */
        DOMparser() {}

        /**
         * @brief   Constructs the tree from the provided data.
         * @param   data    the data
         */
        DOMparser(const std::string &data)
        {
            _parser(data);
        }

        /**
         * @brief   Loads the tree from the data.
         * @return  -1  if data is empty
         *          n   number n > 0 denoting character number if there is error
         *              parsing at that line
         *          0   if parsed successfully
         */
        inline int loadTree(const std::string &data)
        {
            if (data == "")
                return -1;
            else
                return _parser(data);
        }

        /**
         * @brief   Returns the loaded tree else the tree is blank
         *          with only one node - root node with blank tag name.
         */
        inline DOMtree getTree()
        {
            return tree;
        }

        /**
         * @brief   Returns string of formatted document.
         * @param   minified    If output is required to be in minified form.
         * @param   indent      string which denotes indentation, default is 4 spaces.
         * @param   indentation initial indentation of the output, that is the 
         *                      indentation applied on root node, default is empty.
         * */
        std::string getOutput(bool minified = false, std::string indent = "    ", std::string indentation = "")
        {
            // return _process_output_for_node(0, indent, indentation, (minified ? "" : "\n"));
            if (minified)
                return _process_output_for_node(0, "", "", "");
            else
                return _process_output_for_node(0, indent, indentation, "\n");
        }

        /**
         * @brief   =operator overload
         * */
        DOMparser &operator=(const DOMparser &parser)
        {
            this->tree = parser.tree;
            
            return *this;
        }
    };

} // namespace dom_parser

#endif
