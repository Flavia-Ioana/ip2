#include <iostream>
#include <fstream>
#include <cstring>
#include <graphics.h>

using namespace std;

ifstream fin("cod.in");

struct nod
{
    int info;
    nod *urm;
};

struct coordonate
{
    int x1, y1, x2, y2;// coordonatele dreptunghiului
    int i; //informatia dreptunghiului
}C[1001];

nod *L[1001];

char text[202];
char cuv[1000][202];
int n;
int a[200][200];
int S[1001]; ///stiva
int top;
bool viz[202];

void Spatii(char s[202])
{
    int i = 0;
    while(s[i] == ' ' && s[i] != 0)
        strcpy(s + i, s + i + 1);
}
bool read(char s[202])
{
    if(strncmp(s, "citeste", 7) == 0) return 1;
    return 0;
}

bool switchClause(char s[202])
{
    if(strncmp(s, "switch", 6) == 0) return 1;
    return 0;
}

bool forClause(char s[202])
{
    if(strncmp(s, "pentru", 6) == 0) return 1;
    return 0;
}

bool dowhileClause(char s[202])
{
    if(strncmp(s, "repeta", 6) == 0) return 1;
    return 0;
}

bool whileClause(char s[202])
{
    if(strncmp(s, "cat_timp", 8) == 0)return 1;
    return 0;
}

int ifClause(char s[202])
{
    if(strncmp(s, "daca", 4) == 0)return 1;
    return 0;
}

int elseClause(char s[202])
{
    if(strncmp(s, "altfel", 6) == 0)return 1;
    return 0;
}

int endifClause(char s[202])
{
    if(strncmp(s, "sfarsit_daca", 12) == 0) return 1;
    return 0;
}

int endforClause(char s[202])
{
    if(strncmp(s, "sfarsit_pentru", 14) == 0) return 1;
    return 0;
}

int endwhileClause(char s[202])
{
    if(strncmp(s, "sfarsit_cat_timp", 16) == 0) return 1;
    return 0;
}

void Inserare(nod *&prim, int x)
{
    nod * curent = new nod;
    curent->info = x;
    curent->urm = NULL;
    if(prim == NULL) prim = curent;
    else
    {
        nod *p = prim;
        while(p -> urm != NULL)
            p = p ->urm;
        p->urm = curent;
    }
}

void Construire(int i, int &x1, int &y1, int &x2, int &y2)
{
    if(ifClause(cuv[i]) == 1)
    {
        rectangle(x1, y1, x2, y2);
        int mij = (x1+x2)/2;
        line(x1, y1, mij, y2);
        line(mij, y2, x2, y1);
        outtextxy((x1+x2)/2-textwidth(cuv[i])/2, y1+10, cuv[i]);
        outtextxy(x1 + 10, (y1+y2)/2 - textheight("DA")/2, "DA");
        outtextxy(x2 - textwidth("NU") - 10, (y1+y2)/2-textheight("NU")/2, "NU");
        y1 = y2, y2 +=100;
    }
    else
    {
        rectangle(x1, y1, x2, y2);
        outtextxy((x1+x2)/2-textwidth(cuv[i])/2,(y1+y2)/2, cuv[i]);
        y1 = y2, y2 +=100;
    }
}

void DFS(int n, int x1, int y1, int x2, int y2)
{
    viz[n] = 1;
    Construire(n, x1, y1, x2, y2);
    for(nod *p = L[n]; p != 0; p = p->urm)
          if(viz[p->info] == 0)
               DFS(p->info, x1, y1, x2, y2);
}

int main()
{
    initwindow(getmaxwidth(), getmaxheight(), "Nassi");
    int x1 = 0,y1 = 0, x2 =  300, y2 = 100;
    int i;
    while(fin.getline(text, 202))
       {
           Spatii(text);
           strcpy(cuv[++n], text);
       }

    //presupunem ca codul este scris corect
    //ne folosim de o stiva pentru a pune daca, pentru, cat timp, repeta etc si atasam la lista de adiacenta a lui L[S[top]]]
    //pozitia propozitiei curente
    //daca avem sfarsit daca, sfarsit_pentru etc atasam se elimina din stiva
    Inserare(L[1], 1);
    for(i = 2; i <= n; i++)
    {
        if(top == 0)
             {
               Inserare(L[1], i); //(adaug la vecinii nodului radacina care e Line[1])
               S[++top] = i;
             }
        else if(top != 0)
          {
            if(ifClause(cuv[i]) == 1 || elseClause(cuv[i]) == 1 || forClause(cuv[i]) == 1 || whileClause(cuv[i]) == 1 || dowhileClause(cuv[i]) == 1)
             {
                Inserare(L[S[top]], i); //(adaug ca vecin la Line[S[top]])
                S[++top] = i;

             }
            else if(endifClause(cuv[i]) == 1 || endforClause(cuv[i]) == 1 || endwhileClause(cuv[i]) == 1 || strncmp(cuv[i], "pana_cand", 9) == 0)
                      {
                          top--;
                          if(elseClause(cuv[S[top]]) == 1) top--;
                      }
                 else {
                         Inserare(L[S[top]], i); //(adaug ca vecin la Line[S[top]])
                         S[++top] = i;
                      }
          }
     }


    DFS(1, x1, y1, x2, y2);

    for(int i = 1; i <= n; i++)
    {
        for(nod *p = L[i]; p != 0; p=p->urm)
            cout << cuv[p->info] << "    ";
        cout << "\n";
    }
    //acum avem arborele ca lista simpla inlantuita---> construim tabelul in functie de relatii
    //ca mai jos
    //folosind DFS
    /*
    //grafica
    for(int i = 1; i <= n; i++)
        if(simpleClause(cuv[i]) == 1)
             {
                rectangle(x1, y1, x2, y2);
                outtextxy((x1+x2)/2-textwidth(cuv[i])/2,(y1+y2)/2, cuv[i]);
                y1 = y2, y2 +=100;
             }
        else if(ifClause(cuv[i]) == 1)
             {
                 line(x1,y1, x2, y1);
                 int mij = (x1+x2)/2;
                 line(x1, y1, mij, y2);
                 line(mij, y2, x2, y1);
                 y1 = y2, y2 +=100;
             }*/

    getch();
    closegraph();

    return 0;
}
