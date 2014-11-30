#ifndef TREEAHOCORASICK_H
#define TREEAHOCORASICK_H

#include <cmath>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

class TreeAhoCorasick
{
	static const int MAXNODES = 1000;
	
	public:
		
		int P; // Numero de nos na arvore de prefixo 
		char chartonode[MAXNODES]; // caractere para este no
		vector<int> word_end[MAXNODES]; // Numero das palavras terminando aqui
		int adj[MAXNODES][ALPHABET_SIZE]; // lista de adjacencia: [no][charactere] = no filho (-1 se nao-existente)
		vector<int> vadj[MAXNODES]; // Numero dos nos filhos	
		
		///aho-corasick
		int suf[MAXNODES];
		int preend[MAXNODES]; // back link 
		int dep[MAXNODES]; // depth do no
		int par[MAXNODES]; // pai do no
		
		// inicializar variaveis e alocar espaco
		void init() {
			word_end[0].clear();	
			fill_n(adj[0], ALPHABET_SIZE, -1);
			vadj[0].clear();
			P = 1;
		}
		
		// Adicionar a palavra no arvore de prefixos e associa a ela o numero pat_num
		void add(string pat, int pat_num){
			int c = 0;
			for (int i = 0; pat[i]; i++) {
				int &cs = adj[c][(int)pat[i]]; // Nao esquecer o '&' para referencia
				if (cs == -1){
					cs = P;
					vadj[c].push_back(P);
					chartonode[P] = pat[i];
					word_end[P].clear();
					fill_n(adj[P], ALPHABET_SIZE, -1);		
					vadj[P].clear();
					P++;
				}
				c = cs;
			}
			word_end[c].push_back(pat_num);
		}
		
		// completar arvore
		void complete_tree() {
			queue<int> fila;
			fila.push(0);
			par[0] = -1;
			dep[0] = 0;
			while(!fila.empty()) {
				int i = fila.front();
				fila.pop();
				if (i == 0)
					suf[i] = preend[i] = -1;
				else {
					int s = suf[par[i]];
					while(s != -1 && adj[s][(int)chartonode[i]] == -1)
						s = suf[s];
					suf[i] = s == -1 ? 0 : adj[s][(int)chartonode[i]];
					preend[i] = word_end[suf[i]].size() ? suf[i] : preend[suf[i]];
				}
				for (int k : vadj[i]) {
					par[k] = i;
					dep[k] = dep[i]+1;
					fila.push(k);
				}
			}
		}
		
};

#endif