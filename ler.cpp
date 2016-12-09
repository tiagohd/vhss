#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cctype>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <utility>
#include <algorithm>
#include <inttypes.h>
#include "ler.h"
//#include "estruturas.h"

using namespace std;
////                [109][20]
string vhdl_keyWords[] = {"abs","access","after","alias","all","and","architecture","array","assert","attribute","begin","block","body","buffer","bus","case","component","configuration","constant","disconnent","downto","else","elsif","end","end block","end case","end component","end for","end generate","end if","end loop","end process","end record","end units","entity","exit","file","for","function","generate","generic","generic map","group","guarded","if","impure","in","inertial","inout","is","label","library","linkage","literal","loop","map","mod","nand","new","next","nor","not","null","of","on","open","or","others","out","package","package body","port","port map","postponed","procedure","process","pure","range","record","register","reject","rem","report","return","rol","ror","select","severity","signal","sla","sll","sra","srl","subtype","then","to","transport","type","unaffected","units","until","use","variable","wait","when","while","with","xnor","xor"};
//char vhdl_keyWords[109][20] = {"abs","access","after","alias","all","and","architecture","array","assert","attribute","begin","block","body","buffer","bus","case","component","configuration","constant","disconnent","downto","else","elsif","end","end block","end case","end component","end for","end generate","end if","end loop","end process","end record","end units","entity","exit","file","for","function","generate","generic","generic map","group","guarded","if","impure","in","inertial","inout","is","label","library","linkage","literal","loop","map","mod","nand","new","next","nor","not","null","of","on","open","or","others","out","package","package body","port","port map","postponed","procedure","process","pure","range","record","register","reject","rem","report","return","rol","ror","select","severity","signal","sla","sll","sra","srl","subtype","then","to","transport","type","unaffected","units","until","use","variable","wait","when","while","with","xnor","xor"};
//
//// array   not  INOUT  reverse
//
////         [13][3]
//char operVHD[][3] = {"+","-","*","**","/","=","&","/=",">",">=","<","<=",":="};
//
////               [27][5]
//char operandosVHD[][5] = {"and","or","nand","nor","xor","xor","=","/=","<","<=",">",">=","sll","srl","sra","rol","ror","+","-","&","*","/","mod","rem","**","abs","not"};
//
////                         6   7   8   9   10   11   12  13  14   15 16  17   18  19   20  21
//char operBudasyn[16][3] = {"+","-","*","/","<<",">>","&","|","^","<",">","=","!","<=",">=","<:"};
//
////                   15  16  17  18  19   20
//char operRel[6][3] = {"<",">","=","!","<=",">="};
//
////                     6   7   8   9   10   11   12  13  14
//char operArit[9][3] = {"+","-","*","/","<<",">>","&","|","^"};
////                   [11][10]
string budasyn_keyWords[]= {"in","var","out","main","while","loop","endwhile","end","if","else","endif"};
//char budasyn_keyWords [][9]= {"in","var","out","main","while","loop","endwhile","end","if","else","endif"};
//

void addInOutVar ();
void except(int lin, string  aux);
char* getToken(char *s, char *t, int linha);
char*  copia(char* aux);
void caixaAlta (char *s);
void existNext(std::vector<string>token,int y, int n);
void validaString (string str, int n);
void exibeNo (std::vector<No*> l);
void expected(string fmt, ...);
void validaNum (string str, int n);
void vhdlHeader(FILE *p);
void bufferHeader (FILE *p);
void writeInOut (FILE *f, string nome);
vector <string> infixToPosfix(vector <string> infix, int linha);

vector<No*> In, Out, Var, Nx;
vector<Linha*> Lin;

/*
 * descrição: Verifica se o char lido é uma operação
 */
//bool opChar(char c)
//{
//    if (c=='+'||c=='-'||c=='*'||c == '<' || c == '>' || c == '/' || c == '|' || c == '^' || c == '&' || c == '!' || c == '=')
//        return true;
//    return false;
//}

///* alerta sobre alguma entrada esperada */
void expected(string fmt, ...)
{
    va_list args;

    fputs("Error: Line ", stderr);

    va_start(args, fmt.c_str());
    vfprintf(stderr, fmt.c_str(), args);

    va_end(args);

    fputs(" expected!\n", stderr);

    In.clear();;
    Out.clear();
    Var.clear();
    Nx.clear();
    Lin.clear();
    exit(1);
}

/*
 * descrição: Notificação de arquivo não encontrado
 */
void checkFile(FILE *file, string name)
{
    if (file == NULL)  {
        printf("%s not Found.\n", name);
        exit (1);
    }
}

/*
 *
	*/
void exibeNo (std::vector<No*> l)
{
				unsigned i;
				for(i=0; i < l.size(); i++)
					  cout<<l[i]->nome <<" ";
}	

/*
 * função que aplica o caixa baixa nos tokens
 */
void caixaAlta (char *s)
{
    int i;
    for(i=0; s[i]!='\0'; i++)
        s[i] = tolower(s[i]);
}

/*
 * inserção no início: retorna a lista atualizada
 */
char*  copia(char* aux)
{
    char* obj;
    int x;
    obj=(char*) malloc((strlen(aux)+1)*sizeof(char));
    for(x=0; x < strlen(aux) && aux[x] != '\0'; x++)
        obj[x]=aux[x];
    obj[x]='\0';
    return obj;
}

//// Notificação de erro na leitura do arquivo
void except(int lin, string  aux)
{
	   cout<<"Linha "<<lin<<" com especificacao invalida... "<<aux<<endl;
//    printf("\nLinha %d com especificacao invalida... %s\n",lin,aux);
//    printLin(lin);
    In.clear();;
    Out.clear();
    Var.clear();
    Nx.clear();
    Lin.clear();
    exit(0);
}

/*
 * cabeçalho para vhdl
 */
unsigned long long int power(int x, int y)
{
    unsigned long long int z = 1;
    while (y > 0) {
        z *= (unsigned long long int) x;
        y--;
    }
    return z;
}

/*
 * descrição:
 */
void stripFext (std::string str)
{
	   int h=0,n,l;
	   for ( l=0; l < str.size(); l++)
					   if (str[l]=='.')
									   h=l;
								
								n= l - h;
								if (h>0)
            str.erase (str.begin()+h, str.end()-n); 	
								cout<<"\n Ficou assim a bagaça: "<<str<<endl;			
}

/*
 * descrição:
 */
void stripFext (char *str)
{
	   for ( ; *str != '\0'; str++)
					   if(*str =='.')
          *str='\0';
}

/*
 * cabeçalho para vhdl
 */
void vhdlHeader(FILE *p)
{
    fprintf(p,"--------------------------------------------\n");
    fprintf(p,"-- Codigo Gerado Pela Ferramenta geraVHD  --\n");
    fprintf(p,"-- Autor: Tiago da Silva Curtinhas (2016) --\n");
    fprintf(p,"--------------------------------------------\n");
    fprintf(p,"library ieee;\n");
    fprintf(p,"use ieee.std_logic_1164.all;\n");
    fprintf(p,"use ieee.numeric_std.all;\n");
}

/*
 * cabeçalho para vhdl
 */
void bufferHeader (FILE *p)
{
    fprintf(p,"LIBRARY altera_mf;\n");
    fprintf(p,"USE altera_mf.altera_mf_components.all;\n");
}
//
///*    + soma aritmético 		    		        <  menor relacional
// *    - subtração aritmético 			 	        >  maior relacional
// *    * multiplicação aritmético 			        =  igual relacional
// *    / divisão aritmético 				           !  diferente relacional
// *   << desl. à esquerda aritmético 		        <= menor ou igual relacional
// *   >> desl. à direita aritmético 			        >= maior ou igual relacional
// *    & E bit a bit
// *    | OU bit a bit
// *    ^ OU Exclusivo bit a bit
// */
//
///*
//* estrai o token vhdl de s e atualiza a string
//*     retorna atualizada ou null
//*/
char* getToken(char *s, char *t, int linha)
{
    char c; //, aux[3];

    /* First skip over leading white spaces */
    while (*s == ' ' || *s == '\t')
        s++;
    if (*s == '\0' || *s == '\n' || *s == '\r')
        return(NULL);
    if (*s == '[' || *s == ']' || *s == '/' || *s == '*'  || *s == '+' || *s == '-' ||	*s == '=' || *s == '(' || *s == ')' ||	*s == ',' || *s == ';' || *s == '>' || *s == '<' || *s == ':' || *s == '"' || *s == 0x27 || *s == '|' || *s == '^' || *s == '&' || *s == '!') {
        c = *s;
        *t++ = *s++;
        if (( c == '<' && *s == ':') || (c == '<' && *s == '=') || (c == '>' && *s == '=') || (c== '>' && *s == '>') || (c== '<' && *s == '<'))
            *t++ = *s++;
        *t = '\0';
        return s;
    }
    while  (*s != 9 && *s != 32 && *s != '\t' && *s != '\n' && *s != '\r' && *s != '\f' && *s != '\0' && *s != 0x27 && *s != '"' && *s != '/' && *s != '*' && *s != '+' && *s != '-' && *s != ':' && *s != '<' && *s != '>' && *s != '=' && *s != '[' && *s != ']' && *s != '(' && *s != ')' && *s != '&' && *s != '|' )
    {
        if ( *s == '#' || *s == '$' || *s == '%' || *s == '.' || *s == '?' || *s == '@' || *s == 0x5C ||  *s == 0x60 || *s == '{' || *s == '}' || *s == '~' || *s > 0x7E)
        {
            printf("Erro, linha %d: Caracter Ivalido! %c\n",linha,*s);
            exit(1);
        }
        *t++ = *s++;
    }
    *t = '\0';
    return s;
}

Linha* buscaLinha(vector<Linha*> Lin, int pos )
{
	 Linha *l=NULL;
			for (int i = 0; i < Lin.size(); i++){
						if	(Lin[i]->n == pos){
									   l = Lin[i];
						}
						return l;
			}
}

/*
 *
 */
void linha1 (std::vector <std::string> token)
{

    for	(int i=0; i < token.size();i++)
        printf(" %s ", token[i].c_str());
}

/*
 *
 */
void wlinha1 (FILE *p, vector<No*> n)
{
    
    for (int i=0; i < n.size(); i++)
        fprintf(p," %s ", n[i]->nome.c_str());
}

/*
 * cabeçalho para vhdl
 */
void exibLin (vector<Linha*> Lin)
{
	   //Linha *l;
    for	(int i=0; i < Lin.size();i++){//l = Lin; l != NULL; l = l->prox) {
       //l=Lin[i];
							printf("\n Linha %3d: ",Lin[i]->n);
							//cout<<"\n Linha "<<Lin[i]->n<<": "<<endl;
        linha1(Lin[i]->token);
    }
}

/*
 * cabeçalho para vhdl
 */
void writeInOut (FILE *f, string nome)
{
    No *N;
    //unsigned long long int valor;
    fprintf(f,"ENTITY %s IS\n",nome.c_str());
    fprintf(f," PORT(\n");
    for (int i=0; i < In.size(); i++){
        //printf("l->n: %d  nome: %s \n",N->tam,N->nome);
        fprintf(f,"    %15s ",In[i]->nome.c_str());
        fprintf(f," : IN  ");
        if (In[i]->tam == 1)
            fprintf(f,"std_logic;\n");
        else {
            //valor = power(2,N->tam-1);
            fprintf(f,"SIGNED  (%2d DOWNTO 0);\n",In[i]->tam-1);  //"PRIu64"
            // fprintf(f," ";\n");
        }
    }
    for (int i=0; i < Out.size(); i++){
        //printf("l->n: %d  nome: %s \n",N->tam,N->nome);
        fprintf(f,"    %15s ",Out[i]->nome.c_str());
        fprintf(f," : OUT ");
        if(Out[i]->tam == 1) {
            if	(Out.size() != i+1)
                fprintf(f,"std_logic;\n");
            else
                fprintf(f,"std_logic);\n");
        } else {
            //valor = power(2,N->tam-1);
            fprintf(f,"SIGNED  (%2d",Out[i]->tam-1);
            if	(Out.size() != i+1)
                fprintf(f," DOWNTO 0);\n",Out[i]->tam-1);
            else
                fprintf(f," DOWNTO 0));\n",Out[i]->tam-1);
        }
    }
    fprintf(f,"END %s;\n",nome.c_str());
    printf("foi...\n");
}

/*
 * Converte de budasyn para vhdl
 */
void budasynToVhdl(char *nome)
{
    FILE *pa, *pb;
    string vv;
				std::vector<Linha*>::iterator it;
    char *word, sh[50], tlinha[MAXSTR], var[MAXSTR],*s,*t;//,aux[3];
    int pos=0,tam,li=0;
    Linha *l;
    unsigned int n,x;
				string formato = "r";
    pa = fopen (nome, formato.c_str());
    checkFile(pa, nome);
    n=0;
    while(!feof(pa))
    {
        fgets(tlinha , MAXSTR , pa );
        pos++;
        strcpy(var,tlinha);
        t = var;
        while (*t != '\r' && *t != '\n' && *t != '\0')
            t++;
        *t='\0';
        tam = strlen(var);

        if (tam > 3){
            t = strstr (var,"--");
            if(t != NULL){
                *t = '\0';
            }
            t = strstr (var,";");
            if(t != NULL){
                *t = '\0';
            }
            tam = strlen(var);
            s = var;
            t = sh;
            s=getToken(s, t, pos);

            x=0;
            while (s != NULL){
                if	(x == 0) {
                   x++;
																   l = new linha();
																   l ->n = pos;
																   l->tipo = 0;
																   Lin.push_back(l);
																			//l = Lin[li];
																			l = Lin.back();;
																			li++;
																}
																vv = sh;
																l->token.push_back(vv);
                s=getToken(s, t, pos);
                n++;
            }
        }
    }
    printf("Mostrando as linha validas e seus tokens\n");
    exibLin(Lin);
    addInOutVar ();
//    //avalExpressao()
    //l= getLinha(Lin,25);
    l= buscaLinha(Lin,25);
    if (l != NULL){
					  printf("Achou a linha: %d \n",l->n);
       infixToPosfix(l->token,l->n);
				}
    printf("\n");
    printf("Total de tokens: %u\n\n", n);
    strcpy(sh,nome);
    stripFext(sh);
    pb = fopen ("out_vhd.txt", "w");
    checkFile(pb, "out_vhd.txt");
    vhdlHeader(pb);
    bufferHeader(pb);
    writeInOut(pb,sh);
    printf("\n\nEntrou mudo e saiu calado...\n");
    fclose(pa);
//    fclose(pb);
				In.clear();;
    Out.clear();
    Var.clear();
    Nx.clear();
    Lin.clear();
}

bool buscaRW(string nome)
{
    int i;
    string v;
    printf("\n Pelo menos entrou aqui .... \n");
    for ( i=0; i < 11; i++) {
        v = budasyn_keyWords[i];
        printf(" %s",v.c_str());
        if (v == nome){
									   printf("\n");
            return true;
								}
									
    }
				printf("\n");
    //exit(1);
    return false;

}

/*
 * checa se um indentificador já foi previamente
 * declarado ou se é uma palavra reservada
 */
void checkId (int n, string nome)
{
    No *ii;
				unsigned i;
    string s;
				char caux[MAXSTR];
				
				for (i=0; i < In.size(); i++){
					  if (nome == In[i]->nome){
								  sprintf(caux," O identificacor: %s foi previamente declarado na linha: %d ...",nome.c_str(),In[i]->linha);
										s = caux;
          except(n,s);							
							}
				}
				
				for (i=0; i < Var.size(); i++){
					  if (nome == Var[i]->nome){
								  sprintf(caux," O identificacor: %s foi previamente declarado na linha: %d ...",nome.c_str(),Var[i]->linha);
										s = caux;
          except(n,s);							
							}
				}
				
				for (i=0; i < Out.size(); i++){
					  if (nome == Out[i]->nome){
								  sprintf(caux," O identificacor: %s foi previamente declarado na linha: %d ...",nome.c_str(),Out[i]->linha);
										s = caux;
          except(n,s);							
							}
				}

    if ( buscaRW(nome)) {
        sprintf(caux," A palavra reservada: %s  nao pode ser utilizada como identificador",nome.c_str());
								s = caux;
        except(n,s);
    }
}

/*
 * desmaterializa a conversão de tokens para dados
 * de entrada e saida e as variaveis
 */
void addInOutVar ()
{
    string  nn;
				No *no1;
    Linha *l;
				vector <string> n;
    int tipo=0,flag,y;
    for	(int i=0; i < Lin.size(); i++){
        y=0;
        n = Lin[i]->token;
        existNext(n,y+1, Lin[i]->n);
        nn = n[y+1];
        validaString (n[y],Lin[i]->n);
        flag = 1;
        checkId(Lin[i]->n, nn);
        if (n[y] == "in") {
            validaString (nn, Lin[i]->n);
												no1 = new No();
												no1->nome=nn;
												In.push_back(no1);
												no1 = In.back();
            flag = 0;
        } else if (n[y] == "out") {
            validaString (nn, Lin[i]->n);
												no1 = new No();
												no1->nome=nn;
												Out.push_back(no1);
												no1 = Out.back();
            flag = 0;
        } else if (n[y] == "var") {
            validaString (nn, Lin[i]->n);
												no1 = new No();
												no1->nome=nn;
												Var.push_back(no1);
												no1 = Var.back();
            flag = 0;
        } else if (n[y] == "main") {
            if  (nn != ":")  {
                //printLin(l->n);
                expected("%d, \':\'", Lin[i]->n);
            }

            if (n.size() != 2)
                except(Lin[i]->n,"indentificador main com muitos argumentos..");
            if (In.size() == 0)
                except(Lin[i]->n," Esperado ao menos uma entrada no cabecalho da especificacao..");
            if (Out.size() == 0)
               except(Lin[i]->n," Esperado ao menos uma saida no cabecalho da especificacao..");
            printf("\n\n In: ");
            exibeNo(In);
            printf("\n\n Out: ");
            exibeNo(Out);
            printf("\n\n Var: ");
            exibeNo(Var);
            printf("\n\n.. ok ...\n");
            return;
        } else if(flag)
            except(Lin[i]->n," Sentenca invalida na regiao de cabecalho..");

          y++;
										existNext(n,y+1, Lin[i]->n);
										y++;

        if (n[y] != "(" ) {
              //printLin(Lin[i]->n);
            expected("%d, \'(\'", Lin[i]->n);
        }
        existNext(n,y+1, Lin[i]->n);
        y++;
        validaNum (n[y], Lin[i]->n);
        no1->tam=atoi(n[y].c_str());
        if (no1->tam < 1 || no1->tam >64)
            except(Lin[i]->n,"esperado entre () um numero entre 1 a 64...");
        no1->tipo=tipo; // posição da entrada
        no1->linha=Lin[i]->n;
        existNext(n,y+1, Lin[i]->n);
        y++;
        if (n[y] != ")" ) {
              //printLin(l->n);
            expected("%d, \')\'", Lin[i]->n);
        }
								
								printf("n.size: %d vs %d y \n",n.size(),y);
        if(n.size() != y+1)
            except(Lin[i]->n,"6 sentenca com mais argumentos que o esperado..");
        tipo++;
    }
}

///*
// *
// */
void existNext(std::vector<string>token, int y, int n)
{
    if(token.size() == y)
        except(n," sentenca incompleta..");
}

/*
 *
 */
void printLin(int n)
{
    
    printf("\nLinha %d: ",n);
     for	(int i=0; i < Lin.size();i++)
        if(Lin[i]->n == n)
            linha1(Lin[i]->token);
    printf("\n");
}

/*
 *
 */
void validaString (string str, int n)
{
    string x;
    x = str;
				int i=0;
    //65   90   97   122
    //  A - Z     a - z
    if	(!((x[i] > 64 && x[i] < 91) || (x[i] > 96 && x[i] < 123))) {
        except(n," Os nomes de indentificadores nao pode iniciar por um numero..");
    }
    i++;

    for ( ; i < str.size(); i++) {
        if (x[i] == '-')
            x[i] = '_';
        if (!((x[i] > 64 && x[i] < 91) || (x[i] > 96 && x[i] < 122) || (x[i] > 47 && x[i] < 58) || (x[i] == '_' )))
            except(n,str);
    }
}

/*
 *
 */
void validaNum (string str, int n)
{
    for (int i=0 ; i < str.size(); i++)
        if (!(str[i] > 47 && str[i] < 58))
            except(n,"4 esperado entre () um numero entre 1 a 64...");
}

/*
 * materializa a conversão de budasyn para vhdl
 */
void bTovhdl (No *list, string nome)
{

}

////////////////////////////////////////////////////////////////
////                 uso da pilha                             //
////////////////////////////////////////////////////////////////

///*
// * A entrada é um vetor infixVect de tokens (strings) de uma expressão infixa
// *
// * Quando o  token x for um operando
// *    Inclui-lo no final do vetor postfixVect de tokens (strings) que armazena a expressão pós-fixa correspondente
// *
// * Quando o token x for um parênteses ou um operador
// *
// *    Se o token x for “(“
// *    				 Empilhar o token x em  stackVect de tokens (strings)
// *
// *    Se o token x for “)”
// *         Repetidamente desempilhar um token y de stackVect e adiciona-lo ao vetor postfixVect até encontrar  “(“ no final da pilha stackVect. Desempilhar então “(“ de stackVect.
// *         Se stackVect esvaziar antes de encontrar um “(“, a expressão de entrada não é válida.
// *
// *    Se o token x for um operador
// *         Passo 1: Examinar o token y no topo de stackVect.
// *         Passo 2: Se(caso 1) stackVect  não estiver vazia e
// *                    (caso 2) y  não for “(“ e
// *                    (caso 3) y for um operador de precedência igual ou maior do que a de x,
// *                             desempilhar o token y de stackVect,
// *                             adiciona-lo ao vetor postfixVect, e
// *																													ir para o Passo 1 novamente.
// *         Passo 3: Se (caso 1) stackVect estiver vazia ou
// *                     (caso 2) y for “(“ ou
// *   	     												(caso 3) y for um operador de precedência menor do que a de x ,
// *                              adicionar o token x ao vetor postfixVect.
// *
// *         Quando todos os tokens de infixVect tiverem sido processados,
// *         repetidamente desempilhar um token y de stackVect e adiciona-lo ao
// *									vetor postfixVect até esvaziar a pilha stackVect.
// *
// */
///*
// * Recebe dois strings.
// * O primeiro string (de entrada) representa uma expressão aritmética sob a forma infixa.
// * O segundo string serve para retornar uma expressão aritmética na forma pós-fixa.
// * O string de entrada é varrido do início até o final.
// * Se o token do string de entrada for um operando é imediatamente copiado para o string de saída. Se o token de entrada for um operador, enquanto houver operadores empilhados com precedência maior do que o operador de entrada, estes operadores que estavam empilhados são transferidos para a expressão de saída. O operador de entrada é empilhado.
// * Ao término da varredura os operadores são desempilhados e copiados para a expressão de saída. Os delimitadores não são transferidos para o string de saída.
// *
// *
// */  
//
vector <string> infixToPosfix(vector <string> infix, int linha)
{
	   vector <string> pilha, posfix,var;
			
    string aux;
	   char bl[3];
	   int x=0;

    var=infix;
    var.pop_back();
    var.pop_back();
    printf("Infixa: ");
    linha1(infix);
    printf("\n");
    printf("Infixa: ");
    linha1(var);
    printf("\n");
    exit(1);
//    for ( ; var != NULL; var = var->prox) { //
//	    x++;
//        printf("token %2d: %s\n",x,var->nome);
//        if(isOperand(var->nome)==2) { //Quando o  token for um operando
//            if(posfix == NULL)
//                posfix = iniNo(var->tam, var->nome);
//            else
//                insNo(posfix,var->nome);
//            printf("operando: %s\n",var->nome);
//
//        } else { // Quando o token for um parênteses ou um operador
//
//            if (var->nome == "(") { //Se o token x for “(“
//                pushNo(pilha,var); //Empilhar o token x
//                printf("parentese: %s\n",var->nome);
//
//            } else if (var->nome == ")") { //Se o token x for “)“
//                printf("parentese: %s\n",var->nome);
//                do {
//
//                    aux = pilhaGet(pilha);
//                    if (aux == NULL)                 // Se stackVect esvaziar antes de
//                        expected("%d, \'(\'", linha);//encontrar um “(“,
//                    //a expressão de entrada não é válida.
//                    if (aux->nome == "(") { // Desempilhar então “(“ de stackVect.
//                        aux = popNo(pilha);
//                        free(aux);
//                        aux=NULL;
//                        break;
//                    }
//
//                    insNo(posfix,aux->nome);
//                    free(popNo(pilha));
//                } while(aux != NULL);
//            }
//            //Se o token x for um operador
//            // passo1
//            else if (isOperand(var->nome) == 1) {
//                aux = pilhaGet(pilha);
//                //Passo 2:
//                if (aux->nome == "("){
//                    pushNo(pilha,aux); //Empilhar o token x
//				}else{
//	
//					while (aux != NULL) {
//						if	((aux->nome == "(")!=0) && (prcd( aux->nome, var->nome )!=2) ) {
//							aux = popNo(pilha);
//							insNo(posfix,aux->nome);
//							free(aux);
//							aux = pilhaGet(pilha);
//							continue;
//						}
//	                
//						if (aux->nome,"(")
//                           pushNo(pilha,aux); //Empilhar o token x
//					}
//				}
// 
//                if ((aux == NULL) || (aux->nome,"(") || prcd( aux->nome, var->nome )==2) {
//                    pushNo(pilha,var); // por fim desempilhar o (
//                }
//            }
//        }
//		printf("\n Rodada \n");
//		mostraP(pilha, var, posfix);
//		scanf("%s",bl);
//    }
//
//    aux = pilhaGet(pilha);
//    while(aux != NULL) {
//        aux = popNo(pilha);
//        insNo(posfix,aux->nome);
//        free(aux);
//        aux = pilhaGet(pilha);
//    }
//
//    printf("\n\nposfix: ");
//    exibeNo(posfix);
//    printf("\n");
//    except(0," Meu ovo...esquerdo");
    return posfix;
}
//
//void mostraP(pilhaNo *P, No *infix, No *posfix)
//{
//	No *l;
//
//	printf("infix: ");
//	for(l=infix; l != NULL; l=l->prox)
//		printf("%s ", l->nome);
//	printf("\n");
//
//	printf("Pilha: ");
//	for(l=P->item; l != NULL; l=l->prox)
//		printf("%s ", l->nome);
//	printf("\n");
//
//	printf("posfix: ");
//	for(l=posfix; l != NULL; l=l->prox)
//		printf("%s ", l->nome);
//	printf("\n");
//}
//
///*
// *  função: isOperand
// *  Recebe um caractere, ou “token”, e
//	*		retorna: verdadeiro (false) quando o caractere não for
// *   +, -, *, /, , (, ).
//	*  e verdadeiro quando for um operando
// */
//int isOperand (string nome)
//{
//
//    int i;
//    if	(("(" == nome) || (")" == nome))
//        return 0;
//    for(i=0; i<27; i++)
//        if	(operandosVHD[i] ==  nome)
//            return 1;
//    return 2;
//}
//
///*
// *
//	*/
//bool isDigit ()
//{
//
//    return false;
//}
//
///*
// *
//	*/
//void avalPosfix()
//{
//
//}
//
///*
// *
//	*/
//int prio (string nome)
//{
//    // menor  ----->  maior
//    //0  1  2 3 4 5 6 7
//
//    // logicos
//    if ( (nome == "and") || (nome == "or") || (nome == "nand") || (nome == "nor") || (nome == "xor") || (nome == "xnor"))
//        return 1;
//    // relacionais
//    if ((nome == "=") || (nome == "/=") || (nome == "<") || (nome == "<=") || (nome == ">") || (nome == ">="))
//        return 2;
//    // deslocamento
//    if ((nome == "sll") || (nome == "<<") || (nome == "srl") || (nome == "sla") || (nome == "sra") || (nome == "rol") || (nome == "ror"))
//        return 3;
//    // adição
//    if ((nome == "+") || (nome == "-") || (nome == "&"))
//        return 4;
//    // sinal
//    if ((nome == "+") || (nome == "-"))
//        return 5;
//    // multiplicação
//    if ((nome == "*") || (nome == "/") || (nome == "mod") || (nome == "rem"))
//        return 6;
//    // diversos
//    if ((nome == "**") || (nome == "abs") || (nome == "not"))
//        return 7;
//    // maior
//    return 0;
//}
//
///*
// *
//	*/
//void oper ()
//{
//
//}
//
///*
// * eval
// * Recebe um string que representa uma expressão infixa e faz uma varredura do início para o final do string.
//	* Se o token corrente for um operando deve ser empilhado sob a forma de real.
//	* Se o token corrente for um operador devem ser desempilhados dois operandos sobre os quais o operador vá atuar. O resultado da aplicação do operador aos operandos é empilhado.
// * Ao término da varredura do string a função retorna o topo da pilha.
// * Ler os tokens de um vetor de tokens (strings) postfixVect contendo uma expressão pós-fixa
// *
//	* Quando o token for um operando
// * Empilhá-lo em stackVect
// * Quando o token for um operador
// * Desempilhar de stackVect dois operandos
// * Aplicar o operador aos dois operandos desempilhados de stackVect obtendo um valor (novo operando)
// * Empilhar em stackVect o valor obtido da aplicação do operador aos operandos desempilhados
// * Ao término da varredura do string de postfixVect a função retorna o topo da pilha
// *
// */
//void eval()
//{
//
//
//}
//
///*
// *
// */
//int prcd (string nome1, string nome2)
//{
//    int n1,n2;
//
//    n1 = prio (nome1);
//    n2 = prio (nome2);
//
//    if (n1 > n1)
//        return 1;
//    if (n2 > n1)
//        return 2;
//    return 0;
//}
//
///*
// *
//	*/
//void orcd ()
//{
//
//
//}
//
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

/*
 * ler arq.vhdl
 */
int main (int argc, char*  argv[])  // futuro: getTokens()
{
    FILE *pa;
    string word, *pch, *sh, linha, var, s;//,aux[3];
    int pos=0,tam;
    unsigned int n;
    No *ListToken=NULL;
    if(argc != 2)
    {
        printf("\n\n\t Argumentos invalidos!\n");
        printf("\n\t Use:\n");
        printf("\n\t prog.exe arq.txt\n");
								var = ("difeq.txt");
        budasynToVhdl((char*)"difeq.txt");
        return 1;
    }
    //buscaRW("xota");
    printf("\n");
				var=argv[1];
    budasynToVhdl(argv[1]);
    return 1;
    pa = fopen (argv[1], "r");
				word=argv[1];
    checkFile(pa, word);
    n=0;
    //while(!feof(pa))
    //{
    //    fgets(linha , MAXSTR , pa );
    //    pos++;
    //    tam = strlen(linha);
    //    strcpy(var,linha);
    //    pch = var;
    //    while (*pch != '\r' && *pch != '\n' && *pch != '\0')
    //        pch++;
    //    *pch='\0';
    //    // printf("linha %2d tam: %d: %s \n",pos,tam,var);
    //    if(tam > 3)
    //    {
    //        // remove os comentários
    //        pch = strstr (var,"--");
    //        if(pch != NULL)
    //        {
    //            *pch = '\0';
    //        }
    //        tam = strlen(var);
    //        pch = var;
    //        sh = s;
    //        pch=getWord(pch, sh, pos);
    //        while (pch != NULL)
    //        {
    //            n++;
    //            word=copia(s);
    //            caixaAlta(word);
    //            if(ListToken == NULL)
    //                ListToken=iniNo(0,word);
    //            else
    //                insNo(ListToken,word);
    //            sh = s;
    //            pch=getWord(pch, sh, pos);
    //        }
    //    }
    //}
    //printf("Sem os sinal de menos\n");
    //exibeNo(ListToken);
    //printf("Sem os sinal de menos\n");
    //sinal(ListToken);
    //printf("\n");
    //printf("Total de tokens: %u\n\n", n);
    //fclose(pa);
    //liberaNo(In);
    //liberaNo(Out);
    //liberaNo(Var);
    //liberaNo(Nx);
    //liberaLinha(Lin);
				return 0;
}
