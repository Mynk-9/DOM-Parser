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

#ifndef XML_PARSER_XML_PARSER
#define XML_PARSER_XML_PARSER

#include <string>
#include <vector>
#include <map>
#include <stack>

#include "XMLtree.hpp"

namespace xml_parser
{
    class XMLparser
    {
    private:
        XMLtree tree;

        /**
         * @brief   loads tree from the data
         */
        int _parser(const std::string &data)
        {
            std::stack<XMLnodeUID> element_stack;
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
                    XMLtree _tree(tag_name);
                    // std::cout << "\n\tdebug: "
                    //           << "root tag=" << tag_name << "\n";
                    XMLnodeUID uid = 0; // uid is 0 for root
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
                std::string tag_name;
                std::map<std::string, std::string> attr;
                int res;
                if (*i == '<')
                {
                    // std::cout << "\n\tdebug: tick; current_char=" << *i << "\n";
                    res = _data_scan_tag(i, data.end(), tag_name, attr);
                    // std::cout << "\n\tdebug: res=" << res << " tag_name=" << tag_name << "\n";
                    XMLnodeUID uid;
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

                attributes[attribute] = value;
            }
            return 1;
        }

    public:
        /**
         * @brief Default constructor.
         */
        XMLparser() {}

        /**
         * @brief   Constructs the tree from the provided data.
         * @param   std::string data    the data
         */
        XMLparser(const std::string &data)
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
        inline XMLtree getTree()
        {
            return tree;
        }
    };

} // namespace xml_parser

#endif