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
#include <queue>
#include <memory>
#include <filesystem>
#include <fstream>

namespace dom_parser
{

    /**
     *  @brief  Stores definitions of the lexer tokens as const static objects.
     * */
    struct lexer_token_values
    {
    public:
        // file begin
        const static char T_FILEBEG = '0';
        // file end
        const static char T_FILEEND = '1';

        // open tag
        const static char T_OPENTAG = '<';
        // close tag
        const static char T_CLOSTAG = '>';
        // a backward slash "/"
        const static char T_BKSLASH = '/';

        // an identifier
        const static char T_IDNTIFR = 'I';
        // an equal "=" sign
        const static char T_EQLSIGN = '=';
        // a double quote `"`
        const static char T_DBLQUOT = '\"';
        // a single quote `'`
        const static char T_SINQUOT = '\'';
    };

    /**
     *  @brief Class representing a token.
     * */
    class lexer_token
    {
    public:
        char token;
        std::string value;

        /**
         *  @brief Constructor
         *  @param  _token  token taken from lexer_token_values
         *  @param  _value  value associated with token
         * */
        lexer_token(char _token, std::string &&_value)
        {
            token = _token;
            value.swap(_value);
        }
    };

    /**
     *  @brief  Lexer class.
     * */
    class lexer
    {
    private:
        std::queue<std::shared_ptr<lexer_token>> token_buffer;
        std::ifstream fin;

        /**
         *  @brief  Adds token to token_buffer
         *  @param  _token  token taken from lexer_token_values
         *  @param  _value  value associated with token
         * */
        void buffer_add_token(char _token, std::string &&_value)
        {
            token_buffer.push(
                std::move(
                    std::shared_ptr<lexer_token>(
                        new lexer_token(
                            _token, std::move(_value)))));
        }

        /**
         *  @brief  Checks if provided char is one of the chars which are 
         *          represented by tokens.
         * */
        inline bool check_special_char(char c)
        {
            return (c == '<' || c == '>' || c == '/' ||
                    c == '=' || c == '\"' || c == '\'');
        }

        /**
         *  @brief  Generates tokens for the next input from file
         * */
        void generate_tokens()
        {
            std::string buff;
            if (fin >> buff) // if input successful
            {
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
                            ++i;
                            if (check_special_char(*i))
                            {
                                --i; // --i because for-loop would ++i anyway
                                break;
                            }
                        }
                        break;
                    }

                    buffer_add_token(token_name, std::move(token_value));

                    if (i == buff.end()) // if buffer end has already reached then terminate the
                        break;           // loop otherwise i would be incremented further end()
                }
            }
            else // file finished
            {
                buffer_add_token(lexer_token_values::T_FILEEND, std::move(""));
            }
        }

    public:
        // Deleted default constructor
        lexer() = delete;

        /**
         *  @brief  Constructor
         *  @param  path    path of the file which is to be scanned.
         * */
        lexer(std::filesystem::path path)
        {
            fin.open(path);
            buffer_add_token(lexer_token_values::T_FILEBEG, std::move(""));
        }

        /**
         *  @brief  Returns the pointer to the next token from the token buffer.
         * */
        lexer_token *next()
        {
            token_buffer.pop();
            if (token_buffer.size() == 0)
                generate_tokens();

#ifdef DOM_PARSER_DEBUG_MODE
            std::cout << "\n\tdebug: LEXER: token: "
                      << token_buffer.front().get()->token << " value: "
                      << token_buffer.front().get()->value << "\n";
#endif
            return token_buffer.front().get();
        }
    };

}; // namespace dom_parser

#endif
