#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "list.h"

extern char** lst;

struct cmd_inf {
	char ** argv; // список из имени команды и аргументов
	char *infile; // переназначенный файл стандартного ввода
	char *outfile; // переназначенный файл стандартного вывода
	int doublegr; //>>
	int backgrnd; // =1, если команда подлежит выполнению в фоновом режиме cmd_inf* psubcmd; // команды для запуска в дочернем shell
	struct cmd_inf* pipe; // следующая команда после “|”
	struct cmd_inf* next; // следующая после “;” (или после “&&”)
};

typedef struct cmd_inf node;
typedef struct cmd_inf* tree;
char *plex;

tree make_cmd(){
	tree t=calloc(1, sizeof(node));
	t -> argv = NULL;
	t -> infile = NULL;
	t -> outfile = NULL;
	t -> doublegr = 0;
	t -> backgrnd = 0;
	t -> pipe = NULL;
	t -> next = NULL;
	return t;
}


//
void make_shift(int n){
    while(n--)
        putc(' ', stderr);
}

void print_argv(char **p, int shift){
    char **q=p;
    if(p!=NULL){
        while(*p!=NULL){
             make_shift(shift);
             fprintf(stderr, "argv[%d]=%s\n",(int) (p-q), *p);
             p++;
        }
    }
}

void print_tree(tree t, int shift){
    char **p;
    if(t==NULL)
        return;
    p=t->argv;
    if(p!=NULL)
        print_argv(p, shift);
    else{
        make_shift(shift);
        fprintf(stderr, "psubshell\n");
    }
    make_shift(shift);
    if(t->infile==NULL)
        fprintf(stderr, "infile=NULL\n");
    else
        fprintf(stderr, "infile=%s\n", t->infile);
    make_shift(shift);
    if(t->outfile==NULL)
        fprintf(stderr, "outfile=NULL\n");
    else
        fprintf(stderr, "outfile=%s\n", t->outfile);
    make_shift(shift);
    fprintf(stderr, "append=%d\n", t->doublegr);
    make_shift(shift);
    fprintf(stderr, "background=%d\n", t->backgrnd);
    make_shift(shift);
    make_shift(shift);
    if(t->pipe==NULL)
        fprintf(stderr, "pipe=NULL \n");
    else{
        fprintf(stderr, "pipe---> \n");
        print_tree(t->pipe, shift+5);
    }
    make_shift(shift);
    if(t->next==NULL)
        fprintf(stderr, "next=NULL \n");
    else{
        fprintf(stderr, "next---> \n");
        print_tree(t->next, shift+5);
    }
}
//

int wset(char* w){
	return (strcmp(w, "|")!=0) &&
	       (strcmp(w, "&")!=0)  &&
	       (strcmp(w, "<")!=0) && 
	       (strcmp(w, ">")!=0) &&
	       (strcmp(w, ">>")!=0) ;
}


void make_bgrnd(tree t){
    while (t!=NULL){
        t->backgrnd = 1;
        t = t->pipe;
    }
}

void add_arg(tree t){
	
	if (t->argv == NULL){
		t->argv = calloc(1, sizeof(* t->argv));
	}
	int i=0;	
	while (t->argv[i]) i++;

	
	t->argv[i]=plex;
	t->argv = realloc(t->argv, ((i+2) * sizeof(*t->argv)));
	t->argv[i+1]=NULL;
}

tree clear_tree(tree t){
    if (t==NULL) return NULL;

    clear_tree(t->next);
    clear_tree(t->pipe);
    free(t->argv);
    free(t);
    return NULL;
}

tree build_tree(){
	typedef enum{ begin, conv, conv1, in, in1, out, out1,
		end, backgrnd, err, comma, nextback} vertex;
	
	vertex v= begin;
    int c_flag = 0;
	int i=0;

	if (lst==NULL) return NULL;

	tree t, c, p, z;
	while(1){
        switch(v){
		case begin:
			z=t=c=make_cmd();
			plex=lst[i];
			add_arg(t);
			i++;
			plex=lst[i];
			if (plex!=NULL){
                p=c;
				v=conv;}
			else
				v=end;
        break;

        case conv:
            if (strcmp(plex, "|")==0)
                v=conv1;
            else if (strcmp(plex, "<")==0)
                v=in;
            else if (strcmp(plex, "&")==0)
                v=backgrnd;
            else if (strcmp(plex, ">")==0 || strcmp(plex, ">>")==0)
                v=out;
            else if (strcmp(plex, ";") == 0)
                v = comma;
            else{
                add_arg(c);
                i++;
                plex=lst[i];
                if (plex == NULL)
                    v=end;
            }
        break;

        case comma:
             c=make_cmd();
             plex=lst[++i];
            if ((plex == NULL) || (wset(plex)==0)){
                v=err;
                break;
            }

             p->next = c;
             t=p=c;
             add_arg(c);
             plex=lst[++i];
             if (plex == NULL)
                v = end;
            else
                v=conv;
        break;


        case conv1:
            c=make_cmd();
            p->pipe = c;
            p=c;
            plex=lst[++i];
            if ((plex!=NULL) && (wset(plex)))
               { v=conv;
                }
            else
                v=err;
            
        break;



        case in:
            plex=lst[++i];
            
            if ((plex!=NULL) && (wset(plex)))
                v=in1;
            else
                v=err;
        break;

        case in1:
            c->infile = plex;
            plex=lst[++i];
            if (plex!=NULL)
                v=conv;
            else
                v=end;
        break;

        case out:
            if (strcmp(plex, ">>")==0)
                c->doublegr = 1;
            plex=lst[++i];
            if ((plex!=NULL) && (wset(plex)))
                v=out1;
            else
                v=err;
            
        break;

        case out1:
            c->outfile=plex;
            plex=lst[++i];
            if (plex!=NULL)
                v=conv;
            else
                v=end;
        break;

        case backgrnd:
				make_bgrnd(t);
				plex=lst[++i];
				if (plex == NULL){
					v=end;
				} else if (wset(plex)==0) 
                    v=err;
				else 
                v=nextback;
				
			break;

			case nextback:
				c=make_cmd();

				p->next = c;
				t=p=c;
				add_arg(c);

				plex=lst[++i];
				if (plex==NULL) v=end;
				else v=conv;
			break;

        case err:
            clear_tree(z);
            fprintf(stderr, "Error in building tree: %s\n", plex);
            return NULL;
        break;

        case end:
            return z;
	}
	}
}