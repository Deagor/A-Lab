#ifndef GRAPH_H
#define GRAPH_H

#include "stdafx.h"
#include <list>
#include <queue>

using namespace std;

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

template <class NodeType, class ArcType>
class NodeCostComparer {
public:
	bool operator()(GraphNode<NodeType, ArcType> * n1, GraphNode<NodeType, ArcType> * n2) const 
	{
		pair<string, int> p1 = n1->data();
		pair<string, int> p2 = n2->data();
		return p1.second > p2.second;
	}

};

template<class NodeType,class ArcType>
class NodeFComparer
{
public:
	bool operator()(GraphNode<NodeType, ArcType> *n1, GraphNode<NodeType, ArcType> *n2) const
	{
		float p1 = n1->getHValue() + n1->getGValue();
		float p2 = n2->getHValue() + n2->getGValue();
		return p2 < p1;
	}
};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    This is the graph class, it contains all the
//                  nodes.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph {
private:

    // typedef the classes to make our lives easier.
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

// ----------------------------------------------------------------
//  Description:    An array of all the nodes in the graph.
// ----------------------------------------------------------------
    Node** m_pNodes;

// ----------------------------------------------------------------
//  Description:    The maximum number of nodes in the graph.
// ----------------------------------------------------------------
    int m_maxNodes;


// ----------------------------------------------------------------
//  Description:    The actual number of nodes in the graph.
// ----------------------------------------------------------------
    int m_count;





public:           
    // Constructor and destructor functions
    Graph( int size );
    ~Graph();

    // Accessors
    Node** nodeArray() const {
       return m_pNodes;
    }

	int getCount() const { return m_count; }

    // Public member functions.
    bool addNode( NodeType data, int index,int posX,int posY );
    void removeNode( int index );
    bool addArc( int from, int to, ArcType weight );
    void removeArc( int from, int to );
    Arc* getArc( int from, int to );        
    void clearMarks();
    void depthFirst( Node* pNode, void (*pProcess)(Node*) );
    void breadthFirst( Node* pNode, void (*pProcess)(Node*) );

	//adapted breadthfirst
	void adaptedBreadthFirst(Node* pNode, void(*pProcess)(Node*), Node* target);
	void UCS(Node* pStart, Node* pDest/*, void (*pProcess)(int,int,Node*)*/);
	void aStar(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node*>&path);
	void test();
	void setH(int G, int arcWeight, Node* cNode);
	void ResetNodes();


};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph( int size ) : m_maxNodes( size ) {
   int i;
   m_pNodes = new Node * [m_maxNodes];
   // go through every index and clear it to null (0)
   for( i = 0; i < m_maxNodes; i++ ) {
        m_pNodes[i] = 0;
   }

   // set the node count to 0.
   m_count = 0;
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
   int index;
   for( index = 0; index < m_maxNodes; index++ ) {
        if( m_pNodes[index] != 0 ) {
            delete m_pNodes[index];
        }
   }
   // Delete the actual array
   delete m_pNodes;
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode( NodeType data, int index,int posX,int posY) {
   bool nodeNotPresent = false;
   // find out if a node does not exist at that index.
   if ( m_pNodes[index] == 0) {
      nodeNotPresent = true;
      // create a new node, put the data in it, and unmark it.
      m_pNodes[index] = new Node;
      m_pNodes[index]->setData(data);
      m_pNodes[index]->setMarked(false);
	  m_pNodes[index]->setPos(posX, posY);

	  DrawLevel::getSingleton()->CreateShape(posX, posY);
      // increase the count and return success.
      m_count++;
    }
        
    return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode( int index ) {
     // Only proceed if node does exist.
     if( m_pNodes[index] != 0 ) {
         // now find every arc that points to the node that
         // is being removed and remove it.
         int node;
         Arc* arc;

         // loop through every node
         for( node = 0; node < m_maxNodes; node++ ) {
              // if the node is valid...
              if( m_pNodes[node] != 0 ) {
                  // see if the node has an arc pointing to the current node.
                  arc = m_pNodes[node]->getArc( m_pNodes[index] );
              }
              // if it has an arc pointing to the current node, then
              // remove the arc.
              if( arc != 0 ) {
                  removeArc( node, index );
              }
         }
        

        // now that every arc pointing to the current node has been removed,
        // the node can be deleted.
        delete m_pNodes[index];
        m_pNodes[index] = 0;
        m_count--;
    }
}

// ----------------------------------------------------------------
//  Name:           addArd
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc( int from, int to, ArcType weight ) {
     bool proceed = true; 
     // make sure both nodes exist.
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     if( m_pNodes[from]->getArc( m_pNodes[to] ) != 0 ) {
         proceed = false;
     }

     if (proceed == true) {
        // add the arc to the "from" node.
        m_pNodes[from]->addArc( m_pNodes[to], weight );
     }
        
     return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc( int from, int to ) {
     // Make sure that the node exists before trying to remove
     // an arc from it.
     bool nodeExists = true;
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         nodeExists = false;
     }

     if (nodeExists == true) {
        // remove the arc.
        m_pNodes[from]->removeArc( m_pNodes[to] );
     }
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
// Dev-CPP doesn't like Arc* as the (typedef'd) return type?
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc( int from, int to ) {
     Arc* pArc = 0;
     // make sure the to and from nodes exist
     if( m_pNodes[from] != 0 && m_pNodes[to] != 0 ) {
         pArc = m_pNodes[from]->getArc( m_pNodes[to] );
     }
                
     return pArc;
}

// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks() {
     int index;
     for( index = 0; index < m_maxNodes; index++ ) {
          if( m_pNodes[index] != 0 ) {
              m_pNodes[index]->setMarked(false);
          }
     }
}


// ----------------------------------------------------------------
//  Name:           depthFirst
//  Description:    Performs a depth-first traversal on the specified 
//                  node.
//  Arguments:      The first argument is the starting node
//                  The second argument is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::depthFirst( Node* pNode, void (*pProcess)(Node*) ) {
     if( pNode != 0 ) {
           // process the current node and mark it
           pProcess( pNode );
           pNode->setMarked(true);

           // go through each connecting node
           list<Arc>::iterator iter = pNode->arcList().begin();
           list<Arc>::iterator endIter = pNode->arcList().end();
        
		   for( ; iter != endIter; ++iter) {
			    // process the linked node if it isn't already marked.
                if ( (*iter).node()->marked() == false ) {
                   depthFirst( (*iter).node(), pProcess);
                }            
           }
     }
}


// ----------------------------------------------------------------
//  Name:           breadthFirst
//  Description:    Performs a depth-first traversal the starting node
//                  specified as an input parameter.
//  Arguments:      The first parameter is the starting node
//                  The second parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirst( Node* pNode, void (*pProcess)(Node*) ) {
   if( pNode != 0 ) {
	  queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( pNode );
      pNode->setMarked(true);

      // loop through the queue while there are nodes in it.
      while( nodeQueue.size() != 0 ) {
         // process the node at the front of the queue.
         pProcess( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
         list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) {
              if ( (*iter).node()->marked() == false) {
				 // mark the node and add it to the queue.
                 (*iter).node()->setMarked(true);
                 nodeQueue.push( (*iter).node() );
              }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  
}

//Adapted breadth first
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::adaptedBreadthFirst( Node* pNode, void (*pProcess)(Node*), Node* target ) {

	bool goalReached = false;

   if( pNode != 0) {
	  queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( pNode );
      pNode->setMarked(true);

      // loop through the queue while there are nodes in it and target not reached
      while( nodeQueue.size() != 0  && goalReached == false) {
         // process the node at the front of the queue.
         pProcess( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
         list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) {
			 if((*iter).node() == target)
			 {
				 goalReached = true;
				 target->setPrevNode((*iter).node());
			 }
              if ( (*iter).node()->marked() == false) {
				  (*iter).node()->setPrevNode(nodeQueue.front());
				 // mark the node and add it to the queue.
                 (*iter).node()->setMarked(true);
                 nodeQueue.push( (*iter).node() );
              }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::UCS(Node* pStart, Node* pDest)
{
	priority_queue<Node*, vector<Node*>, NodeCostComparer<NodeType, ArcType>> pq;
	bool goalReached = false;
	
	for(int i=0;i<m_count;i++)
	{
		NodeType d = m_pNodes[i]->data();
		m_pNodes[i]->setData(make_pair(d.first, INT_MAX));
	}

	NodeType data = pDest->data();
	pDest->setData(make_pair(data.first, 0));
	pq.push(pDest);
	pDest->setMarked(true);
	pDest->setHValue(0);

	while(pq.size() != 0 && pq.top() != pStart)
	{
		auto currentNode = pq.top();
		pq.pop();
		 list<Arc>::const_iterator iter = currentNode->arcList().begin();
		 list<Arc>::const_iterator endIter = currentNode->arcList().end();
		 
		 for( ; iter != endIter; iter++ ) 
		 {
			 if ((*iter).node()->marked()){continue; }
			 else if ((*iter).node() != (currentNode)->getPrevNode())
			 {
				 setH(currentNode->data().second, (*iter).weight(), (*iter).node());
				 //distC = weight of arc from pq.top to the child + distance from top
				 float distC = (*iter).weight() + currentNode->data().second;
				 if (distC < (*iter).node()->data().second)
				 {
					 NodeType data = (*iter).node()->data();
					 (*iter).node()->setData(make_pair(data.first, distC));
					 (*iter).node()->setPrevNode(currentNode);

					 if ((*iter).node()->marked() == false)
					 {
						 pq.push((*iter).node());
						 (*iter).node()->setMarked(true);
					 }
				 }
			 }
		 }
	}
	/*if (pq.size() != 0 && pq.top() == pDest)
	{
		list<Arc>::const_iterator iter = pq.top()->arcList().begin();
		list<Arc>::const_iterator endIter = pq.top()->arcList().end();

		for (int i = 0; i < length; i++)
		{

		}
	}*/
}

template<class NodeType,class ArcType>
void Graph<NodeType, ArcType>::aStar(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node*>&path)
{
	priority_queue<Node*, vector<Node*>, NodeFComparer<NodeType, ArcType>> pq;
	
	bool goalReached = false;

	UCS(pStart, pDest);

	for (int i = 0; i < m_count; i++)
	{
		m_pNodes[i]->setMarked(false);
		m_pNodes[i]->setPrevNode(NULL);
	}
	
	pStart->setGValue(0);
	pq.push(pStart);
	pStart->setMarked(true);

	while (pq.size() != 0 && pq.top() != pDest)
	{
		auto currentNode = pq.top();
		pq.pop();

		list<Arc>::const_iterator iter = currentNode->arcList().begin();
		list<Arc>::const_iterator endIter = currentNode->arcList().end();

		for (; iter != endIter; iter++)
		{
			if ((*iter).node() != (currentNode)->getPrevNode())
			{
				//distC = weight of arc from pq.top to the child + distance from top
				float distC = currentNode->getGValue() + (*iter).node()->getHValue();
				if (distC < (*iter).node()->getGValue())
				{
					(*iter).node()->setGValue(currentNode->getGValue() + (*iter).weight());
					(*iter).node()->setPrevNode(currentNode);

					if ((*iter).node()->marked() == false)
					{
						pq.push((*iter).node());
						//make_heap(pq.top(), pq.top() + pq.size(),NodeFComparer)
						//make_heap(const_cast<Node**>(&pq.top()), const_cast<Node**>(&pq.top()) + pq.size(), NodeFComparer<NodeType, ArcType>());
						(*iter).node()->setMarked(true);
					}
				}
			}
		}	
	}
	//found target. put path into path vector(traverse the previous nodes)
	Node* target = pDest;
	while (target->getPrevNode() != NULL)
	{
		path.push_back(target);
		target->setInPath(true);
		target = target->getPrevNode();
	}
	if (target->getPrevNode() == NULL)
	{
		path.push_back(target);
		target->setInPath(true);
	}
}

template<class NodeType,class ArcType>
void Graph<NodeType, ArcType>::test()
{
	for (int i = 0; i < m_count; i++)
	{ 
		cout << m_pNodes[i]->data().first << "\t";
		cout << m_pNodes[i]->getHValue() << "\t";
		cout << m_pNodes[i]->getGValue() << "\t";
		//int numArcs = m_pNodes[i]->arcList().size();
		//auto theList = m_pNodes[i]->arcList();
		//for (auto iter = theList.begin(); iter != theList.end(); iter++)
		//{
		//	cout << iter->weight() << "\t";
		//}
		cout << endl;
	}
}

template<class NodeType,class ArcType>
void Graph<NodeType,ArcType>::setH(int G, int arcWeight, Node* cNode)
{
	cNode->setHValue((G + arcWeight) * 0.9f);
}
#include "GraphNode.h"
#include "GraphArc.h"


template<class NodeType,class ArcType>
void Graph<NodeType, ArcType>::ResetNodes()
{
	for (int i = 0; i < m_count; i++)
	{
		m_pNodes[i]->setMarked(false);
		m_pNodes[i]->setPrevNode(NULL);
		m_pNodes[i]->setInPath(false);
		m_pNodes[i]->setGValue(INT_MAX);
		m_pNodes[i]->setHValue(INT_MAX);
	}
}

#endif
