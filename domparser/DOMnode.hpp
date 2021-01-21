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

        // if innerData node
        bool innerDataNode = false;
        std::string innerData;

    public:
        /**
         * @brief   Constructor for DOMnode
         * @param   tagName     Name of the tag of the node.
         * @param   uid         UID of this node.
         */
        DOMnode(std::string tagName, DOMnodeUID uid, DOMnodeUID parent)
            : tagName(tagName), uid(uid), parent(parent){};

        /**
         * @brief   Constructor for DOMnode specially for storing inner-data
         * @param   uid         UID of this node
         * @param   parent      UID of the parent
         * @param   innerData   inner text data stored by the node
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
         * @param   attribute   Name of the attribute
         * @param   value       Data of the attribute
         */
        inline void setAttribute(std::string attribute, std::string value)
        {
            if (innerDataNode)
                return;
            tagAttributes[attribute] = value;
        }

        /**
         * @brief   Sets attributes from the std::map provided
         *          which has pairs like {attribute, value}.
         *          Previous attributes will be cleared.
         * @param   attributes  attributes to be set
         */
        inline void setAttributes(const std::map<std::string, std::string> &attributes)
        {
            tagAttributes = attributes;
        }

        /**
         * @brief   Sets attributes from the rvalue std::map provided
         *          which has pairs like {attribute, value}.
         *          Previous attributes will be cleared.
         * @param   attributes  attributes to be set
         */
        inline void setAttributes(std::map<std::string, std::string> &&attributes)
        {
            tagAttributes.swap(attributes);
        }

        /**
         * @brief   Gets the value of the said attribute. Returns
         *          empty string if the attribute does not exist.
         * @param   attribute   Name of the attribute
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
         * @param   child    Child node.
         */
        inline void addChild(DOMnodeUID child)
        {
            if (innerDataNode)
                return;
            children.push_back(child);
        }

        /**
         * @brief   Removes the child node with the given UID.
         * @param   uid     UID of the child node to remove.
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
         * @param   new_parent_UID    UID of the new parent node.
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
         *    Operator overload for =operator removed.
         *    Reason:
         *      A situation may arise when the user would need to copy one
         *      node to another. This would create the following problems:
         *      1) If the user uses nodes vector in DOMtree, then they would
         *          only be using =operator in shared_ptr and this would
         *          result in multiple pointers to the same node. So if a 
         *          change to one copy is made, similar changes would reflect
         *          on another. This essentially means that a node is created
         *          which mirrors the original node and which has incorrect 
         *          connections with child nodes.
         *      2) If user wants to copy node across trees, this should not 
         *          be allowed for the sake of preventing errors as node UIDs
         *          may not be consistent across different trees and hence,
         *          connections of copied node might be inconsistent.
         *      3) If one node is copied, then the original and copied nodes
         *          would have same set of children nodes. Any node can have 
         *          only one parent node, and since children nodes are not 
         *          updated while copying, hence a copied node would have 
         *          inconsistent connections which point to the children 
         *          nodes but children nodes do not point back.
         *    Considering the above reasons, =operator overload is removed from
         *    DOMnode.
         */
    };

}; // namespace dom_parser

#endif
