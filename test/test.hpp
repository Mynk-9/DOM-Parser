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
#include <fstream>
#include <chrono>

#include <filesystem>

#include "./../domparser/DOMparser.hpp"

using namespace std;

struct loadTest
{
private:
    filesystem::path file;
    ofstream fout;
    dom_parser::DOMparser parser;
    bool verbose = true;
    bool use_primitive = false;

    inline void debug_print(string s)
    {
        if (verbose)
            cout << "\n\tloadTest: " << s << "\n";
    }

public:
    void set_file(string path)
    {
        try
        {
            file = filesystem::path(path);
        }
        catch (...)
        {
            debug_print("@set_file: problem with path");
        }
    }
    inline void set_verbose(bool _verbose)
    {
        verbose = _verbose;
    }
    inline void set_primitive(bool _primitive)
    {
        use_primitive = _primitive;
    }
    long long run(const string output_file)
    {
        // run test
        debug_print("Starting...");

        auto timer_start = chrono::steady_clock::now();
        int e;
        if (!use_primitive)
            e = parser.loadTree(file);
        else
            e = parser.loadTree_primitive(file);
        auto timer_stop = chrono::steady_clock::now();

        debug_print("PARSER RETURN VALUE: " + e);
        if (e != 0)
        {
            debug_print("Error: At line number: " + e);
            return -1;
        }
        else
            debug_print("Loading done.");
        long long total_time = chrono::duration_cast<chrono::milliseconds>(timer_stop - timer_start).count();

        // print the output
        debug_print("Writing output...");
        fout.open(output_file);
        fout << parser.getOutput();
        fout.close();
        debug_print("Output is present in: " + output_file);

        return total_time;
    }
} loadTest;

struct oldTests
{
    void run()
    {
        dom_parser::DOMtree tree("root");
        dom_parser::DOMnodeUID uid;

        uid = tree.addNode(0, "child1");
        uid = tree.addNode(uid, "child1child1");
        tree.getNode(uid).setAttribute("attr1", "val1");
        uid = tree.getNode(uid).getParent();
        uid = tree.getNode(uid).getParent(); // now at root node
        uid = tree.addNode(uid, "child2");
        tree.getNode(uid).setAttribute("abc", "xyz");
        cout << tree.getNode(uid).getAttribute("abc") << "\n";

        cout << "\nParser working...\n";
        // dom_parser::DOMparser parser;
        ifstream fin("sheet1.xml");
        ofstream fout("output.xml");
        string data = "", _data;
        string _tag;
        list<dom_parser::DOMnodeUID> _children;
        dom_parser::DOMparser parser;
        while (fin >> _data)
            data += _data + " ";
        // cout << data << "\n";
        parser.loadTree(data);
        fout << parser.getOutput(true) << flush;
        cout << "done.\n";

        fin.close();
        fout.close();
    }
} oldTests;