#ifndef TREE_H_
#define TREE_H_

typedef struct treenode *tree;
typedef enum tree_e { BST, RBT } type_t;  

extern tree tree_free(tree b);
extern void tree_inorder(tree b, void f(char *col, char *str));
extern tree tree_insert(tree b, char *str);
extern tree tree_new(type_t tree_type);
extern void tree_preorder(tree b, void f(int frequency, char *str));
extern int tree_search(tree b, char *str);
extern int tree_depth(tree b);
extern void tree_output_dot(tree t, FILE *out);
static void tree_output_dot_aux(tree t, FILE *out);


#endif
