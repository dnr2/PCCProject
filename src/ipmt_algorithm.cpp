#include "ipmt_algorithm.h"
#include "SuffixTree.h"
#include "SuffixArray.h"

#include <assert.h>
#include <stdlib.h>

#define WINDOW_SIZE		65535
#define BUFFER_SIZE		255

using namespace std;

int tpf_find(vector<string> &patterns, string &textfile, int error, int tpf_type, bool count)
{
	int ret = TPF_OK;

	if (tpf_type == TPF_EXACT){ // Boyer Moore
	// int tpf_boyer_moore(string &pat, string &textfile, bool count)
		string pat = patterns[0];

		tpf_boyer_moore(pat, textfile, count);

	} else if (tpf_type == TPF_EXACT_PATTERNFILE){ // Aho-Corasick
		
		tpf_aho_corasick(patterns, textfile, count);
		
	} else if (tpf_type == TPF_APPROXIMATE){ // Wu-Manber
	// int tpf_wu_manber(string &pat, string &textfile, int error, bool count)		
		
		for (string &pat : patterns){		
			int cur_error = min( error, (int) pat.length());
			if( (error < 10 && pat.length() < 63 ) || (error < 2 && pat.length() > 100) ) 
			// if( (error < 2 && pat.length() > 100) ) 
				tpf_wu_manber(pat, textfile, cur_error, count);
			else 
				tpf_sellers(pat, textfile, cur_error, count);			
		}

	} else {
		ret = TPF_ERR_NO_SUCH_OPTION;
	}

	return ret;
}

struct tuple{
	int pos, size;
	char literal;
	tuple(int poS, int sizE, char literaL) : pos(poS), size(sizE), literal(literaL)
	{}
};

string tuple_encode(tuple tup)
{
	string ret = "";
	ret += tup.pos;
	ret += '|';
	ret += tup.size;
	ret += '|';
	ret += tup.literal;
	ret += '$';
}

int lz77_encode(string &text, string &ret)
{
	ret = "";

	int i = 0;

	string window, buffer;

	while (i < text.length()){
		int begin = (i > WINDOW_SIZE)? i-WINDOW_SIZE : 0;

		window = text.substr(begin, i-begin);
		buffer = text.substr(i, BUFFER_SIZE);

		tuple tup = tuple(0, 0, text[i]);

		for (int size = buffer.length(); size>0; size--){
			int index = window.rfind(buffer.substr(0, size));
			if (index >= 0){
				char lit = '';
				if (i + size < text.length()){
					lit = text[i+size];
				}
				tup.pos = window.length()-index-1;
				tup.size = size;
				tup.literal = lit;
				break;
			}
		}
		i = i + tup.size + 1;
		ret += tuple_encode(tup);
	}

	return 0;
}

int lz77_decode(string &encoded, string &text)
{
	text = "";
	int pos = 0;
	int size = 0;
	char literal;
	while (true){
		int idx = encoded.find('|');

		if (idx < 0)
			break;

		pos = text.length() - atoi(encoded.substr(0,idx)) - 1;
		encoded.erase(0,idx+1);
		
		idx = encoded.find('|');
		size = atoi(encoded.substr(0,idx));
		encoded.erase(0,idx+1);

		literal = encoded[0];
		encoded.erase(0, 2);

		text += text.substr(pos, size);
		text += literal;

	}

}