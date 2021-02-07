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

#include <filesystem>
#include <fstream>

#ifdef DOM_PARSER_DEBUG_MODE
#include <iostream>
#endif

#include "DOMLexer.hpp"
#include "DOMtree.hpp"

namespace dom_parser
{
    class DOMparser
    {
    private:
        DOMtree tree;

        /**
         * @brief   deprecated, loads tree from the data
         */
        [[deprecated]] int _parser(const std::string &data)
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
         * @brief   loads tree from the data
         */
        int _parser(std::filesystem::path file)
        {
            lexer _lexer(file);
            std::stack<DOMnodeUID> element_stack;
            auto _T = _lexer.next();

            if (_T->token != lexer_token_values::T_OPENTAG)
                return -2; // root node required, error

            // scan root node
            {
                DOMnodeUID uid = 0; // for root
                std::string tag_name;
                std::map<std::string, std::string> attributes;

                int res = _data_scan_tag(_lexer, tag_name, attributes);
                if (res != 1)
                    return -2;

                DOMtree _tree(tag_name);
                tree = _tree;
                element_stack.push(uid);
                tree.getNode(uid).setAttributes(std::move(attributes));
            }
            _T = _lexer.next();

            while (_T->token != lexer_token_values::T_FILEEND)
            {
                std::string tag_name;
                std::map<std::string, std::string> attributes;
                DOMnodeUID uid;

                if (_T->token == lexer_token_values::T_OPENTAG) // read tag
                {
                    int res = _data_scan_tag(_lexer, tag_name, attributes);

#ifdef DOM_PARSER_DEBUG_MODE
                    std::cout << "\n\tdebug: PARSER: TAG: " << tag_name
                              << " ATTRIBUTES:";
                    for (const auto &attr : attributes)
                    {
                        std::cout << "\n\t\t" << attr.first
                                  << "=\"" << attr.second << "\"\n";
                    }
#endif

                    switch (res)
                    {
                    case 0: // fail
#ifdef DOM_PARSER_DEBUG_MODE
                        std::cout << "\n\tdebug: PARSER: fail"
                                  << "\n";
#endif
                        return -2;
                    case -1: // closing tag
#ifdef DOM_PARSER_DEBUG_MODE
                        std::cout << "\n\tdebug: PARSER: closing tag"
                                  << "\n";
#endif
                        element_stack.pop();
                        break;
                    case 1: // success
#ifdef DOM_PARSER_DEBUG_MODE
                        std::cout << "\n\tdebug: PARSER: success"
                                  << "\n";
#endif
                        uid = tree.addNode(element_stack.top(), tag_name);
                        element_stack.push(uid);
                        tree.getNode(uid).setAttributes(std::move(attributes));
                        break;
                    case -2: // self closing tag
#ifdef DOM_PARSER_DEBUG_MODE
                        std::cout << "\n\tdebug: PARSER: self closing tag"
                                  << "\n";
#endif
                        uid = tree.addNode(element_stack.top(), tag_name);
                        tree.getNode(uid).setAttributes(std::move(attributes));
                        break;
                    }

                    _T = _lexer.next();
                }
                else // read innerData
                {
#ifdef DOM_PARSER_DEBUG_MODE
                    std::cout << "\n\tdebug: PARSER: innerData"
                              << "\n";
#endif
                    std::string innerData = "";
                    while (_T->token != lexer_token_values::T_OPENTAG)
                    {
                        innerData += _T->value + " ";
                        _T = _lexer.next();
                    }
                    innerData.erase(innerData.length() - 1, 1); // trim the last space
                    tree.addInnerDataNode(element_stack.top(), innerData);
                }
            }

            return 0;
        }

        /**
         * @brief   deprecated, scans tag data
         * @return  0   fail
         *          1   success
         *          -1  closing tag
         *          -2  self closing tag
         * */
        [[deprecated]] int _data_scan_tag(std::string::const_iterator &i, const std::string::const_iterator &end,
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
         * @brief   scans tag data
         * @return  0   fail
         *          1   success
         *          -1  closing tag
         *          -2  self closing tag
         * */
        int _data_scan_tag(lexer &_lexer,
                           std::string &tag_name,
                           std::map<std::string, std::string> &attributes)
        {
            auto _T = _lexer.next();
            // everytime we use lexer::next() we will check for file-end token
            // if we get abrupt file end, error value will be returned

            switch (_T->token)
            {
            case lexer_token_values::T_FILEEND: // found file end
                return 0;

            case lexer_token_values::T_BKSLASH: // closing tag
                _T = _lexer.next();
                if (_T->token != lexer_token_values::T_IDNTIFR)
                    return 0;
                _T = _lexer.next();
                if (_T->token != lexer_token_values::T_CLOSTAG)
                    return 0;
                return -1;

            case lexer_token_values::T_IDNTIFR: // found identifier

                tag_name = _T->value; // set tagname

                _T = _lexer.next();
                if (_T->token == lexer_token_values::T_FILEEND)
                    return 0;

                // scan attributes till closing tag
                while (_T->token != lexer_token_values::T_CLOSTAG &&
                       _T->token != lexer_token_values::T_BKSLASH)
                {
                    // error: not identifier
                    if (_T->token != lexer_token_values::T_IDNTIFR)
                        return 0;

                    std::string attribute, value;
                    // get attribute name
                    attribute = _T->value;

                    // check next token for equal sign
                    _T = _lexer.next();
                    // either token should be equal sign or an identifier or > or /
                    // > for tag closing, and / for /> type tag closing
                    // otherwise error
                    if (_T->token == lexer_token_values::T_IDNTIFR ||
                        _T->token == lexer_token_values::T_BKSLASH ||
                        _T->token == lexer_token_values::T_CLOSTAG) // no value attribute
                    {
                        attributes[attribute] = "";
                        continue;
                    }
                    else if (_T->token != lexer_token_values::T_EQLSIGN) // error
                        return 0;

                    // scan attribute value
                    // next token is either double/single quote or an identifier
                    _T = _lexer.next();
                    if (_T->token == lexer_token_values::T_IDNTIFR) // identifier
                    {
                        attributes[attribute] = _T->value;
                    }
                    else if (_T->token == lexer_token_values::T_DBLQUOT ||
                             _T->token == lexer_token_values::T_SINQUOT) // quote
                    {
                        auto T_QUOTE = _T->token;
                        _T = _lexer.next();
                        // scan till we encounter that quote or file-end
                        while (_T->token != T_QUOTE)
                        {
                            if (_T->token == lexer_token_values::T_FILEEND)
                                return 0;
                            value += _T->value + " ";

                            _T = _lexer.next();
                        }
                        value.erase(value.length() - 1, 1); // trim the last space
                        attributes[attribute] = value;
                    }
                    else
                        return 0;

                    _T = _lexer.next(); // next token
                }

                // check if element opening tag or self closing tag
                if (_T->token == lexer_token_values::T_BKSLASH)
                {
                    _T = _lexer.next();
                    if (_T->token == lexer_token_values::T_CLOSTAG)
                        return -2; // self closing
                    else
                        return 0; // error
                }
                else if (_T->token == lexer_token_values::T_CLOSTAG)
                    return 1; // success
                else
                    return 0; // error
            }

            return 0;
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
         * @brief   Deprecated. Constructs the tree from the provided data.
         * @param   data    the data
         */
        [[deprecated]] DOMparser(const std::string &data)
        {
            _parser(data);
        }

        /**
         * @brief   Deprecated. Loads the tree from the data.
         * @param   data    data provided for the tree to be loaded from
         * @return  -1  if data is empty
         *          n   number n > 0 denoting character number if there is error
         *              parsing at that line
         *          0   if parsed successfully
         */
        [[deprecated]] inline int loadTree(const std::string &data)
        {
            if (data == "")
                return -1;
            else
                return _parser(data);
        }

        /**
         * @brief   Loads the tree from the data.
         * @param   data    data provided for the tree to be loaded from
         * @return  -2  if file does not exist
         *          -1  if data is empty
         *          n   number n > 0 denoting character number if there is error
         *              parsing at that line
         *          0   if parsed successfully
         */
        [[deprecated]] inline int loadTree_primitive(std::filesystem::path path)
        {
            std::ifstream fin(path);
            if (!fin.is_open())
                return -2;

            std::string buff;
            std::string data = "";
            while (fin >> buff)
                data += buff + " ";
            fin.close();

            if (data == "")
                return -1;
            else
                return _parser(std::move(data));
        }

        /**
         * @brief   Loads the tree from the data utilisizing a tokenizer/lexer.
         * @param   data    data provided for the tree to be loaded from
         * @return  -2  error
         *          0   if parsed successfully
         */
        inline int loadTree(std::filesystem::path path)
        {
            return _parser(path);
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
