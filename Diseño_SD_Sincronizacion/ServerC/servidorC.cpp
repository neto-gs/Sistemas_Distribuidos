#include "Respuesta.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

// trie node 
struct TrieNode { 
    struct TrieNode *children[10]; 
    // isEndOfWord is true si el nodo representa
    // fin de cadena
    bool isEndOfWord; 
}; 
  
// Returna un nuevo nodo (initialized to NULLs) 
struct TrieNode *getNode(void) { 
    struct TrieNode *pNode =  new TrieNode; 
    pNode->isEndOfWord = false; 
  
    for (int i = 0; i < 9; i++) 
        pNode->children[i] = NULL; 
  
    return pNode; 
} 
  
// Si no existe, inserta key dentro de nodo 
// Si la key es prefijo del nodo, sólo
// marca al nodo 
void insert(struct TrieNode *root, string key) { 
    struct TrieNode *pCrawl = root; 
  
    for (int i = 0; i < key.length(); i++) { 
        int index = key[i] - '0'; 
        if (!pCrawl->children[index]) 
            pCrawl->children[index] = getNode(); 
  
        pCrawl = pCrawl->children[index]; 
    } 
  
   // marcamos el ultimo nodo  
    pCrawl->isEndOfWord = true; 
} 
  
// Returna true si la key esta en el  trie, de lo contrario false
bool search(struct TrieNode *root, string key) { 
    struct TrieNode *pCrawl = root; 
  
    for (int i = 0; i < key.length(); i++) { 
        int index = key[i] - '0'; 
        if (!pCrawl->children[index]) 
            return false; 
  
        pCrawl = pCrawl->children[index]; 
    }
    return (pCrawl != NULL && pCrawl->isEndOfWord); 
} 

int main(int argc,char * argv[])
{
    if (argc != 2){
        printf("Forma de uso >: ./server puerto\n");
        exit(0);
    }
    Respuesta resp(atoi(argv[1]));
    struct TrieNode *root = getNode(); 
    cout << "Servidor C iniciado....\n";
    std::ofstream file1("registrosC.txt", std::ofstream::out | std::ofstream::trunc);
    file1.close();
    ofstream output;
    while (true) {
        struct mensaje *msj = resp.getRequest();
        if(msj != NULL) {
            output.open("registrosC.txt", ios::out | ios::app );
            struct registro r;
            memcpy(&r, &msj->arguments, sizeof(registro));
            struct timeval actual;
            actual.tv_sec = 0;
            actual.tv_usec = 0;
            if(!search(root, string(r.celular))){
                insert(root, string(r.celular));
                gettimeofday(&actual,NULL);
                string regs = r.toString();
                cout << actual.tv_sec << endl;
                cout << actual.tv_usec << endl;
                regs += to_string(actual.tv_sec) + to_string(actual.tv_usec);
                output.write(regs.c_str(),regs.length());
                output.write("\n",1);
                output.flush();
                resp.sendReply((char *)&actual);
            }else{
            	cout << "Telefono ya registrado" << endl;
                resp.sendReply((char *)&actual);
            }
            output.close();
        }
        cout << endl;
    }
}