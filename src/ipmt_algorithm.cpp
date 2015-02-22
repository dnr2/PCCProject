#include "ipmt_algorithm.h"


void readFromFile(string & textFile, string & input)
{
	ifstream infile(textFile);

	if (! infile.good()){
		cout << "Arquivo de padrão << " << textFile << " >> inválido" << endl;
		exit(1);
	}
	string line;
	while ( getline( infile, line) ) {
		line += '\n';
		input += line;
	}
}

void writeToFile( string & textFile, string & output)
{
	ofstream ofs;
 	ofs.open ( textFile, ofstream::out | ofstream::trunc );
  	ofs << output;
  	ofs.close();
}


void ipmt_index_tree(string &textfile)
{
	string fileContent;
	readFromFile( textfile , fileContent);
	SuffixTree suffixTree(fileContent);
	string treeRepresentation;
	suffixTree.getByteRepresentation(treeRepresentation);

	string textPlusTree;
	textPlusTree += fileContent.size();
	textPlusTree += '\n';
	textPlusTree += fileContent;
	textPlusTree += treeRepresentation;
	string encoded;

	lzw_encode( textPlusTree, encoded);
	

	string outputFileName;
	int index = textfile.rfind(".");
	outputFileName = textfile.substr( 0, index);
	outputFileName += ".idx";
	writeToFile( outputFileName, encoded);

}

void ipmt_index_array(string &textfile)
{

}

void ipmt_search(vector<string> &patterns, string &textfile)
{

}

void lzw_encode(const string & text, string & ret)
{
	vector<int> encoded;
	map<string,int> dictionary;
	for (int i = 0; i < 256; i++){
		dictionary[string(1, i)] = i;
	}

	string s = "";
	char ch;

	for ( int i = 0; i < (int) text.length(); i++){
		ch = text[i];

		string sch = s + ch;

		if (dictionary.count(sch)){
			s = sch;
		}else{
			encoded.push_back(dictionary[s]);
			int size = dictionary.size() + 1;
			dictionary[sch] = size;

			s = ch;
		}
	}
	if (! s.empty()){
		encoded.push_back(dictionary[s]);
	}

	
	vectorToString( encoded , ret);
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
	for (int i = 1; i < (int) encoded.size(); i++){
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

void vectorToString(vector<int> v, string & ret){
	ostringstream os;
	
	for (vector<int>::iterator it = v.begin() ; it != v.end(); ++it){
		os << *it << '\n';
	}
	ret = os.str();
}



/*
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
	
	return 0;
}*/