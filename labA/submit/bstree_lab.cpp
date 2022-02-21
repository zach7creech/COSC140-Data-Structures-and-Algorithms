/* Author: Zachery Creech
 * COSC140 Fall 2019
 * LabA: bstree_lab.cpp
 * This program implements the methods Depth(), Height(), Ordered_Keys(), an assignement overload
 * for copying trees (t2 = t1), and the copy constructor for the class BSTree, defined in
 * bstree_notes.cpp.
 * 11/25/2019 */

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "bstree.hpp"
using namespace std;
using CS140::BSTree;
using CS140::BSTNode;

/* ---------------------------------------------- */
/* You'll write all of these as part of your lab. */

//returns the distance that a key is from the root of the tree
int BSTree::Depth(const string &key) const
{
  BSTNode *n;
  n = sentinel->right;
  int depth = 0;
  //loops through the tree comparing keys to find the correct node, adding one to depth for each node visited, if found returns the depth
  while(1)
  {
    if(n == sentinel) return -1;
    if(n->key == key) return depth;
	if(key < n->key)
		n = n->left;
	else
		n = n->right;
	depth++;
  }
}
           
//returns the depth of the node with the maximum depth in the tree plus 1 (for the root) using a recursive function
int BSTree::Height() const
{
  return recursive_find_height(sentinel->right);
}

//returns a sorted vector of the keys in the tree by traversing it recursively
vector <string> BSTree::Ordered_Keys() const
{
  vector <string> rv;
  make_key_vector(sentinel->right, rv);
  return rv;
}
    
//copy constructor for the BSTree class, the copy is a balanced version of the original
BSTree::BSTree(const BSTree &t)        
{
  //copy constructor has to define the sentinel
  sentinel = new BSTNode;
  sentinel->key = "--SENTINEL---";
  sentinel->val = NULL; 
  sentinel->left = NULL;
  sentinel->right = sentinel;
  *this = t;
}

//operator overload for '=' operator, allows a tree structure to be set equal to another and balances the tree
BSTree& BSTree::operator= (const BSTree &t) 
{
  //current tree might have contents that need to be cleared before copying
  Clear();
  vector<string> keys = t.Ordered_Keys();
  vector<void *> vals = t.Ordered_Vals();
  //the ordered vectors are used in make_balanced_tree() to balance the tree
  sentinel->right = make_balanced_tree(keys, vals, 0, keys.size());
  size = t.size;
  return *this;
}

//recursively finds the height of the tree by visiting every node and finding the deepest node
int BSTree::recursive_find_height(const BSTNode *n) const
{
  //base case, if both left and right are the sentinel then a leaf has been reached
  if(n->right == sentinel && n->left == sentinel)
    return 1;
  
  //if the current node has two children then the function needs to first visit the left branch, then the right, then compares the heights
  if(n->left != sentinel && n->right != sentinel)
  {	
    int lheight, rheight;
	BSTNode *l, *r;
	l = n->left;
    //finds left height
	lheight = 1 + recursive_find_height(l);
	r = n->right;
	//finds right height
	rheight = 1 + recursive_find_height(r);
	if(lheight > rheight)
		return lheight;
	else
		return rheight;
  }
  
  //if the current node has no left child check the right's height
  if(n->left == sentinel)
  {
    n = n->right;
	return 1 + recursive_find_height(n);
  }
  else
  {
    n = n->left;
	return 1 + recursive_find_height(n);
  }
}

//returns a sorted vector of keys in the tree
void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const
{
  //base case is when the sentinel is reached
  if(n == sentinel) return;
  //traverses all "left" branches and pushes back the keys
  make_key_vector(n->left, v);
  v.push_back(n->key);
  //traverses all "right" branches
  make_key_vector(n->right, v);
}

//makes a balanced tree by building the tree from the left-most node to the right
BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys, 
                            const vector<void *> &vals,
                            size_t first_index,
                            size_t num_indices) const
{
  //base case is when there's only one indice left to assign, meaning its a leaf
  if(num_indices == 1)
  {
    BSTNode *leaf = new BSTNode;
	leaf->key = sorted_keys[first_index];
	leaf->val = vals[first_index];
	leaf->left = sentinel;
	leaf->right = sentinel;
	return leaf;
  }

  BSTNode *root = new BSTNode;
  size_t rootInd, subtree_indices, l_indices, r_indices;

  //formula for the current root index is the left-most node of the subtree, first_index, added to the middle of the number of indices in the subtree
  rootInd = first_index + (num_indices / 2);
  //this is how many indices are present in the current subtree
  subtree_indices = num_indices / 2; 
  //if the number of indices is even then the indices to the left is equal to half the indices (rounded down) and right has one less than left
  if(num_indices % 2 == 0)
  {	  
    l_indices = subtree_indices; //left subtree number of indices, same as parent index
    r_indices = subtree_indices - 1; //right subtree number of indices
  }
  else
  {
	//if the number of indices is odd then both left and right sides of the subtree have the same number of indices
	l_indices = subtree_indices;
	r_indices = l_indices;
  }
  
  root->key = sorted_keys[rootInd];
  root->val = vals[rootInd];

  //this if-else statment decides how to assign the right child. The right will be sentinel only if the current num_indices is even and there are only 2 left
  if((num_indices % 2 != 0) || (num_indices % 2 == 0 && num_indices != 2)) 
  {  
	BSTNode *lchild = new BSTNode;
    //this recursively finds the leftmost node in the subtree
	lchild = make_balanced_tree(sorted_keys, vals, first_index, l_indices);
    //once that left-most child node finally returns, the parent pointers are appropriately connected
	lchild->parent = root;
    root->left = lchild;
   	
	BSTNode *rchild = new BSTNode;
	//this recursively visits the right side of each root in the current subtree
	rchild = make_balanced_tree(sorted_keys, vals, rootInd + 1, r_indices);
    //once that right child returns, the parent pointers are appropriately connected
	rchild->parent = root;
	root->right = rchild;
  }
  else
  {
    //left node is still traversed and assigned as usual
	BSTNode *lchild = new BSTNode;
	lchild = make_balanced_tree(sorted_keys, vals, first_index, l_indices);
	lchild->parent = root;
	root->left = lchild;

	//in this case, there's only two more nodes to assign (including the root) so the right child must be the sentinel
	root->right = sentinel;
  }
  //once children are assigned and all of THEIR children are assigned, return the root (that is the child of another root or the sentinel)
  return root;
}
