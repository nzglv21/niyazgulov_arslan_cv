#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include "tree.h"
#include "list.h"



// void pwd(char* argv[]){
//     int f = 1;
//     char *buf=malloc(sizeof(char)*400);
//     getcwd(buf, 400);
//     if (argv[1]!=NULL){
//         if (strcmp(argv[1], ">")){
//             if ((f =open(argv[2], O_WRONLY|O_CREAT|O_APPEND, 0666))==-1){
//                 fprintf(stderr, "Error\n");
//                 exit(1);
//             }}
//         else 
//             if ((f =open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666))==-1){
//                 fprintf(stderr, "Error\n");
//                 exit(1);
//             }
//     }
//     write(f, buf, strlen(buf));
//     if (f==1)
//         printf("\n");
//     free(buf);
//     close(f);
//     exit(0);
// }

void cd(char ** lst){
	int check;
	if (lst[1] != NULL){
		check =chdir(lst[1]);
	}
	else {
		check =chdir(getenv("HOME"));
	}
	if (check == -1){
		fprintf(stderr, "Error, cd needs in 1 argument: a PATH\n");
	}		
}



void execmd(tree t, tree T){
	int f1, f2;
	if (t->backgrnd==0){
		signal(SIGINT, SIG_DFL);
	}
	else{
		signal(SIGINT, SIG_IGN);
		
	}
	if (t->infile != NULL){
		if ((f1=open(t->infile, O_RDONLY)) == -1) {
			fprintf(stderr, "Error: no %s file, or no rights\n", t->infile);
			exit(1);
		}
		dup2(f1, 0);
		close(f1);
	}

	if (t->outfile != NULL){
		if (t->doublegr == 0) {
			if  ((f2=open(t->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)  {
				fprintf(stderr, "Error");
				exit(1);

			}
		}
		else {
			if ((f2=open(t->outfile, O_WRONLY | O_CREAT | O_APPEND, 0666)) == -1)  {
				fprintf(stderr, "Error");
				exit(1);

			}
		}	
		dup2(f2, 1);
		close(f2);
	}

	if (t->backgrnd==1){
		int f3;
		if ((f3 = open("/dev/null", O_RDONLY)) == -1) {
			fprintf(stderr, "Error");
		}
		dup2(f3, 0);
		close(f3);
		if (fork()==0){
			execvp(t->argv[0], t->argv);
			clear_tree(T);
			clearlist();
			fprintf(stderr, "%s: sdfdf command not found\n", t->argv[0]);
			exit(1);
		}
		int status;
		wait(&status);
		clear_tree(T);
		clearlist();
		exit(0);
	}	
	execvp(t->argv[0], t->argv);
	fprintf(stderr, "%s: sdfdf command not found\n", t->argv[0]);
	clear_tree(T);
	clearlist();	
	exit(1);
}


tree conv(tree t, tree T){
	int fd[2], in, out, next_in, i=1, j;
	tree g=t;
	pipe (fd);
	out=fd[1];
	next_in=fd[0];
	if (fork()==0){
		close(next_in);
		dup2(out,1);
		close(out);
		execmd(g, T);
		clear_tree(T);
		clearlist();
		fprintf(stderr, "%s: command not found\n", t->argv[0]);
	}
	++i;
	in=next_in;
	g=g->pipe;
	while (g->pipe!=NULL){
		close(out);
		pipe(fd);
		out=fd[1];
		next_in=fd[0];
		if (fork()==0){
			close(next_in);
			dup2(in, 0);
			close(in);
			dup2(out, 1);
			close(out);
			execmd(g, T);
			clear_tree(T);
			clearlist();
			fprintf(stderr, "%s: command not found\n", t->argv[0]);
		}
		close(in);
		in=next_in;
		i++;
		g=g->pipe;
	}
	close(out);
	if (fork()==0){
		dup2(in,0);
		close(in);
		execmd(g, T);
		clear_tree(T);
		clearlist();
		fprintf(stderr, "%s: command not found\n", t->argv[0]);
	}
	close(in);
	for (j=1; j<=i; j++) wait(NULL);
	return g;
}






void start(tree t){
	if (t == NULL)
		return;
	tree T=t;
	while (t!=NULL){
		if (t->backgrnd == 0){
			if (t->pipe != NULL){
				t = conv(t, T);
			}
			else{
				if (fork()==0){
					execmd(t, T);}
				else 
					wait(NULL);
			}
		}
		else{
			if (fork() == 0){
				
				if (t->pipe != NULL){
					t = conv(t, T);
				}
				else{
					if (fork()==0){
						execmd(t, T);}
					else 
						wait(NULL);
				}
			clear_tree(T);
			clearlist();
			kill(getppid(), SIGUSR2);
			exit(0);
			}
		}
		t = t->next;
	}
}

