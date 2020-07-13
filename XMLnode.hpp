#ifndef XML_PARSER_XML_NODE
#define XML_PARSER_XML_NODE

#include <list>
#include <map>
#include <string>

namespace xml_parser
{
    // @brief   Node in the XML tree.
    template <typename T>
    class node
    {
    private:
        T *parent;
        std::list<node<T>> children;
        std::string innerData;
        std::map<std::string, std::string> tagAttributes;
        std::string tagName;

    public:
        node(std::string tagName) : tagName(tagName){};
        // ~node()
        // {
        //     children.~_List_base();
        //     tagAttributes.~map();
        //     tagName.~basic_string();
        //     delete parent;
        // }

        // @brief   Returns the tagName of the node.
        inline std::string getTagName()
        {
            return tagName;
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
        // @param   std::string value        Data of the attribute
        void setAttribute(std::string attribute, std::string value)
        {
            tagAttributes[attribute] = value;
        }

        // @brief   Gets the value of the said attribute. Returns
        //          empty string if the attribute does not exist.
        // @param   std::string attribute   Name of the attribute
        std::string getAttribute(std::string attribute)
        {
            return tagAttributes.find(attribute)->second;
        }

        // @brief   Adds a new child to the node.
        // @param   xml_parser::node<T> child   Child node.
        void addChild(node<T> child)
        {
            children.push_back(child);
        }

        // @brief   Removes all the child nodes with tagName same as
        //          childTagName.
        // @param   std::string childTagName   Tag name of the child node to remove.
        // @return  int number of child nodes removed.
        int removeChildren(std::string childTagName)
        {
            int count = 0;
            for (auto i = children.begin(); i != children.end(); ++i)
            {
                if ((*i).getTagName() == childTagName)
                {
                    i = children.erase(i);
                    ++count;
                }
            }
            return count;
        }
    };

}; // namespace xml_parser

#endif