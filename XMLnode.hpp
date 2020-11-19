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

#ifndef XML_PARSER_XML_NODE
#define XML_PARSER_XML_NODE

#include <list>
#include <map>
#include <string>

#include "XMLnodeUID.hpp"

namespace xml_parser
{
    /// @brief   Node in the XML tree.
    class XMLnode
    {
    private:
        XMLnodeUID uid;
        XMLnodeUID parent;
        std::list<XMLnodeUID> children;
        std::map<std::string, std::string> tagAttributes;
        std::string tagName;

        bool innerDataNode = false;
        std::string innerData;

    public:
        /**
         * @brief   Constructor for XMLnode
         * @param   std::string             tagName     Name of the tag of the node.
         * @param   xml_parser::XMLnodeUID  uid         UID of this node.
         */
        XMLnode(std::string tagName, XMLnodeUID uid, XMLnodeUID parent)
            : tagName(tagName), uid(uid), parent(parent){};

        /**
         * @brief   Constructor for XMLnode specially for storing inner-data
         * @param   XMLnodeUID  uid         UID of this node
         * @param   XMLnodeUID  parent      UID of the parent
         * @param   std::string innerData   inner text data stored by the node
         * */
        XMLnode(XMLnodeUID uid, XMLnodeUID parent, std::string innerData)
            : uid(uid), parent(parent), innerData(innerData), innerDataNode(true){};

        /**
         * @brief   Returns the tagName of the node.
         */
        inline std::string getTagName()
        {
            return tagName;
        }

        /**
         * @brief   Returns the UID of the node.
         */
        inline XMLnodeUID getUID()
        {
            return uid;
        }

        /**
         * @brief   Sets a new value to existing attribute or
         *          adds new attribute with the given value.
         * @param   std::string attribute   Name of the attribute
         * @param   std::string value       Data of the attribute
         */
        inline void setAttribute(std::string attribute, std::string value)
        {
            if (innerDataNode)
                return;
            tagAttributes[attribute] = value;
        }

        /**
         * @brief   Gets the value of the said attribute. Returns
         *          empty string if the attribute does not exist.
         * @param   std::string attribute   Name of the attribute
         */
        inline std::string getAttribute(std::string attribute)
        {
            return tagAttributes.find(attribute)->second;
        }

        /**
         * @brief   Gets the ordered map of all the attributes
         *          with their values.
         * */
        inline std::map<std::string, std::string> getAllAttributes()
        {
            return tagAttributes;
        }

        /**
         * @brief   Returns the list of children of the node.
         */
        inline std::list<XMLnodeUID> getChildrenUID()
        {
            return children;
        }

        /**
         * @brief   Adds a new child to the node.
         * @param   xml_parser::XMLnodeUID child    Child node.
         */
        inline void addChild(XMLnodeUID child)
        {
            if (innerDataNode)
                return;
            children.push_back(child);
        }

        /**
         * @brief   Removes the child node with the given UID.
         * @param   std::XMLnodeUID uid     UID of the child node to remove.
         */
        void removeChild(XMLnodeUID uid)
        {
            if (innerDataNode)
                return;
            children.remove(uid);
        }

        /**
         * @brief   Returns the parent node UID.
         */
        inline XMLnodeUID getParent()
        {
            return parent;
        }

        /**
         * @brief   Sets a new parent node.
         * @param   xml_parser::XMLnodeUID new_parent_UID    UID of the new parent node.
         */
        inline void setParent(XMLnodeUID new_parent_UID)
        {
            parent = new_parent_UID;
        }

        /**
         * @brief   Checks if node is inner-data node
         * */
        inline bool isInnerDataNode()
        {
            return innerDataNode;
        }

        /**
         * @brief   Returns inner-data if the node is stores inner data.
         *          Returns empty string if node does not store inner-data.
         * */
        inline std::string getInnerData()
        {
            return innerData;
        }

        /**
         * @brief   Operator overload for =operator
         */
        void operator=(const XMLnode &node)
        {
            uid = node.uid;
            parent = node.parent;
            children = node.children;
            tagAttributes = node.tagAttributes;
            tagName = node.tagName;

            innerDataNode = node.innerDataNode;
            innerData = node.innerData;
        }
    };

}; // namespace xml_parser

#endif