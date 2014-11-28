#include "tpf_algorithm.h"
#include "Bitset.h"
#include "TreeAhoCorasick.h"
#include <assert.h>

using namespace std;

int tpf_find(vector<string> &patterns, string &textfile, int error, int tpf_type, bool count)
{
	// TODO: adicionar distancia de edição como parametro e checar se é maior que o padrão
	int ret = TPF_OK;

	for (string &pat : patterns){
		if ( (int) pat.length() < error){
			ret = TPF_ERROR_TOO_LONG;
			return ret;
		}
	}

	switch (tpf_type){
		// Boyer Moore
		case TPF_EXACT:
			break;
		// Aho-Corasick
		case TPF_EXACT_PATTERNFILE:
			break;
		// Wu-Manber
		case TPF_APPROXIMATE:
			break;
		default:
			return TPF_ERR_NO_SUCH_OPTION;
			break;
	}
	return ret;
}


// ---- Inicio AHO-CORASICK ---- //

void tpf_aho_corasick(char *txt, TreeAhoCorasick & tree) {	
	int c = 0;
	for (int i = 0; txt[i]; i++) {
		while(c != -1 && tree.adj[c][(int)txt[i]] == -1)
			c = tree.suf[c];
		c = c == -1 ? 0 : tree.adj[c][(int)txt[i]];
		int t = c;
		while(t != -1) {
			for (int w : tree.word_end[t]){				
				printf("palavra numero %d aparece na posicao %d\n", w, i-(tree.dep[t])+1);
			}
			t = tree.preend[t];
		}
	}
}

int tpf_aho_corasick(char **pats, int pat_amount, char *txt, char **o_results)
{
	
	return TPF_OK;
}


// ---- Fim AHO-CORASICK ---- //

// ---- Inicio WU-MANBER ---- //

int char_mask(string &pat, Bitset *C)
{
	int m = pat.length();

	for (int i = 0; i < ALPHABET_SIZE; i++){
		C[i] = Bitset(m);
		C[i].set();
	}

	Bitset Mk(m);
	Mk.set(0);

	for (int k = 0; k < m; k++){
		C[(int) pat[k]] = C[(int) pat[k]] & ~Mk;
		Mk = Mk << 1;
	}

	return TPF_OK;
}

int tpf_wu_manber(string &pat, char *txt, int error)
{
	int text_len = strlen(txt);//TEMPORARIO

	int results = 0;
	int m = pat.length();
	Bitset *C = new Bitset[ALPHABET_SIZE];
	char_mask(pat, C);

	// msb = 2 ** (m-1)
	Bitset msb(m);
	msb.set(m-1);

	Bitset *S = new Bitset[error+1];
	S[0] = Bitset(m);
	S[0].set();

	for (int q = 1; q <= error; q++){
		S[q] = S[q-1] << 1;
	}

	Bitset S1(m);
	Bitset S2(m);

	for (int j = 0; j < text_len; j++){
		S1 = S[0];
		S[0] = (S[0] << 1) | C[(int) txt[j]];
		for (int q = 1; q <= error; q++){
			S2 = S[q];
			S[q] = 		((S[q] << 1) | C[(int) txt[j]])
					&	(S1 << 1)
					&	(S[q-1] << 1)
					&	S1;
			S1 = S2;
		}
		if (S[error] < msb){
			cout << "*****	RESULT_MATCH 	*****" << endl ;
			results += 1;

			string str(txt);
			int begin =  max(0, j-m-error);
			cout << "substring("<<begin<<", "<<j+1<<")" <<endl;
			cout << str.substr(begin, m+error+1) << endl;
		}
	}

	cout << "Results: " << results << endl<< endl;

	return TPF_OK;
}

// ---- Fim WU-MANBER ---- //

// ---- Inicio BOYER-MOORE ---- //

vector<int> border(string &pat)
{
	int m = pat.length();
	vector<int> B(m+1);
	fill(B.begin()+1, B.end(), 0);
	B[0] = -1;

	int i = 1;

	while (i < m){
		int j = 0;
		while(i+j<m && pat[i+j]==pat[j]){
			j += 1;
			B[i+j] = j;
		}
        i += j - B[j];
	}

	cout << "[";
	for (i = 0; i < m; i++){
		cout << B[i] << ", ";
	}
	cout << B[m] << "]" << endl;
	return B;
}

vector<int> good_suffix(string &pat)
{
	int m = pat.length();
	vector<int> Pi = border(pat);
	vector<int> PiR = border(pat);
	vector<int> S(m+1);
	fill(S.begin(), S.end(), m-Pi[m]);

	for (int l = 1; l <= m; l++){
		int j = m - 1 - PiR[l];
		assert(j<m && j>=0);
		if (pat[j]!=pat[m-l-1] && S[j] > l-PiR[l]) {
			S[j] = l-PiR[l];
		}
	}

	cout << "[";
	for (int i = 0; i < m; i++){
		cout << S[i] << ", ";
	}
	cout << S[m] << "]" << endl;
	return S;

}

vector<int> bad_char(string &pat)
{
	int m = pat.length();
	vector<int> C(ALPHABET_SIZE);
	fill(C.begin(), C.end(), -1);

	for (int j = 0; j < m; j++){
		C[(int) pat[j]] = j;
	}

	cout << "[";
	for (int i = 0; i < m; i++){
		cout << C[i] << ", ";
	}
	cout << C[m] << "]" << endl;
	return C;
}

int tpf_boyer_moore(string &pat, char *txt)
{
	int m = pat.length();
	int n = strlen(txt);
	vector<int> C = bad_char(pat);
	vector<int> S = good_suffix(pat);
	int occ = 0;
	int i = 0;

	while (i <= n-m){
		int j = m-1;
		while (j >= 0 && txt[i+j]==pat[j]){
			j -= 1;
		}
		if (j < 0){
			occ += 1;
			string str(txt);
			cout << "substring("<<i<<", "<<i+m<<")" <<endl;
			cout << str.substr(i, m) << endl;
			i += S[m+j+1];
		}
		else{
			i += max(j-C[(int) txt[i+j]], S[j]);
		}
	}
	return TPF_OK;
}

// ---- Fim BOYER-MOORE ---- //


// ============ ignore ============ // 


//TESTE AHO-CORASICK

int i_pattern_amount;
char i_patterns[100][100];
char i_text[1000];
char str_aux[1000];


void teste_aho_corasick()
{	
	
	while( gets( str_aux ) ){
		if( str_aux[0] == '%' && str_aux[1] == '%') break;
		strcat(i_text, str_aux);
	}
	
	int pos =0, sz = strlen( i_text);
	while( pos < sz){
		printf("pos %3d - ", pos );
		for(int i = 0; i < 25; i++){
			printf("%c", i_text[pos++]);
		}
		cout << endl;
	}
	
	i_pattern_amount = 0;
	
	while( cin >> i_patterns[i_pattern_amount] ){
		printf("%d - %s\n", i_pattern_amount + 1, i_patterns[i_pattern_amount] );
		i_pattern_amount++;
	}
	TreeAhoCorasick tree;
	tree.init();		
	for( int i =0; i < i_pattern_amount; i++){
		tree.add( i_patterns[i], i+1 );
	}
	tree.complete_tree();
	tpf_aho_corasick( i_text , tree);
}

//TESTE BOYER-MOORE

void teste_boyer_moore()
{
			
	string padrao("bao");
	char texto[85] = "abaobabaobab";
	//int distancia = 0;
	
	
	tpf_boyer_moore(padrao, texto);
	
	/*
	char padrao[30] = "boraefjsbor";
	vector<int> S = good_suffix(padrao);
	*/
}

int main(){

	teste_aho_corasick();
	
	return 0;
}
