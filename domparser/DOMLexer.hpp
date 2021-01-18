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

namespace dom_parser
{
    struct lexer_tokens
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

}; // namespace dom_parser

#endif
