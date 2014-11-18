#include "tpf_algorithm.h"

int tpf_find(char **i_patterns, int i_pattern_amount, char *i_textfile, int i_tpf_type, char **o_results)
{
	int ret = TPF_OK;
	switch (i_tpf_type){
		case TPF_EXACT:
			break;
		case TPF_EXACT_PATTERNFILE:
			ret = tpf_aho_corasick(i_patterns, i_pattern_amount, i_textfile, o_results);
			break;
		case TPF_APPROXIMATE:
			break;
		default:
			return TPF_ERR_NO_SUCH_OPTION;
			break;
	}
	return ret;
}


// ---- Inicio AHO-CORASICK ---- //


using namespace std;

int P; // Number of nodes in the prefix tree
char ltt[DOTS]; // Character for this node
vector<int> wend[DOTS]; // Numbers of the words ending here
int adj[DOTS][ALPHABET_SIZE]; // [node][character]: child node (-1 if non-existent)
vector<int> vadj[DOTS]; // Numbers of the child nodes

int dec(char c) { // ``character $\rightarrow$ number'' mapping, for example:
	return c-'a';
}

void init() {
	wend[0].clear();
	fill_n(adj[0], ALPHABET_SIZE, -1);
	vadj[0].clear();
	P = 1;
}


// Add a word to the prefix tree and give it the number w
void add(const char *wort, int w) {
	int c = 0;
	for (int i = 0; wort[i]; i++) {
		int &cs = adj[c][dec(wort[i])]; // Don't forget the ``\verb|&|''
		if (cs == -1) {
			cs = P;
			vadj[c].push_back(P);
			ltt[P] = wort[i];
			wend[P].clear();
			fill_n(adj[P], ALPHABET_SIZE, -1);
			vadj[P].clear();
			P++;
		}
		c = cs;
	}
	wend[c].push_back(w);
}

///ahocorasick
int suf[DOTS], preend[DOTS], dep[DOTS], par[DOTS];

void initaho() {
	queue<int> qu;
	qu.push(0);
	par[0] = -1;
	dep[0] = 0;
	while(!qu.empty()) {
		int i = qu.front();
		qu.pop();
		if (i == 0)
			suf[i] = preend[i] = -1;
		else {
			int s = suf[par[i]];
			while(s != -1 && adj[s][dec(ltt[i])] == -1)
				s = suf[s];
			suf[i] = s == -1 ? 0 : adj[s][dec(ltt[i])];
			preend[i] = wend[suf[i]].size() ? suf[i] : preend[suf[i]];
		}
		for (int k : vadj[i]) {
			par[k] = i;
			dep[k] = dep[i]+1;
			qu.push(k);
		}
	}
}

vector<pair<int,int>> matches; // All pairs (a,b) such that word number b starts at position a

void search(const char *text) {
	matches.clear();
	int c = 0;
	for (int i = 0; text[i]; i++) {
		while(c != -1 && adj[c][dec(text[i])] == -1)
			c = suf[c];
		c = c == -1 ? 0 : adj[c][dec(text[i])];
		int t = c;
		while(t != -1) {
			for (int w : wend[t])
				matches.push_back(pair<int,int>(i-dep[t]+1, w));
			t = preend[t];
		}
	}
}

int tpf_aho_corasick(char **i_patterns, int i_pattern_amount, char *i_textfile, char **o_results)
{
	
	return TPF_OK;
}

// ---- Fim AHO-CORASICK ---- //

// ---- Inicio WU-MANBER ---- //

int char_mask(char *i_pattern, bitset *C)
{
	int pattern_len = strlen(i_pattern);
	bitset<pattern_len> C[ALPHABET_SIZE];

	for (int i = 0; i < ALPHABET_SIZE; i++){
		C[i].set();
	}

	bitset<pattern_len> Mk;
	Mk.set(pattern_len);

	for (int k = 1; k <= pattern_len; k++){
		C[(int) i_pattern[k]] &= ~Mk;
		Mk <<= 1;
	}

	return TPF_OK;
}

int tpf_wu_manber(char *i_pattern, char *i_textfile, int i_distance, char **o_results)
{
	int text_len = strlen(i_textfile);//TEMPORARIO

	int results = 0;
	int pattern_len = strlen(i_pattern);
	bitset<pattern_len> C[ALPHABET_SIZE];
	char_mask(i_pattern, C);
	long msb = 2 ** (pattern_len-1);

	bitset<pattern_len> S[i_distance+1];
	S[0].set();

	for (int q = 1; q <= i_distance; q++){
		S[q] = S[q-1] << 1;
	}

	bitset<pattern_len> S1, S2;

	for (int j = 1; j <= ; j++){
		S1 = S[0];
		S[0] = (S[0] << 1) | C[(int) i_textfile[j]];
		for (int q = 1; q <= i_distance; q++){
			S2 = S[q];
			S[q] = 		((S[q] << 1) | C[(int) i_textfile[j]])
					&	(S1 << 1)
					&	(S[q-1] << 1)
					&	S1
			S1 = S2;
		}
		if (S[i_distance] < msb){
			results += 1;
		}
	}

	return TPF_OK;
}

// ---- Fim WU-MANBER ---- //

// ---- Inicio BOYER-MOORE ---- //

int tpf_boyer_moore(char *i_pattern, char *i_textfile, char **o_results)
{
	return TPF_OK;
}

// ---- Fim BOYER-MOORE ---- //


// ignore

int main(){
	
	return 0;
}
