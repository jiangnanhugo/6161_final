/*
 * Robert Emerson
 */

#include "BoruvkaTree.hpp"

BoruvkaTree::BoruvkaTree()
{
  
}
/**
* Construct the tree.
*/
BoruvkaTree::BoruvkaTree( int numLeaves, std::map<int, vertex_descriptor> intToVertex )
{
  this->root = new BoruvkaNode;
  this->numNodes = 2*numLeaves;
  this->intToVertex = intToVertex;
  this->numParents = 0; //Number of non-leaf nodes, not including root
  
  std::map< int, vertex_descriptor>::iterator it = intToVertex.begin();
  for ( ; it != intToVertex.end(); ++it)
  {
    this->vertexToInt.insert( std::pair< vertex_descriptor, int>( it->second, it->first) );
  }
}


/**
* Destructor for the tree.
*/
BoruvkaTree::~BoruvkaTree( )
{
    this->makeEmpty();
}

/**
 * Create all the leaves of the tree
 */
void BoruvkaTree::create( std::vector<vertex_descriptor> vertices)
{
  std::vector<vertex_descriptor>::iterator first = vertices.begin();
  vertex_descriptor rootVertex = boost::graph_traits<Graph>::null_vertex();
  vertexToNode[rootVertex] = root;
  nodeToVertex[root] = rootVertex;
  
  for( ; first != vertices.end(); ++first)
  {
      BoruvkaNode* newNode = new BoruvkaNode(*first);
      newNode->setParent( root, -1);
      vertexToNode[*first] = newNode;
      nodeToVertex[newNode] = *first;
  }
}

/**
 * Sets the parent for the selected vertex descriptor
 */
void BoruvkaTree::setParent( vertex_descriptor child1, vertex_descriptor child2, int weight)
{
   if ( vertexToNode[child1]->getParent() == root && vertexToNode[child2]->getParent() == root)
  {
    //Set the parent to be a null node as it isn't in the real graph, just the Boruvka steps for a bit
    vertex_descriptor parent = boost::graph_traits<Graph>::null_vertex();
    BoruvkaNode* newNode = new BoruvkaNode(parent);
    
    newNode->setParent( root, -1);
    vertexToNode[parent] = newNode;
    nodeToVertex[newNode] = parent;
    intToVertex.insert( std::pair<int, vertex_descriptor>( (numNodes/2)+numParents, parent) );
    vertexToInt.insert( std::pair<vertex_descriptor, int>( parent, (numNodes/2)+numParents) );
    
    setParent(vertexToNode[child1], vertexToNode[parent], weight);
    setParent(vertexToNode[child2], vertexToNode[parent], weight);
  }
  else if ( vertexToNode[child1]->getParent() != root && vertexToNode[child2]->getParent() != root)
  {
    setParent( nodeToVertex[vertexToNode[child1]->getParent()], nodeToVertex[vertexToNode[child2]->getParent()], weight);
  }
  else if ( vertexToNode[child1]->getParent() != root && vertexToNode[child2]->getParent() == root)
  {
    setParent(vertexToNode[child2], vertexToNode[child1]->getParent(), weight);
  }
  else if ( vertexToNode[child1]->getParent() == root && vertexToNode[child2]->getParent() != root)
  {
    setParent(vertexToNode[child1], vertexToNode[child2]->getParent(), weight);
  }
}

void BoruvkaTree::setParent( BoruvkaNode* child, BoruvkaNode* parent, int weight)
{
  child->setParent(parent, weight);
  parent->addChild(child);
}

/**
* Make the tree logically empty.
*/
void BoruvkaTree::makeEmpty( )
{
    // call the private makeEmpty() routine
    makeEmpty( root);
}

BoruvkaNode* BoruvkaTree::getRoot() const
{
  return this->root;
}

//need to implement getChildren and getSiblings still

/**
* Internal method to make subtree empty.
*/
void BoruvkaTree::makeEmpty( BoruvkaNode* parent )
{
    if ( parent != NULL )
    {
      std::vector<BoruvkaNode*>::iterator childNode = parent->getChildren().begin();
      for (; childNode != parent->getChildren().end(); ++childNode)
      {
	makeEmpty(*childNode);
      }
        delete parent;
    }
}

std::vector<int> BoruvkaTree::getChildren()//std::map<int, vertex_descriptor> intsToVertex)
{
  std::vector<int> children = std::vector<int>(numNodes, -1);
  std::map< BoruvkaNode*, vertex_descriptor>::iterator nodeIterator = nodeToVertex.begin();
  //Still a linear operation as the multiplication of times we run the inner and outer loop is <= number of nodes
  for ( ; nodeIterator != nodeToVertex.end(); ++nodeIterator)
  {
    int childNumber = -1;
    vertex_descriptor currentVertex = nodeIterator->second;
    
    if ( !nodeIterator->first->getChildren().empty())
    {
      childNumber = vertexToInt[nodeToVertex[nodeIterator->first->getChildren().front()]];
    }
    
    children[vertexToInt[currentVertex]] = childNumber;
  }
  
  return children;
}

std::vector<int> BoruvkaTree::getSiblings()
{
  std::vector<int> siblings = std::vector<int>(numNodes, -1);
  std::map< BoruvkaNode*, vertex_descriptor>::iterator nodeIterator = nodeToVertex.begin();
  //Still a linear operation as the multiplication of times we run the inner and outer loop is <= number of nodes
  for ( ; nodeIterator != nodeToVertex.end(); ++nodeIterator)
  {
     if ( nodeIterator->first->getParent() != NULL)
     {
	std::vector<BoruvkaNode*> temp = nodeIterator->first->getParent()->getChildren();
	vertex_descriptor currentVertex = nodeIterator->second;
	int siblingNumber = -1;
	
	std::vector<BoruvkaNode*>::iterator findCurrent = temp.begin();
	while ( findCurrent != temp.end() && *findCurrent != vertexToNode[currentVertex] )
	{
	  ++findCurrent;
	}
	if ( findCurrent != temp.end() )
	{
	  siblingNumber = vertexToInt[nodeToVertex[*findCurrent]];
	}
	
	siblings[vertexToInt[currentVertex]] = siblingNumber;
     }
  }
  return siblings;
}

int BoruvkaTree::getRootInt()
{
  return vertexToInt[nodeToVertex[root]];
}