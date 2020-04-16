////////////////////////////////////////////////////////////////////////////////
///This is a part of the course "Algorithms and Data Structures" 
///provided by  the School of Software Engineering of the Faculty 
///of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////


#ifndef RBTREE_RBTREE_H_
#define RBTREE_RBTREE_H_


namespace xi
{


template<typename Element, typename Compar>
class RBTree;


template<typename Element, typename Compar>
class IRBTreeDumper
{
public:
    typedef RBTree<Element, Compar> TTree;
    typedef typename RBTree<Element, Compar>::Node TTreeNode;
public:
    enum RBTreeDumperEvent
    {
        DE_AFTER_LROT,                  
        DE_AFTER_RROT,                  
        DE_AFTER_BST_INS,               
        DE_AFTER_INSERT,                
        DE_AFTER_RECOLOR1,             
        DE_AFTER_RECOLOR3D,             
        DE_AFTER_RECOLOR3G,             
        
#ifdef RBTREE_WITH_DELETION
        DE_AFTER_BST_REMOVE,            
        DE_AFTER_REMOVE,                

       
#endif
    };
public:
    
    virtual void rbTreeEvent(RBTreeDumperEvent ev, TTree* tr, TTreeNode* nd) = 0;


protected:
    ~IRBTreeDumper() {};
}; // class RBTreeDumper


template<typename, typename>
class RBTreeTest;


template<typename Element, typename Compar = std::less<Element> >
class RBTree
{
public:
    enum Color
    {
        BLACK,
        RED
    };
    
    
    class Node
    {
        friend class RBTree<Element, Compar>;
        
        template<typename, typename>
        friend
        class RBTreeTest;
    
    public:
        
        enum WhichChild
        {
            LEFT,               
            RIGHT,              
            NONE               
        };
    public:
        
        const Node* getLeft() const { return _left; }
        
        const Node* getRight() const { return _right; }
        
        const Node* getParent() const { return _parent; }
        
        Color getColor() const { return _color; }
        
        bool isBlack() const { return _color == BLACK; }
        
        bool isRed() const { return _color == RED; }
        
        
        bool isDaddyRed() const
        {
            if (!_parent)
                return false;
            return (_parent->_color == RED);
        }
        
        
        bool isLeftChild() const
        {
            if (!_parent)
                return false;
            return (_parent->_left == this);
        }
        
        
        bool isRightChild() const
        {
            if (!_parent)
                return false;
            return (_parent->_right == this);
        }
        
        WhichChild getWhichChild() const
        {
            if (!_parent)
                return NONE;
            if (_parent->_left == this)
                return LEFT;
            return RIGHT;
        }
        
        
        const Element &getKey() const { return _key; }
    
    protected:
        
        Node(const Element &key = Element(),
             Node* left = nullptr,
             Node* right = nullptr,
             Node* parent = nullptr,
             Color col = BLACK)
                : _key(key), _left(left), _right(right), _parent(parent), _color(col)
        {
            
            if (_left)
                _left->_parent = this;
            
            if (_right)
                _right->_parent = this;
        }
        
        ~Node();                               
    
    protected:
        Node(const Node &);                      
        Node &operator=(Node &);               
    
    protected:
        
        Node* setLeft(Node* lf);
       
        Node* setRight(Node* rg);
        
        void setBlack() { _color = BLACK; }
        
        void setRed() { _color = RED; }
        

        Node* getDaddy(bool &isLeftChild)
        {
            if (!_parent)
                return nullptr;
            
            isLeftChild = (_parent->_left == this);
            
            return _parent;
        }
        
        Node* getChild(bool isLeft)
        {
            return isLeft ? _left : _right;
        }
        
        
        bool isSpecificChildPrv(bool isLeft) const
        {
            if (isLeft)        
                return (_parent->_left == this);
            return (_parent->_right == this);
        }
    
    
    protected:
        Element _key;                           
        Color _color;                         
        
        Node* _parent;                       
        Node* _left;                      
        Node* _right;                       
    }; // class RBTree::Node
    
    friend class Node;

public:
    RBTree();                                   
    ~RBTree();                                  

public:
    void insert(const Element &key);

#ifdef RBTREE_WITH_DELETION
                                                                                                                            
    void remove(const Element& key);
#endif
    
    const Node* find(const Element &key);
    
    bool isEmpty() const { return _root == nullptr; }
    
    const Node* getRoot() const { return _root; }

public:
    
    void setDumper(IRBTreeDumper<Element, Compar>* dumper)
    {
        _dumper = dumper;
    }
    
    void resetDumper()
    {
        _dumper = nullptr;
    }

protected:
    
    Node* insertNewBstEl(const Element &key);
    
    void rebalance(Node* nd);
   
    Node* rebalanceDUG(Node* nd);
    
    void deleteNode(Node* nd); 
    
    void rotLeft(Node* nd);
   
    void rotRight(Node* nd);


protected:
    RBTree(const RBTree &);                      
    RBTree &operator=(RBTree &);                

protected:
    Compar _compar;                            

protected:
    
    Node* _root;


protected:
    IRBTreeDumper<Element, Compar>* _dumper;
    
    template<typename, typename>
    friend
    class RBTreeTest;
    
}; // class RBTree
    
    
    
} // namespace xi



#include "rbtree.hpp"


#endif // RBTREE_RBTREE_H_