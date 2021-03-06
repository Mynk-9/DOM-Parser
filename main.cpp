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
#include <vector>

/**
 * Use the DOM_PARSER_DEBUG_MODE to get debug output in console.
 * It would reduce output speed depending on the size of input file.
 * */
#define DOM_PARSER_DEBUG_MODE
#include "./test/test.hpp"
#undef DOM_PARSER_DEBUG_MODE

using namespace std;

int main()
{
    vector<string> files = {"testing.xml", "part.xml", "ebay.xml"};
    string output_file = "./test/output.xml";
    int select_file = 0;

    loadTest.set_file("./test/" + files[select_file]);
    loadTest.set_verbose(true);
    long long ms = loadTest.run(output_file);
    cout << "Completed Load Test on " << files[select_file] << " in " << ms << " milliseconds.\n";

    return 0;
}

// this file is for testing purposes