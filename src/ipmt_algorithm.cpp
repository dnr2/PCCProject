#include "ipmt_algorithm.h"

void lzw_encode(const string &text, vector<int> &ret)
{
	map<string,int> dictionary;
	for (int i = 0; i < 256; i++){
		dictionary[string(1, i)] = i;
	}

	string s = "";
	char ch;

	for (uint i = 0; i < text.length(); i++){
		ch = text[i];

		string sch = s + ch;

		if (dictionary.count(sch)){
			s = sch;
		}else{
			ret.push_back(dictionary[s]);
			int size = dictionary.size() + 1;
			dictionary[sch] = size;

			s = ch;
		}
	}
	if (! s.empty()){
		ret.push_back(dictionary[s]);
	}


}

string lzw_decode(const vector<int> encoded)
{
	std::map<int,std::string> dictionary;
	for (int i = 0; i < 256; i++){
		dictionary[i] = std::string(1, i);
	}

	int prevcode, currcode;
	string ret;
	string entry;
	string aux;

	prevcode = encoded[0];
	ret = dictionary[prevcode];
	aux = ret;
	for (uint i = 1; i < encoded.size(); i++){
		currcode = encoded[i];

		if (dictionary.count(currcode)){
			entry = dictionary[currcode];
		} else{
			entry = aux + aux[0];
		}
		ret += entry;

		int size = dictionary.size() + 1;
		dictionary[size] = aux + entry[0];

		prevcode = currcode;
		aux = entry;

	}

	return ret;

}

int main(int argc, char** argv) {
	string str, strbuf; 
	while( getline( cin, strbuf)){
		str += strbuf;
		str += "\n";
	}

	
	vector<int> encoded;
	lzw_encode(str, encoded);
	
	//cout << "Encoded\t" << encoded << endl;
	string decoded = lzw_decode(encoded);
	
	//cout << "Decoded\t" << decoded << endl;
	cout << decoded << endl;
	cout << str.length() << endl;
	cout << decoded.length() << endl;
	string enc;
	for (uint i = 0; i < encoded.size(); i++){
		enc += encoded[i];
		enc += '\n';
	}
	cout << enc.length() << endl;

	
	return 0;
}

/*int main(int argc, char** argv)
{
	string str, strbuf; 
	while( getline( cin, strbuf)){
		str += strbuf;
		str += "\n";
	}
	
	// DB( str.size());
    SuffixTree aux(str);
	
	// aux.findOccurrences("for");
	string buff, buff_encoded;
	aux.getByteRepresentation(buff);

	//lz77_encode(buff, buff_encoded);
	DB(buff.length());
	//DB(buff_encoded.length());

	SuffixTree tree( str , buff );
	DB( str.size());
	DB( tree.nodeCount );	
	//tree.findOccurrences("it");	
	//tree.findOccurrences("to");	
	//tree.findOccurrences("the");	
	//tree.findOccurrences("indicate");

    return 0;
}*/