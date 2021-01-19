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

namespace dom_parser
{
    struct lexer_token_values
    {
    public:
        const static char T_FILEBEG = '0'; // file begin
        const static char T_FILEEND = '1'; // file end

        const static char T_OPENTAG = '<'; // open tag
        const static char T_CLOSTAG = '>'; // close tag
        const static char T_BKSLASH = '/'; // a backward slash "/"

        const static char T_IDNTIFR = 'I';  // an identifier
        const static char T_EQLSIGN = '=';  // an equal "=" sign
        const static char T_DBLQUOT = '\"'; // a double quote `"`
        const static char T_SINQUOT = '\''; // a single quote `'`
    };

    class lexer_token
    {
    public:
        char token;
        std::string value;
        lexer_token(char _token, std::string &&_value)
        {
            token = _token;
            value.swap(_value);
        }
    };

    class lexer
    {
    private:
        std::vector<std::shared_ptr<lexer_token>> token_buffer;
        int token_buffer_pos = 0;
        std::ifstream fin;

        inline bool check_special_char(char c)
        {
            return (c == '<' || c == '>' || c == '/' ||
                    c == '=' || c == '\"' || c == '\'');
        }

        void generate_tokens()
        {
            std::string buff;
            while (fin >> buff)
            {
                std::cout << "\n\t\t buffer=" << buff << "\n";
                for (auto i = buff.begin(); i != buff.end(); ++i)
                {
                    std::string token_value;
                    char token_name;
                    switch (*i)
                    {
                    case '<':
                        token_name = lexer_token_values::T_OPENTAG;
                        break;
                    case '>':
                        token_name = lexer_token_values::T_CLOSTAG;
                        break;
                    case '/':
                        token_name = lexer_token_values::T_BKSLASH;
                        break;
                    case '=':
                        token_name = lexer_token_values::T_EQLSIGN;
                        break;
                    case '\"':
                        token_name = lexer_token_values::T_DBLQUOT;
                        break;
                    case '\'':
                        token_name = lexer_token_values::T_SINQUOT;
                        break;
                    default:
                        token_name = lexer_token_values::T_IDNTIFR;
                        token_value = "";
                        while (i != buff.end())
                        {
                            token_value += *i;
                            std::cout << "\n\tdebug: token_value: " << token_value << "\n";
                            ++i;
                            if (check_special_char(*i))
                            {
                                --i;
                                break;
                            }
                        }
                        break;
                    }

                    token_buffer.push_back(
                        std::shared_ptr<lexer_token>(
                            new lexer_token(token_name, std::move(token_value))));

                    if (i == buff.end())
                        break;
                }
            }

            int _cnt = 1;
            for (auto i : token_buffer)
            {
                auto token = (*i.get()).token;
                auto value = (*i.get()).value;

                std::cout << "\t" << _cnt++ << " TOKEN: " << token << " "
                          << "VALUE: " << value << "\n";
            }
        }

    public:
        lexer() = delete;

        lexer(std::filesystem::path path)
        {
            fin.open(path);
            generate_tokens();
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
