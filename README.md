# XML-Parser
 A small XML Parsing library. Work in progress.  
  
Current capabilities:
 1) Scan basic markup code.
 2) Store the markup data in a tree structure.
 3) Storage of the data is highly optimized in terms of storage as well as speed for fast manipulations like moving whole subtrees and multiple deletions-additions of nodes. 

Needed work:
  1) Currently it is not resilient to syntax errors.
  2) Currently it cannot read the plain text data in an element, i.e., the plain text between opening and closing tags.
