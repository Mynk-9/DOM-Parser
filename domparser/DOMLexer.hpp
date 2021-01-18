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

#ifndef DOM_PARSER_DOM_LEXER
#define DOM_PARSER_DOM_LEXER

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <fstream>
#include <iterator>

namespace dom_parser
{
    struct lexer_token_values
    {
    public:
        const static char FILEBEG = 0; // file begin
        const static char FILEEND = 1; // file end

        const static char OPENTAG = 2; // open tag
        const static char CLOSTAG = 3; // close tag
        const static char OPCLTAG = 4; // self closing tag

        const static char ATTRIBUTE = 5; // attribute
        const static char ATTRVALUE = 6; // attribute value
        const static char ATTRNOVAL = 7; // no value attribute
        const static char INNERDATA = 8; // inner data
    };

    struct lexer_token
    {
    public:
        char token;
        std::string value;
    };

    class lexer
    {
    private:
        std::vector<std::shared_ptr<lexer_token>> token_buffer;
        int token_buffer_pos = 0;
        std::ifstream fin;
        std::istreambuf_iterator<char> itr;
        std::istreambuf_iterator<char> eof;

    public:
        lexer(std::filesystem::path path)
        {
            fin.open(path);
            itr = std::istreambuf_iterator<char>(fin);
        }

        void generate_tokens()
        {
        }

        lexer_token *next()
        {
            if (token_buffer_pos == 0)
                generate_tokens();

            return token_buffer[token_buffer_pos].get();
        }
    };

}; // namespace dom_parser

#endif
