#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
#define SIZE 16

int c; /*текущий символ */
char ** lst; /* список слов (в виде массива)*/
char * buf; /* буфер для накопления текущего слова*/ 
int sizebuf; /* размер буфера текущего слова*/
int sizelist; /* размер списка слов*/
int curbuf; /* индекс текущего символа в буфере*/ 
int curlist; /* индекс текущего слова в списке*/

typedef char **list;
void clearlist(){
    int i;
    sizelist=0;
    curlist=0;
    if (lst==NULL) return;
    for (i=0; lst[i] != NULL; i++)
        free(lst[i]);
    free(lst);
    lst = NULL;
}

void null_list(){
    sizelist = 0;
    curlist=0;
    lst=NULL;
}

void termlist(){
    if (lst==NULL) return;
    if (curlist>sizelist-1)
        lst = realloc(lst, (sizelist+1)*sizeof(*lst));
    lst[curlist] = NULL;
    lst = realloc(lst, (sizelist=curlist+1)*sizeof(*lst));
    
}

void nullbuf(){
    buf = NULL;
    sizebuf = 0;
    curbuf = 0;
}

void addsym(){
    if (curbuf>sizebuf-1)
        buf = realloc(buf, sizebuf+=SIZE);
    
    buf[curbuf++]=c;
}

void addword(){
    if (curbuf>sizebuf-1)
        buf=realloc(buf, sizebuf+=1);
    buf[curbuf++] = '\0';

    buf = realloc(buf, sizebuf=curbuf);

    if (curlist>sizelist-1)
        lst = realloc(lst, (sizelist+=SIZE)*sizeof(*lst));

    lst[curlist++] = buf;
}

void printlist(){
    int i;
    if (lst == NULL) return;
    for (i=0;lst[i] != NULL; i++)
        printf("%s\n", lst[i]);
}

void replace_spec_names(){
    int i;
    char* p;
    if (lst==NULL) return;
    for (i=0;lst[i]!=NULL; ++i)
        if (strcmp(lst[i], "$HOME")==0){
            free(lst[i]);
            p = getenv("HOME");
            lst[i]=calloc(1, sizeof(char)*(strlen(p)+1));
            strcpy(lst[i], p);
        }
        else if (strcmp(lst[i], "$USER")==0){
            free(lst[i]);
            struct passwd *pw =getpwuid(getuid());
            lst[i]= pw->pw_name;            
        }
        else if (strcmp(lst[i], "$EUID")==0){
            free(lst[i]);
            int euid = getuid();
            char *euidstr=calloc(12, sizeof(char));
            snprintf(euidstr, 12, "%d", euid);
            lst[i] = euidstr;
        }
        else if (strcmp(lst[i], "$SHELL")==0){
            free(lst[i]);
            p=calloc(1, sizeof(char)*256);
            realpath("/proc/self/exe", p);
            lst[i]=calloc(1, sizeof(char)*(strlen(p)+1));
            strcpy(lst[i], p);
            free(p);
        }
}

int symset(int c){
    return c!='\n' &&
            c!= ' ' &&
            c!= '\t' &&
            c!= EOF &&
            c!= '>' &&
            c!= '<' &&
            c!= ';' &&
            c!= ',' &&
            c!= '&' &&
            c!= '|' &&
            c!= '(' &&
            c!= ')';
}

int valid_sym(int c){
    return c!='+' &&
            c!='%' &&
            c!='@' &&
            c!='!' &&
            c!='^' &&
            c!='?' &&
            c!='*' &&
            c!=':';
}


void clear_buf(){
    int n;
    
    for (n=getchar();n!=EOF && n!='\n';n=getchar());
}

int make_list(){
    typedef enum {Start, Word, Greater, Greater2, Or, Or2,
            Amper, Amper2, Less, Semicolon, Bracket,
             Newline, Stop, Marks, Comment} Vertex;

    c = getchar();
    if (c==EOF){
        kill(getpid(), SIGUSR1);}
    Vertex V=Start;
    null_list();

    lst=NULL;
    while (1){
        switch(V){
        case Start:
            if (c==' ' || c=='\t')
                c=getchar();
            else if (c == EOF){
                printf("%d\n",lst==NULL);
                termlist();
                replace_spec_names();
                return 0;
            }
            else if (c=='\n'){
                termlist();
                replace_spec_names();

                return 1;
            }
            else if (c=='\"'){
                V=Marks;
                nullbuf();
                c=getchar();
            }
            else if (c=='#'){
                V=Comment;
            }
            else{
                nullbuf();
                addsym();
                if (c=='|') V=Or;
                else if (c==';') V=Semicolon;
                else if (c==')' || c== '(') V=Bracket;
                else if (c=='<') V=Less;
                else if (c=='>') V=Greater;
                else if (c=='&') V=Amper;
                else V=Word;
                c=getchar();
            }
            break;
        case Word:
            if (symset(c)){
                addsym();
                c=getchar();
            }
            else {
                V=Start;
                addword();
            }
        break;

        case Greater:
            if (c=='>'){
                addsym();
                c=getchar();
                V=Greater2;
            }
            else{
                V=Start;
                addword();
            }
        break;

        case Or:
            if (c=='|'){
                addsym();
                c=getchar();
                V= Or2;
            }
            else{
                V=Start;
                addword();
            }
        break;

        case Amper:
            if (c=='&'){
                addsym();
                c=getchar();
                V = Amper2;
            }
            else{
                V=Start;
                addword();
            }
        break;

        case Marks:
            if (c!='\"'){
                addsym();
                c=getchar();
            }
            else{
                V=Start;
                addword();
                c=getchar();
            }
            break;

        case Greater2:
        case Amper2:
        case Or2:
            V = Start;
            addword();
        break;

        case Semicolon:
        case Less:
        case Bracket:
            V=Start;
            addword();
        break;

        case Newline:
            clearlist();
            V=Start;
        break;

        case Comment:
            while ((c=getchar()), (c!=EOF) && (c!='\n'));
            V=Start;
        break;

        case Stop:
        return 0;
        }
    }


}

int set(char* w){
	return (strcmp(w, "|")!=0) &&
	       (strcmp(w, "&")!=0)  &&
	       (strcmp(w, "<")!=0) && 
	       (strcmp(w, ">")!=0) &&
	       (strcmp(w, ">>")!=0)&&
           (strcmp(w, "||")!=0) &&
           (strcmp(w, ";")!=0);
        
}

void popular_plex(list l){
    int max_count=0;
    char* p=NULL;
    for (int i=0; l[i]!= NULL;i++){
        if (set(l[i]) == 0)
            continue;
        int count_i = 0;
        for (int j=0;l[j]!=NULL; j++){
            if (strcmp(l[i], l[j]) == 0)
                count_i++;
        }
        if (count_i>max_count){
            p = l[i];
            max_count =count_i;
        }
    }

    if (p!= NULL)
        printf("most popular is: %s, count = %d\n", p,max_count);
}