////////////////////////////////////////////////////////////////////////////////
///This is a part of the course "Algorithms and Data Structures" 
///provided by  the School of Software Engineering of the Faculty 
///of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////


#include <stdexcept>        // std::invalid_argument


namespace xi
{

template<typename Element, typename Compar>
RBTree<Element, Compar>::Node::~Node()
{
    if (_left)
        delete _left;
    if (_right)
        delete _right;
}

template<typename Element, typename Compar>
typename RBTree<Element, Compar>::Node* RBTree<Element, Compar>::Node::setLeft(Node* lf)
{
    if (_left == lf)
        return nullptr;
    
    if (lf)
    {
        if (lf->_parent)
        {
            if (lf->_parent->_left == lf)
                lf->_parent->_left = nullptr;
            else                                    
                lf->_parent->_right = nullptr;
        }
        
        lf->_parent = this;
    }
    
    Node* prevLeft = _left;
    _left = lf;
    
    if (prevLeft)
        prevLeft->_parent = nullptr;
    
    return prevLeft;
}

template<typename Element, typename Compar>
typename RBTree<Element, Compar>::Node* RBTree<Element, Compar>::Node::setRight(Node* rg)
{
    if (_right == rg)
        return nullptr;
    
    if (rg)
    {
        if (rg->_parent)
        {
            if (rg->_parent->_right == rg)
                rg->_parent->_right = nullptr;
            else                                    
                rg->_parent->_left = nullptr;
        }
        
        rg->_parent = this;
    }
    
    Node* prevRight = _right;
    _right = rg;
    
    if (prevRight)
        prevRight->_parent = nullptr;
    
    return prevRight;
}

template<typename Element, typename Compar>
RBTree<Element, Compar>::RBTree()
{
    _root = nullptr;
    _dumper = nullptr;
}

template<typename Element, typename Compar>
RBTree<Element, Compar>::~RBTree()
{
    if (_root)
        delete _root;
}

template<typename Element, typename Compar>
void RBTree<Element, Compar>::deleteNode(Node* nd)
{
    if (nd == nullptr)
        return;
    
    delete nd;
}

template<typename Element, typename Compar>
void RBTree<Element, Compar>::insert(const Element &key)
{
    Node* newNode = insertNewBstEl(key);
    
    if (_dumper)
        _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_BST_INS, this, newNode);
    
    rebalance(newNode);
    
    if (_dumper)
        _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_INSERT, this, newNode);
    
}

template<typename Element, typename Compar>
const typename RBTree<Element, Compar>::Node* RBTree<Element, Compar>::find(const Element &key)
{
    Node* curr_node = _root;
    
    while (curr_node != nullptr)
    {
        if (key < curr_node->getKey())
            curr_node = curr_node->getChild(true);
        else if (key > curr_node->getKey())
            curr_node = curr_node->getChild(false);
        else
            return curr_node;
    }
    return nullptr;
};

template<typename Element, typename Compar>
typename RBTree<Element, Compar>::Node*
RBTree<Element, Compar>::insertNewBstEl(const Element &key)
{
    Node* y = nullptr;
    Node* x = _root;
    
    while (x != nullptr)
    {
        y = x;
        if (key < x->getKey())
            x = x->getChild(true);
        else if (key > x->getKey())
            x = x->getChild(false);
        else
            throw std::invalid_argument("The element with the same key is already placed");
    }
    
    Node* newNode = new Node(key, nullptr, nullptr, nullptr, RED);
    
    if (y == nullptr) 
    {
        _root = newNode;
        newNode->setBlack();
    }
    else
    {
        if (key < y->getKey())
            y->setLeft(newNode);
        else
            y->setRight(newNode);
    }
    
    return newNode;
}

template<typename Element, typename Compar>
typename RBTree<Element, Compar>::Node*
RBTree<Element, Compar>::rebalanceDUG(Node* nd)
{    
    Node* uncle = nd->_parent->_parent->getChild(!nd->_parent->isLeftChild()); 
    
    if (uncle != nullptr && uncle->isRed()) 
    {
        uncle->setBlack();
        nd->_parent->setBlack();
        uncle->_parent->setRed();
        nd = uncle->_parent;
        
        if (_dumper)
            _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_RECOLOR1, this, nd);
    }
    else
    {
        if (nd->_parent->isLeftChild() && nd->isRightChild()) 
        {
            nd = nd->_parent;
            rotLeft(nd);
        }
        else if (nd->_parent->isRightChild() && nd->isLeftChild()) 
        {
            nd = nd->_parent;
            rotRight(nd);
        }
        
        nd->_parent->setBlack();
        if (_dumper)
            _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_RECOLOR3D, this, nd);
        
        nd->_parent->_parent->setRed();
        if (_dumper)
            _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_RECOLOR3G, this, nd);
        
        if (nd->_parent->isLeftChild())
            rotRight(nd->_parent->_parent); 
        else
            rotLeft(nd->_parent->_parent);
    }
    
    return nd;
}

template<typename Element, typename Compar>
void RBTree<Element, Compar>::rebalance(Node* nd)
{
    if (nd->isBlack())
        return;
    
    while (nd->isDaddyRed())
    {
        nd = rebalanceDUG(nd);
    }
    _root->setBlack();
}

template<typename Element, typename Compar>
void RBTree<Element, Compar>::rotLeft(typename RBTree<Element, Compar>::Node* nd)
{
    Node* y = nd->getChild(false);
    if (!y)
        throw std::invalid_argument("Can't rotate left since the right child is nil");
    
    nd->setRight(y->getChild(true));
    if (y->getChild(true) != nullptr)
        y->getChild(true)->_parent = nd;
    
    if (nd->_parent == nullptr)
        _root = y;
    else if (nd->isLeftChild())
        nd->_parent->setLeft(y);
    else
        nd->_parent->setRight(y);
    
    y->setLeft(nd);
    nd->_parent = y;
    
    if (_dumper)
        _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_LROT, this, nd);
}

template<typename Element, typename Compar>
void RBTree<Element, Compar>::rotRight(typename RBTree<Element, Compar>::Node* nd)
{
    Node* y = nd->getChild(true);
    if (!y)
        throw std::invalid_argument("Can't rotate right since the left child is nil");
    
    nd->setLeft(y->getChild(false));
    if (y->getChild(false) != nullptr)
        y->getChild(false)->_parent = nd;
    
    if (nd->_parent == nullptr)
        _root = y;
    else if (nd->isLeftChild())
        nd->_parent->setLeft(y);
    else
        nd->_parent->setRight(y);
    
    y->setRight(nd);
    nd->_parent = y;
    
    if (_dumper)
        _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_RROT, this, nd);
}
    
} // namespace xi

