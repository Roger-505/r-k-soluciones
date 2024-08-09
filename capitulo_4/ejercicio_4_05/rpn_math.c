#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define BUFSIZE 100
#define MAXOP 100
#define MAXVAL 100
#define NUMBER '0'

int getop(char s[]);
void push(double);
double pop(void);
int getch(void);
void ungetch(int);

void ptop(void);    // print top element of the stack
void dtop(void);    // duplicate top element of the stack
void stop(void);    // swap top two elements of the stack
void clear(void);   // clear the stack

int sp = 0;
int bufp = 0;
double val[MAXVAL];
char buf[BUFSIZE];

int main(void){
    int type;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF){
        switch (type){
            case NUMBER:
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                op2 = pop();
                if (op2 != 0.0)
                    push(pop() / op2);
                else
                    printf("error: zero division\n");
                break;
            case '\n':
                printf("result: %.8g\n", pop());
                break;
            case 'c':
                clear();
                break;
            case 'p':
                ptop();
                break;
            case 'd':
                dtop();
                break;
            case 's':
                stop();
                break;
            case 'S':
                push(sin(pop()));
                break;
            case 'E':
                push(exp(pop()));
                break;
            case 'P':
                op2 = pop();
                push(pow(pop(), op2));
                break;
            case '%':
                op2 = pop();
                if (op2 != 0.0)
                    push((int)(pop()) % (int)(op2));
                else
                    printf("error: zero division\n");
                break;
            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }
    return 0;
}

int getop(char s[]){
    int i = 0, c;

    while((s[0] = c = getch()) == ' ' || c == '\t');

    s[1] = '\0';
    if (!isdigit(c) && c != '.' && c != '-')
        return c;
    
    if (c == '-'){
        int next = getch();
        
        if (next == ' ' || next == '\t' || next == '\n'){
            ungetch(next);
            return c;
       }
        else if (!isdigit(next) && next != '.'){
            return next;
        }
        else{
            s[++i] = c = next;
        }
    }

    if (isdigit(c))
        while (isdigit(s[++i] = c = getch()));
    
    if (c == '.')
        while (isdigit(s[++i] = c = getch()));

    s[i] = '\0';
    if (c != EOF)
        ungetch(c);
    
    return NUMBER;
}

int getch(void){
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c){
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else 
        buf[bufp++] = c;
}

void push(double f){
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}

double pop(void){
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

void ptop(void){
    if (sp > 0)
        printf("stack top: %g\n", val[sp - 1]);
    else
        printf("error: stack empty\n");
}

void dtop(void){
    int temp = pop();

    push(temp);
    push(temp);
}

void stop(void){
    int temp1 = pop();
    int temp2 = pop();

    push(temp1);
    push(temp2);
}

void clear(void){
    while (sp > 0)
        val[--sp] = 0.0;
}

// gcc rpn_math.c -lm
//./a.out < txt