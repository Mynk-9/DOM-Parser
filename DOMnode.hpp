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

#ifndef DOM_PARSER_DOM_NODE
#define DOM_PARSER_DOM_NODE

#include <list>
#include <map>
#include <string>

#include "DOMnodeUID.hpp"

namespace dom_parser
{
    /// @brief   Node in the DOM tree.
    class DOMnode
    {
    private:
        DOMnodeUID uid;
        DOMnodeUID parent;
        std::list<DOMnodeUID> children;
        std::map<std::string, std::string> tagAttributes;
        std::string tagName;

        bool innerDataNode = false;
        std::string innerData;

    public:
        /**
         * @brief   Constructor for DOMnode
         * @param   std::string             tagName     Name of the tag of the node.
         * @param   dom_parser::DOMnodeUID  uid         UID of this node.
         */
        DOMnode(std::string tagName, DOMnodeUID uid, DOMnodeUID parent)
            : tagName(tagName), uid(uid), parent(parent){};

        /**
         * @brief   Constructor for DOMnode specially for storing inner-data
         * @param   DOMnodeUID  uid         UID of this node
         * @param   DOMnodeUID  parent      UID of the parent
         * @param   std::string innerData   inner text data stored by the node
         * */
        DOMnode(DOMnodeUID uid, DOMnodeUID parent, std::string innerData)
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
        inline DOMnodeUID getUID()
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
         * @brief   Returns reference to the the ordered map of all the attributes
         *          with their values.
         * */
        inline const std::map<std::string, std::string> &getAllAttributes()
        {
            return tagAttributes;
        }

        /**
         * @brief   Returns reference to the list of children of the node.
         */
        inline const std::list<DOMnodeUID> &getChildrenUID()
        {
            return children;
        }

        /**
         * @brief   Adds a new child to the node.
         * @param   dom_parser::DOMnodeUID child    Child node.
         */
        inline void addChild(DOMnodeUID child)
        {
            if (innerDataNode)
                return;
            children.push_back(child);
        }

        /**
         * @brief   Removes the child node with the given UID.
         * @param   std::DOMnodeUID uid     UID of the child node to remove.
         */
        void removeChild(DOMnodeUID uid)
        {
            if (innerDataNode)
                return;
            children.remove(uid);
        }

        /**
         * @brief   Returns the parent node UID.
         */
        inline DOMnodeUID getParent()
        {
            return parent;
        }

        /**
         * @brief   Sets a new parent node.
         * @param   dom_parser::DOMnodeUID new_parent_UID    UID of the new parent node.
         */
        inline void setParent(DOMnodeUID new_parent_UID)
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
         * @brief   Returns reference to inner-data if the node stores inner data.
         *          Returns empty string if node does not store inner-data.
         * */
        inline const std::string &getInnerData()
        {
            return innerData;
        }

        /**
         * @brief   Operator overload for =operator
         */
        void operator=(const DOMnode &node)
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

}; // namespace dom_parser

#endif
