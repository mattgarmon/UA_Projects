//*****************************************************************************//
//  Matt Garmon ~ 2016                                                         //
//  AVL_Tree.hpp - Declaration/Implementation of the AVL Binary Tree Class     //
//*****************************************************************************//

#ifndef AVL_TREE
#define AVL_TREE
#include <queue>
#include <utility>
#include <iostream>
#include <algorithm>

//the AVL balanced tree class
template <class M>
class AVL_Tree {
private:
    //node structure
    struct Node {
        M data; //the key of the node
        int height; //the height of the node
        Node* left,* right,* parent; //node pointers to left right and parent
        Node(M key, int h, Node* p) //parameterized node constructor
            : data(key), height(h), left(nullptr), right(nullptr), parent(p) {}
    };
    Node* root; //the root node of the AVL tree
    bool is_left(Node* n) {return n->parent->left == n;} //utility function
    int height(Node* n) {if(n) return n->height; else return 0;} //get height function
    void update_height(Node* n) {n->height = std::max(height(n->left), height(n->right)) + 1;} //updates a nodes height
    int balance_factor(Node* n) {if(n) return height(n->left) - height(n->right); else return 0;} //returns the balance factor of a node
    Node* balance_tree(Node*, int); //balances the tree by choosing correct rotation
    Node* LL_rotate(Node*); //node rotation functions
    Node* RR_rotate(Node*);
    Node* LR_rotate(Node*);
    Node* RL_rotate(Node*);
    void destroy(Node*); //used for tree destruction
    void duplicate(Node* const &, Node* &); //used for copy construction
public:
    AVL_Tree() {root = nullptr;} //constructor
    ~AVL_Tree() {destroy(root);} //destructor
    AVL_Tree(const AVL_Tree &); //copy constructor
    AVL_Tree &operator=(const AVL_Tree &); //copy assignment operator
    Node* find(M key); //find function
    void insert(M); //insert function
    void erase(Node*); //erase function

    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
    Node* get_root() {return root;}
    void levels(const Node*);
    void display(Node*);
    int max_depth(Node*);
    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
};

//rotate function for Left-Left imbalance
template <class M>
typename AVL_Tree<M>::Node* AVL_Tree<M>::LL_rotate(Node* p) {
    Node* q = p->left; //get relevant node pointers
    Node* st = q->right;
    q->parent = p->parent; //link q and p's old parent
    if(p->parent){
        if(is_left(p)) p->parent->left = q;
        else p->parent->right = q;
    }
    q->right = p; //link q and p
    p->parent = q;
    p->left = st; //link p and the q's old subtree
    if(st) st->parent = p;
    update_height(p); //update heights
    update_height(q);
    if(root == p) root = q; //check for root update
    return q; //return the root of the balanced subtree
}

//rotate function for Right-Right imbalance
template <class M>
typename AVL_Tree<M>::Node* AVL_Tree<M>::RR_rotate(Node* p) {
    Node* q = p->right; //get relevant node pointers
    Node* st = q->left;
    q->parent = p->parent; //link q and p's old parent
    if(p->parent){
        if(is_left(p)) p->parent->left = q;
        else p->parent->right = q;
    }
    q->left = p; //link q and p
    p->parent = q;
    p->right = st; //link p and the q's old subtree
    if(st) st->parent = p;
    update_height(p); //update heights
    update_height(q);
    if(root == p) root = q; //check for root update
    return q; //return the root of the balanced subtree
}

//rotate function for Left-Right imbalance
template <class M>
typename AVL_Tree<M>::Node* AVL_Tree<M>::LR_rotate(Node* p) {
    Node* q = p->left; //get relevant node pointers
    Node* r = q->right;
    Node* st1 = r->left;
    Node* st2 = r->right;
    r->parent = p->parent; //link r and p's old parent
    if(p->parent){
            if(is_left(p)) p->parent->left = r;
            else p->parent->right = r;
    }
    r->left = q; //link r and q
    q->parent = r;
    r->right = p; //link r and p
    p->parent = r;
    q->right = st1; //link q and r's first old subtree
    if(st1) st1->parent = q;
    p->left = st2; //link p and r's second old subtree
    if(st2) st2->parent = p;
    update_height(q); //update heights
    update_height(p);
    update_height(r);
    if(root == p) root = r; //check for root update
    return r; //return the root of the balanced subtree
}

//rotate function for Right-Left imbalance
template <class M>
typename AVL_Tree<M>::Node* AVL_Tree<M>::RL_rotate(Node* p) {
    Node* q = p->right; //get relevant node pointers
    Node* r = q->left;
    Node* st1 = r->left;
    Node* st2 = r->right;
    r->parent = p->parent; //link r and p's old parent
    if(p->parent){
        if (is_left(p)) p->parent->left = r;
        else p->parent->right = r;
    }
    r->left = p; //link r and p
    p->parent = r;
    r->right = q; //link r and q
    q->parent = r;
    p->right = st1; //link p and r's first old subtree
    if(st1) st1->parent = p;
    q->left = st2; //link q and r's second old subtree
    if(st2) st2->parent = q;
    update_height(p); //update heights
    update_height(q);
    update_height(r);
    if(root == p) root = r; //check for root update
    return r; //return the root of the balanced subtree
}

//balance tree function, returns the root of newly balanced subtree
template <class M>
typename AVL_Tree<M>::Node* AVL_Tree<M>::balance_tree(Node* n, int bf) {
    //figure out which imbalance case it is
    if(bf == 2) { //left leaning
        if(balance_factor(n->left) < 0) return LR_rotate(n);
        else return LL_rotate(n);
    }
    if(bf == -2){ //right leaning
        if(balance_factor(n->right) < 0) return RR_rotate(n);
        else return RL_rotate(n);
    }
    return n;
}

//destroy function used for tree destruction
template <class M>
void AVL_Tree<M>::destroy(Node* n) {
    //recursively post-order erase the tree
    if(n) {
        if(n->left) destroy(n->left);
        if(n->right) destroy(n->right);
        delete n;
    }
}

//duplicate function used for copy construction
template <class M>
void AVL_Tree<M>::duplicate(Node* const &one, Node* &two) {
    if(!one) two = nullptr;
    //recursively pre-order duplicate the tree
    else {
        two = new Node(one->data, one->height, one->parent);
        //if(one == root) two->root = two; //update root
        duplicate(one->left, two->left);
        duplicate(one->right, two->right);
    }
}

//copy constructor
template <class M>
AVL_Tree<M>::AVL_Tree(const AVL_Tree &b) {
    if(!b.root) root = nullptr;
    else duplicate(b.root, this->root); //duplicate the tree
}

//copy assignment operator
template <class M>
AVL_Tree<M> &AVL_Tree<M>::operator=(const AVL_Tree &b) {
    if(!b.root) root = nullptr;
    else { //destroy current tree and duplicate source tree
        this->~AVL_Tree();
        duplicate(b.root, this->root);
    }
}

//function to find a key and return a pointer
template <class M>
typename AVL_Tree<M>::Node* AVL_Tree<M>::find(M key) {
    Node* i = root; //create an index
    while(i) {
        //try to find the key
        if (i->data == key) return i;
        if(i->data > key) i = i->left;
        else i = i->right;
    }
    //return a pointer to the key, nullptr if not found
    return i;
}

//function to insert a new node
template <class M>
void AVL_Tree<M>::insert(M key) {
    if(!root) { //if no tree, make new node the root
        root = new Node(key, 1, nullptr);
        return;
    }
    Node* i = root; //create an index
    while(true) { //find insertion point and insert new node
        if(i->data > key) {
            if(!i->left) {
                i->left = new Node(key, 1, i);
                update_height(i); //update index height
                while (i->parent) { //update all parent's heights
                    i = i->parent;
                    update_height(i);
                    //check balance factor and potentially fix imbalances
                    if(balance_factor(i) > 1 || balance_factor(i) < -1) {
                        i = balance_tree(i, balance_factor(i));
                    }
                }
                return;
            }
            else i = i->left;
        }
        if(i->data <= key) {
            if(!i->right) {
                i->right = new Node(key, 1, i);
                update_height(i); //update index height
                while (i->parent) { //update all parent's heights
                    i = i->parent;
                    update_height(i);
                    //check balance factor and potentially fix imbalances
                    if(balance_factor(i) > 1 || balance_factor(i) < -1) {
                        i = balance_tree(i, balance_factor(i));
                    }
                }
                return;
            }
            else i = i->right;
        }
    }
}

//Function to erase a node
template <class M>
void AVL_Tree<M>::erase(Node* n) {
    if(n) {
        //no children case
        if(!n->left && !n->right) {
            if(root == n) root = nullptr; //if node is root, make root null
            else { //if node is a child, update parent's children
                if(is_left(n)) n->parent->left = nullptr;
                else n->parent->right = nullptr;
            }
            Node* i = n->parent; //get an index
            while(i){
                update_height(i);
                if(balance_factor(i) > 1 || balance_factor(i) < -1) {
                        i = balance_tree(i, balance_factor(i));
                }
                i = i->parent;
            }
            delete n; //erase the node
            return;
        }
        //one child cases =>
        if(!n->left) {
            if(n == root){ //if node is root, update root
                root = n->right;
                n->right->parent = nullptr;
            } else { //if node is a child, update parent's children and nodes parent
                if(is_left(n)) n->parent->left = n->right;
                else n->parent->right = n->right;
                n->right->parent = n->parent;
            }
            Node* i = n->parent; //get an index
            while(i){
                update_height(i);
                if(balance_factor(i) > 1 || balance_factor(i) < -1) {
                        i = balance_tree(i, balance_factor(i));
                }
                i = i->parent;
            }
            delete n; //erase the node
            return;
        }
        if(!n->right) {
            if(n == root){ //if node is root, update root
                root = n->left;
                n->left->parent = nullptr;
            } else { //if node is a child, update parent's children and nodes parent
                if(is_left(n)) n->parent->left = n->left;
                else n->parent->right = n->left;
                n->left->parent = n->parent;
            }
            Node* i = n->parent; //get an index
            while(i){
                update_height(i);
                if(balance_factor(i) > 1 || balance_factor(i) < -1) {
                        i = balance_tree(i, balance_factor(i));
                }
                i = i->parent;
            }
            delete n; //erase the node
            return;
        }
        //two children case =>
        else {
            Node* i = n; //create an index
            i = i->right; //find successor
            while(i->left) i = i->left;
            n->data = i->data; //set nodes data to successor's data
            if(is_left(i)) i->parent->left = i->right; //update successor's parent and its child
            else i->parent->right = i->right;
            if(i->right) i->right->parent = i->parent;
            Node* ii = i->parent; //get an index
            while(ii){
                update_height(ii);
                if(balance_factor(ii) > 1 || balance_factor(ii) < -1) {
                        ii = balance_tree(ii, balance_factor(ii));
                }
                ii = ii->parent;
            }
            delete i; //erase successor node
            return;
        }
    }
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
template<class M>
void AVL_Tree<M>::display(Node* n) {
    if(!n) return;
    display(n->left);
    std::cout << n->data << ' ';
    display(n->right);
}
template <class M>
void AVL_Tree<M>::levels(const Node *n) {
    if (nullptr == n) {
        return;
    }
    int level = 0;
    typedef std::pair<const Node*,int> node_level;
    std::queue<node_level> q;
    q.push(node_level(n, 1));
    while (!q.empty()) {
        node_level nl = q.front();
        q.pop();

        n = nl.first;
        if (level != nl.second) {
            std::cout << "\nLevel " << nl.second << ": ";
            level = nl.second;
        }
        if(n){
        std::cout << "[" << n->data << ", " << n->height << "] ";
        q.push(node_level(n->left,  1 + level));
        q.push(node_level(n->right, 1 + level));
        }
        else std::cout << "[#, 0] ";
    }
    std::cout << std::endl;
}
template <class M>
int AVL_Tree<M>::max_depth(Node* n)
{
   if(!n) return 0;
   else
   {
       /* compute the depth of each subtree */
       int l_depth = max_depth(n->left);
       int r_depth = max_depth(n->right);

       /* use the larger one */
       if(l_depth > r_depth) return l_depth + 1;
       else return r_depth + 1;
   }
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

#endif // AVL_TREE
