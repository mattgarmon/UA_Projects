//*****************************************************************************//
//  Matt Garmon ~ 2016                                                         //
//  BS_Tree.hpp - Declaration/Implementation of the Binary Search Tree Class   //
//*****************************************************************************//

#ifndef BS_TREE
#define BS_TREE
#include <queue>
#include <utility>
#include <iostream>


//the binary search tree class
template <class M>
class BS_Tree {
private:
    //node structure
    struct Node {
        M data; //the key of the node
        Node* left,* right,* parent; //node pointers to left right and parent
        Node(M key, Node* p) //parameterized node constructor
            : data(key), left(nullptr), right(nullptr), parent(p) {}
    };
    Node* root; //the root node of the binary search tree
    bool is_left(Node* n) {return n->parent->left == n;} //utility function
    void destroy(); //used for tree destruction
    void duplicate(Node* const &, Node* &); //used for copy construction
public:
    BS_Tree() {root = nullptr;} //constructor
    ~BS_Tree() {destroy();} //destructor
    BS_Tree(const BS_Tree &); //copy constructor
    BS_Tree &operator=(const BS_Tree &); //copy assignment operator
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


//destroy function used for tree destruction
template <class M>
void BS_Tree<M>::destroy() {
    Node* n = root;
    while(n) { //recursively erase the tree
        if(n->left) {n = n->left; continue;}
        if(n->right) {n = n->right; continue;}

            Node* p = n->parent;
            if(p) {
                if(is_left(n)) p->left = nullptr;
                else p->right = nullptr;
            }
            delete n;
            n = p;
            if(!n) break;
    }
}

//duplicate function used for copy construction
template <class M>
void BS_Tree<M>::duplicate(Node* const &one, Node* &two) {
    if(!one) two = nullptr;
    else { //recursively duplicate the tree
        two = new Node(one->data, one->parent);
        duplicate(one->left, two->left);
        duplicate(one->right, two->right);
    }
}

//copy constructor
template <class M>
BS_Tree<M>::BS_Tree(const BS_Tree &b) {
    if(!b.root) root = nullptr; //update root
    else duplicate(b.root, this->root); //call duplicate function
}

//copy assignment operator
template <class M>
BS_Tree<M> &BS_Tree<M>::operator=(const BS_Tree &b) {
    if(!b.root) root = nullptr; //update root
    else { //destroy current tree and duplicate source tree
        this->~BS_Tree();
        duplicate(b.root, this->root);
    }
}

//function to find a key and return a pointer => O()
template <class M>
typename BS_Tree<M>::Node* BS_Tree<M>::find(M key) {
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
void BS_Tree<M>::insert(M key) {
    if(!root) { //if no tree, make new node the root
        root = new Node(key, nullptr);
        return;
    }
    Node* i = root; //create an index
    while(true) { //find insertion point and insert new node
        if(i->data > key) {
            if(!i->left) {
                i->left = new Node(key, i);
                return;
            }
            else i = i->left;
        }
        if(i->data <= key) {
            if(!i->right) {
                i->right = new Node(key, i);
                return;
            }
            else i = i->right;
        }
    }
}

//Function to erase a node => best = O(1), worst = O(logn)
template <class M>
void BS_Tree<M>::erase(Node* n) {
    if(n) {
        //no children case => O(1)
        if(!n->left && !n->right) {
            if(root == n) root = nullptr; //if node is root, make root null
            else { //if node is a child, update parent's children
                if(is_left(n)) n->parent->left = nullptr;
                else n->parent->right = nullptr;
            }
            delete n; //erase the node
            return;
        }
        //one child cases => O(1)
        if(!n->left) {
            if(n == root){ //if node is root, update root
                root = n->right;
                n->right->parent = nullptr;
            } else { //if node is a child, update parent's children and nodes parent
                if(is_left(n)) n->parent->left = n->right;
                else n->parent->right = n->right;
                n->right->parent = n->parent;
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
            delete n; //erase the node
            return;
        }
        //two children case => O(log n)
        else {
            Node* i = n; //create an index
            i = i->right; //find successor
            while(i->left) i = i->left;
            n->data = i->data; //set nodes data to successor's data
            if(is_left(i)) i->parent->left = i->right; //update successor's parent and its child
            else i->parent->right = i->right;
            if(i->right) i->right->parent = i->parent;
            delete i; //erase successor node
            return;
        }
    }
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
template<class M>
void BS_Tree<M>::display(Node* n) {
    if(!n) return;
    display(n->left);
    std::cout << n->data << ' ';
    display(n->right);
}
template <class M>
void BS_Tree<M>::levels(const Node *n) {
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
        std::cout << n->data<< ' ';
        q.push(node_level(n->left,  1 + level));
        q.push(node_level(n->right, 1 + level));
        }
        else std::cout << "#" << ' ';
    }
    std::cout << std::endl;
}

template <class M>
int BS_Tree<M>::max_depth(Node* n)
{
   if(!n) return 0;
   else
   {
       /* compute the depth of each subtree */
       int l_depth = 0;
       if(n->left) l_depth = max_depth(n->left);
       int r_depth = 0;
       if(n->right) r_depth = max_depth(n->right);

       /* use the larger one */
       if(l_depth > r_depth) return l_depth + 1;
       else return r_depth + 1;
   }
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

#endif // BS_TREE
