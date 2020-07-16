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
#include <queue>

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
        int parser(const std::string &data)
        {
            return _parser_prototype(data);
        }

        int _parser_prototype(const std::string &data)
        {
            std::queue<XMLnodeUID> stack;
            stack.push(-1);

            for (auto i = data.begin(); i != data.end(); ++i)
            {
                if (*i == '<' && *(i + 1) == '</')
                {
                }
                else if (*i == '<')
                {
                    std::string tagName = "";
                    std::queue<std::pair<std::string, std::string>> attributes;

                    auto j = i + 1;
                    for (; *j != ' ', *j != '>'; ++j)
                        tagName += *j;

                    if (*j == '>')
                    {
                        i = j;
                        continue;
                    }

                    ++j;
                    for (; *j != '>';)
                    {
                        std::string attribute = "", value = "";
                        for (; *j != '='; ++j)
                            attribute += *j;
                        ++j;
                        if (*j == '\"')
                        {
                            ++j;
                            for (; *j != '\"', *j != '>'; ++j)
                                value += *j;
                            if (*j == '\"')
                                ++j;
                        }
                        else
                        {
                            ++j;
                            for (; *j != ' ', *j != '>'; ++j)
                                value += *j;
                        }

                        attributes.push({attribute, value});
                    }

                    i = j;

                    XMLnodeUID uid = tree.addNode(stack.front(), tagName);
                    stack.push(uid);
                    while (!attributes.empty())
                    {
                        auto k = attributes.front();
                        tree.getNode(uid).setAttribute(k.first, k.second);
                        attributes.pop();
                    }
                }
            }
        }

    public:
        /**
         * @brief Default constructor.
         */
        XMLparser();

        /**
         * @brief   Constructs the tree from the provided data.
         * @param   std::string data    the data
         */
        XMLparser(const std::string &data)
        {
            parser(data);
        }

        /**
         * @brief   Loads the tree from the data.
         * @return  -1  if data is not loaded yet
         */
        inline int loadTree(const std::string &data)
        {
            if (data == "")
                return -1;
            else
                return parser(data);
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