#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "mylib.h"
#include "tree.h"

static void helpstatements(char *program){
    fprintf(stderr, "Invalid input: %s\n"
            " -c filename, Please ensure you have spelt the filename correctly \n"
            "    Please make sure it uses words from the stdin dictionary \n"
            "    All unknown words are printed out for you \n"
            "    Timing and the unknown word count are printed out for you \n"
            " -d Prints the tree depth ", program);
    fprintf(stderr,
            " -f filename, Give the 'dot output' not the default filename when \n"
            "    -o is given \n"
            "-o  Output an tree repersentation using the dot notations. \n"
            "-r  Change the standard bst tree to a rbt tree \n"
            "-h  Help message describing how to use the program \n");

}

static void print_info(int freq, char *word){
  printf("%-4d %s\n", freq, word);
}

int main(int argc, char **argv){
    /* Setting all the variables */
    int spellcheck, rbt, outputdot, depth, unknownCount;
    FILE *infile, *outfile;
    char word[256];
    char option;
    clock_t start_f, start_s, end_f, end_s;
    tree t;
    type_t tree_type;
    
    /*defines all the possible options*/
    const char *opstring = "c:df:orh";

    /* Initalising all the variables */
    spellcheck = 0;
    rbt = 0;
    outputdot = 0;
    depth = 0;
    unknownCount = 0;
    infile = NULL;
    outfile = (FILE *) "output-dot.txt";
    t = NULL;
    tree_type = BST;

    /*while loop which checks what command line option has been given */
    while((option =  getopt(argc, argv, opstring)) != EOF) {

        /* Switch statements to filter the options and
           provide the correct option */

        switch(option){
            case 'c':
	      infile = (FILE *) optarg;
	      break;
	    case 'd':
	      depth = 1;
	      break;
	    case 'f':
	      outfile = (FILE *) optarg;
	      break;
	    case 'o':
	      outputdot = 1;
	      break;
	    case 'r':
	      tree_type = RBT;
	      break;
	    case 'h':
	      helpstatements("h");
	      break;
	  
        }
    }

    t = tree_new(tree_type);

    while(getword(word, sizeof word, stdin) != EOF){
      tree_insert(t, word);
    }

    /* for each word read from infile, check to see if it's in the
     * dictionary created above. 
     * If it is then don't do anything.
     * If it isn't, then print the word to stdout. 
     *
     * When finished checking infile, print timing information 
     * and unknownCount to stderr as shown in asgn2. 
     */
    if (infile != NULL){
      while(getword(word, sizeof word, infile) != EOF){
	if (tree_search(t, word) == 0){
	  fprintf(stdout, "%s\n", word);
	}
      }
    }else{
      if (depth == 1){
	fprintf(stdout, "%d\n", tree_depth(t));
      }
      if (outputdot == 1){
	tree_output_dot(t, outfile);
      }
    }

    tree_free(t);
    return EXIT_SUCCESS;
}
