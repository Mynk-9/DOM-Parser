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

#ifndef XML_PARSER_XML_TREE
#define XML_PARSER_XML_TREE

#include <string>
#include <vector>
#include <queue>

#include "XMLnode.hpp"

namespace xml_parser
{
    class XMLtree
    {
    private:
        std::vector<XMLnode> nodes;
        int nodes_counter = 0;

        std::queue<XMLnodeUID> vacantUIDs;
        XMLnode deletedNode = XMLnode("", -1, -1);

        /**
        * @brief   Generates a new XMLnodeUID.
        */
        inline XMLnodeUID generateUID()
        {
            ++nodes_counter;

            if (nodes_counter == 1) // added to fix a possible bug in which multiple root XML
                return 0;           // elements could occur if once root node is deleted.
            if (vacantUIDs.empty())
                return nodes_counter - 1;

            XMLnodeUID uid = vacantUIDs.front();
            vacantUIDs.pop();
            return uid;
        }

        /**
         * @brief   Checks existance of a node with given UID.
         * @param   xml_parser::XMLnodeUID node     The node UID.
         * @return  true or false accordingly
         */
        inline bool checkNodeExistance(XMLnodeUID node)
        {
            return (node < nodes.size() && nodes[node].getUID() != -1);
        }

    public:
        /**
         * @brief   Constructor of empty tree. @a Depriciated @a method - beware
         *          of undefined behaviour when used this contructor without proper
         *          knowledge.
         */
        XMLtree();

        /**
         * @brief   Constructor of the tree with an initial root node.
         * @param   std::string rootName    Name of the root node.
         */
        XMLtree(std::string root)
        {
            XMLnode _root(root, generateUID(), -1);
            nodes.push_back(_root);
        }

        /**
         * @brief   Adds a node within the tree.
         * @param   xml_parser::XMLnodeUID parent   Parent node UID.
         * @param   std::string            tagName  Tag name of the node.
         * @return  XMLnodeID   if node added succefully
         *          -1          if parent does not exist
         */
        XMLnodeUID addNode(XMLnodeUID parent, std::string tagName)
        {
            if (!checkNodeExistance(parent))
                return -1;

            XMLnodeUID UID = generateUID();
            XMLnode node(tagName, UID, parent);

            if (UID < nodes.size())    // If a vacant space if filled then use [] operator
                nodes[UID] = node;     // otherwise push_back to the end of the vector.
            else                       // Condition added to make sure that UID and iterator
                nodes.push_back(node); // position is consistent.

            nodes[parent].addChild(UID);

            return UID;
        }

        /**
         * @brief   Returns a reference to the node with given UID.
         * @param  xml_parser::XMLnodeUID  node    UID of the node.
         */
        inline XMLnode &getNode(XMLnodeUID node)
        {
            return nodes[node];
        }

        /**
         * @brief   Moves a whole subtree from one parent node to another.
         * @param   xml_parser::XMLnodeUID subtree_root     Subtree root node UID.
         * @param   xml_parser::XMLnodeUID new_parent       New parent node of the subtree.
         * @return  true    if moving is successful
         *          false   if moving is unsuccessful due to problem in input.
         */
        bool moveSubtree(XMLnodeUID subtree_root, XMLnodeUID new_parent)
        {
            if (!checkNodeExistance(subtree_root) || !checkNodeExistance(new_parent))
                return false;
            if (subtree_root == 0)
                return false;
            if (subtree_root == new_parent)
                return false;

            std::vector<XMLnodeUID> ancestors = getAncestorList(new_parent);
            for (XMLnodeUID ancestor : ancestors)
                if (ancestor == subtree_root)
                    return false;

            XMLnodeUID old_parent = nodes[subtree_root].getParent();
            nodes[old_parent].removeChild(subtree_root);
            nodes[new_parent].addChild(subtree_root);
            nodes[subtree_root].setParent(new_parent);
            return true;
        }

        /**
         * @brief   Deletes the subtree with the given node as root.
         *          Deletes the single node if no child nodes present.
         * @param   xml_parser::XMLnodeUID subtree_root Subtree root node.
         */
        void deleteSubtree(XMLnodeUID subtree_root)
        {
            if (!checkNodeExistance(subtree_root))
                return;

            XMLnodeUID current_node; // = subtree_root;
            std::queue<XMLnodeUID> node_queue;
            node_queue.push(subtree_root);

            while (!node_queue.empty())
            {
                current_node = node_queue.front();

                for (XMLnodeUID node : nodes[current_node].getChildrenUID())
                    node_queue.push(node);
                node_queue.pop();

                vacantUIDs.push(current_node);
                nodes[current_node] = deletedNode;
                nodes_counter--;
            }
        }

        /**
         * @brief   Returns std::vector of ancestors of the given node.
         * @param   xml_parser::XMLnodeUID node     The node UID.
         */
        std::vector<XMLnodeUID> getAncestorList(XMLnodeUID node)
        {
            std::vector<XMLnodeUID> ancestorList;
            XMLnodeUID node_uid = node;
            while (node_uid != 0)
            {
                node_uid = nodes[node_uid].getParent();
                ancestorList.push_back(node_uid);
            }
            return ancestorList;
        }
    };

} // namespace xml_parser

#endif