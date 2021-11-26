#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "tree.h"
#include "mylib.h"

typedef enum { RED, BLACK } tree_colour;
#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

static type_t tree_type;
/*!
 * @struct treenode
 * @abstract Contains all of the tree's properties.
 *
 * @field *key Pointer to key.
 * @field colour Colour of the node for RBT.
 * @field left The subtree to the left of the node.
 * @field right The subtree to the right of the node. 
 * @field tree_type The type of the tree either BST or RBT.
 * @field freq The frequency of same keys.
*/
struct treenode {
  char *key;
  tree_colour colour;
  tree left;
  tree right;
  int frequency;
};

/*!
 * @function tree_new
 * @abstract Creates a new tree.
 *
 * @param tree_type Either BST or RBT.
 * 
 * @result The new tree created. 
*/
tree tree_new(type_t type) {
  tree b = emalloc(sizeof *b);
  tree_type = type;
  b->frequency = 0;
  b->colour = BLACK;
  return b;
}

/*!
 * @function tree_free
 * @abstract Frees the memory allocated to the tree.
 *
 * @param b The tree.
 *
 * @return The freed tree.
*/
tree tree_free(tree b) {
    /* Is this right? */
    
    if (b == NULL) {
        return b;
    } else {
        tree_free(b->right);
        tree_free(b->left);
        free(b->key);
        free(b);
    }

    return b;
}

/*!
 * @function right_rotate
 * @abstract Performs a right rotation on the tree.
 *
 * @param b The tree.
 * 
 * @result The right-rotated tree.
*/
static tree right_rotate(tree b) {

    /* Make a temp rbt to keep track of original root. */
    tree temp = emalloc(sizeof b);
    temp->key = emalloc(sizeof b->key);
    temp = b;
    /* strcpy(temp->key, b->key); Is this line necessary? */

    /* Right rotation. */
    b = b->left;
    temp->left = b->right;
    b->right = temp; /* Should this be "b->right = temp->key" ? */
    
    return b;

}

/*!
 * @function left_rotate
 * @abtract Performs a left rotation on the tree.
 *
 * @param b The tree.
 *
 * @result The left-rotated tree.
*/
static tree left_rotate(tree b) {

    /* Make a temp rbt to keep track of original root. */
    tree temp = emalloc(sizeof *b);
    temp->key = emalloc(strlen(b->key) + 1 * sizeof(char));
    temp = b;
    /* strcpy(temp->key, b->key); Is this line necessary? */

    /* Left rotation. */
    b = b->right;
    temp->right = b->left;
    b->left = temp; /* Should this be "b->right = temp->key" ? */
    
    return b;

}

/*!
 * @function tree_fix
 * @abstract Fixes the colouring on the tree afer an insert if it is an RBT.
 *
 * @param b The tree.
 *
 * @result The colour-corrected tree.
*/
static tree tree_fix(tree b) {

    if (b == NULL) {
        /* Stopping case. */
        return b;
    }

    if (IS_RED(b->left) && IS_RED((b->left)->left)) {
        if (IS_RED(b->right)) {
            /* If auntie red. */
            b->colour = RED;
            (b->left)->colour = BLACK;
            (b->right)->colour = BLACK;
        } else {
            /* If auntie black. */
            b = right_rotate(b);
            b->colour = BLACK;
            (b->right)->colour = RED;
        }
            
        
    } else if (IS_RED(b->left) && IS_RED((b->left)->right)) {
        
        if (IS_RED(b->right)) {
            /* If auntie red. */
            b->colour = RED;
            (b->left)->colour = BLACK;
            (b->right)->colour = BLACK;
        } else {
            /* If auntie black. */
            b->left = left_rotate(b->left);
            b = right_rotate(b);
            b->colour = BLACK;
            (b->right)->colour = RED;
        }
           
        
    } else if (IS_RED(b->right) && IS_RED((b->right)->left)) {

        if (IS_RED(b->left)) {
            /* If auntie red. */
            b->colour = RED;
            (b->left)->colour = BLACK;
            (b->right)->colour = BLACK;
        } else {
            /* If auntie black. */
            b->right = right_rotate(b->right);
            b = left_rotate(b);
            b->colour = BLACK;
            (b->left)->colour = RED;
        }
           
    } else if (IS_RED(b->right) && IS_RED((b->right)->right)) {

        if (IS_RED(b->left)) {
            /* If auntie red. */
            b->colour = RED;
            (b->left)->colour = BLACK;
            (b->right)->colour = BLACK;
        } else {
            /* If auntie black. */
            b = left_rotate(b);
            b->colour = BLACK;
            (b->left)->colour = RED;
        }
        
    }

    return b;

}

/*!
 * @function tree_search
 * @abstract Searches the tree for a given String. 1 if found, 0 if not. 
 *
 * @param b The tree.
 * @param str The String to search for.
 *
 * @return Either 1 or 0 for True or False.  
*/
int tree_search(tree b, char *str) {
    if (b == NULL) {
        return 0;
    } else if (strcmp(b->key, str) == 0) {
        /* have found str in tree */
        return 1;
    } else if (strcmp(b->key, str) > 0) {
        /* if b->key > str */
        return tree_search(b->left, str);
    } else {
        /* if b->key < str */
        return tree_search(b->right, str);
    }
}

/*!
 * @function tree_insert
 * @abstract Inserts a given String into the tree.
 * 
 * @param b The tree.
 * @param str The String the insert into the tree.
 *
 * @result The tree with the String inserted. 
*/
tree tree_insert(tree b, char *str) {
    if (b == NULL) {
        b = emalloc(sizeof *b);
        b->key = emalloc(strlen(str) + 1 * sizeof(char));
        b->right = NULL;
        b->left = NULL;
        strcpy(b->key, str);
	if (tree_type == RBT){
	  b->colour = RED;
	  /* New. Do i need to allocate memory for the colour? */
	  b = tree_fix(b);
	}/* Is this the appropriate place to call rbt_fix? */
        return b;
    } else if (strcmp(b->key, str) == 0) {
        b->frequency++;
        /* str already in tree, no duplicates */
        return b;
    }

    if (strcmp(b->key, str) > 0) {
        /* if b->key > str */
        b->left = tree_insert(b->left, str);
    } else {
        /* if b->key < str */
        b->right = tree_insert(b->right, str);
    }
    if (tree_type == RBT){
      b = tree_fix(b);
    }
    return b;
}

/*!
 * @function tree_inorder
 * @abstract Passes the colour and key in order to another function. 
 *
 * @param b The tree.
 * @param col The colour of the node.
 * @param str The key of the node.
*/
void tree_inorder(tree b, void f(char *col, char *str)) {
    if (b == NULL) {
        return;
    }
    
    tree_inorder(b->left, f);
    if (IS_BLACK(b)) {
      f("black: ", b->key);
    } else {
      f("red: ", b->key);
    }
    tree_inorder(b->right, f);
}

/*!
 * @function tree_preorder
 * @abstract Passes the colour and key in preorder to another function. 
 *
 * @param b The tree.
 * @param col The colour of the node.
 * @param str The key of the node.
*/
void tree_preorder(tree b, void f(int frequency, char *str)) {
    
    if (b == NULL) {
        return;
    }
        f(b->frequency, b->key);
    tree_preorder(b->left, f);
    tree_preorder(b->right, f);
}

/*!
 * @function tree_depth
 * @abstract Returns the deepest depth of the tree.
 *
 * @param b The tree.
 *
 * @result The longest path from top to bottom of the tree. 
*/
int tree_depth(tree b){
    int depth = 0;
    /* Checks if there is a node at the same level on each side of the tree */
    if(b->left != NULL && b->right != NULL){
        /* Increasing total tree depth */
        depth = depth + 1;
        /* Recursively call tree_depth for each side of the tree */
        return(tree_depth(b->left) && tree_depth(b->right));
    }else if(b->left != NULL){
        /* Increasing total tree depth */
        depth = depth + 1;
        /* Only calling tree_depth for the left side of the tree */
        return(tree_depth(b->left));
    }else if(b->right != NULL){
        /* Increasing total tree depth */
        depth = depth + 1;
        /* Only calling tree_depth for the left side of the tree */
        return(tree_depth(b->right));
    }
    return depth;
}

/**                                                                             
 * Output a DOT description of this tree to the given output stream.            
 * DOT is a plain text graph description language (see www.graphviz.org).       
 * You can create a viewable graph with the command                             
 *                                                                              
 *    dot -Tpdf < dotfile > dotfile.pdf                                         
 *                                                                              
 * where 'dotfile' is a file that has been written by tree_output_dot()         
 *                                                                              
 * You can also use png, ps, jpg, svg... instead of pdf                         
 *                                                                              
 * @param t the tree to output the DOT description of.                          
 * @param out the stream to write the DOT description to.                       
 */                                                                             
void tree_output_dot(tree t, FILE *out) {                                       
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");    
    tree_output_dot_aux(t, out);                                                
    fprintf(out, "}\n");                                                        
}

/**                                                                             
 * Traverses the tree writing a DOT description about connections, and          
 * possibly colours, to the given output stream.                                
 *                                                                              
 * @param t the tree to output a DOT description of.                            
 * @param out the stream to write the DOT output to.                            
 */                                                                             
static void tree_output_dot_aux(tree t, FILE *out) {                            
    if (t == NULL) {                                                            
        return;                                                                 
    }                                                                           
    if(t->key != NULL) {                                                        
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",    
                t->key, t->key, t->frequency,                                   
                (RBT == tree_type && RED == t->colour) ? "red":"black");        
    }                                                                           
    if(t->left != NULL) {                                                       
        tree_output_dot_aux(t->left, out);                                      
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);        
    }                                                                           
    if(t->right != NULL) {                                                      
        tree_output_dot_aux(t->right, out);                                     
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);       
    }                                                                           
}                                                                               
                                                                                
                        
