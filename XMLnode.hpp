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
    // @brief   Node in the XML tree.
    class XMLnode
    {
    private:
        XMLnodeUID uid;
        XMLnodeUID parent;
        std::list<XMLnodeUID> children;
        std::string innerData;
        std::map<std::string, std::string> tagAttributes;
        std::string tagName;

    public:
        // @brief   Constructor for XMLnode
        // @param   std::string             tagName     Name of the tag of the node.
        // @param   xml_parser::XMLnodeUID  uid         UID of this node.
        XMLnode(std::string tagName, XMLnodeUID uid, XMLnodeUID parent)
            : tagName(tagName), uid(uid), parent(parent){};

        // @brief   Returns the tagName of the node.
        inline std::string getTagName()
        {
            return tagName;
        }

        // @brief   Returns the UID of the node.
        inline XMLnodeUID getUID()
        {
            return uid;
        }

        // @brief   Sets the inner data - the data between opening and
        //          closing tags of node.
        // @param   std::string data    The inner data.
        inline void setInnerData(std::string data)
        {
            innerData = data;
        }

        // @brief   Sets a new value to existing attribute or
        //          adds new attribute with the given value.
        // @param   std::string attribute   Name of the attribute
        // @param   std::string value       Data of the attribute
        void setAttribute(std::string attribute, std::string value)
        {
            tagAttributes[attribute] = value;
        }

        // @brief   Gets the value of the said attribute. Returns
        //          empty string if the attribute does not exist.
        // @param   std::string attribute   Name of the attribute
        inline std::string getAttribute(std::string attribute)
        {
            return tagAttributes.find(attribute)->second;
        }

        // @brief   Returns the list of children of the node.
        inline std::list<XMLnodeUID> getChildrenUID()
        {
            return children;
        }

        // @brief   Adds a new child to the node.
        // @param   xml_parser::XMLnodeUID child    Child node.
        inline void addChild(XMLnodeUID child)
        {
            children.push_back(child);
        }

        // @brief   Removes the child node with the given UID.
        // @param   std::XMLnodeUID uid     UID of the child node to remove.
        void removeChild(XMLnodeUID uid)
        {
            children.remove(uid);
        }

        // @brief   Returns the parent node UID.
        inline XMLnodeUID getParent()
        {
            return parent;
        }

        // @brief   Sets a new parent node.
        // @param   xml_parser::XMLnodeUID new_parent_UID    UID of the new parent node.
        inline void setParent(XMLnodeUID new_parent_UID)
        {
            parent = new_parent_UID;
        }

        // @brief   Operator overload for =operator
        void operator=(const XMLnode &node)
        {
            uid = node.uid;
            parent = node.parent;
            children = node.children;
            innerData = node.innerData;
            tagAttributes = node.tagAttributes;
            tagName = node.tagName;
        }
    };

}; // namespace xml_parser

#endif