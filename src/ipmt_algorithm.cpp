#include "ipmt_algorithm.h"


#define DB( x ) cerr << #x << " = " << x << endl
#define _ << " " <<

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
	input = input.substr(0, input.length()-1);
}

void writeToFile( string & textFile, string & output)
{
	ofstream ofs;
 	ofs.open ( textFile, ofstream::out | ofstream::trunc );
  	ofs << output;
  	ofs.close();
}

void splitInputData(string &input, string &originalText, string &treeRepresentation)
{
	int textSize, treeSize;

	
	char * buf = const_cast<char*> (input.c_str() );
	sscanf(buf, "%d", &textSize);
	char *pch;
	pch = strchr(buf, '\n');
	buf = pch+1;
	sscanf(buf, "%d", &treeSize);
	pch = strchr(buf, '\n');
	buf = pch+1;

	DB(textSize);
	DB(treeSize);
	

	char *text = new char[textSize+1];
	memcpy(text, buf, textSize);
	
	text[textSize] = '\0';
	originalText = text;

	buf = buf + textSize;
	char *tree = new char[treeSize+1];
	memcpy(tree, buf, treeSize);

	tree[treeSize] = '\0';
	treeRepresentation = tree;

	
}

void ipmt_index_tree(string &textfile)
{
	string fileContent;
	readFromFile( textfile , fileContent);
	SuffixTree suffixTree(fileContent);
	string treeRepresentation;
	suffixTree.getByteRepresentation(treeRepresentation);
	
	DB(treeRepresentation.size());

	string textPlusTree;
	ostringstream os;
	os << fileContent.size() << '\n';
	os << treeRepresentation.size() << '\n';
	os << fileContent;
	os << treeRepresentation;
	textPlusTree = os.str();
	string encoded;

	lzw_encode( textPlusTree, encoded);

	DB(textPlusTree.size());
	DB(encoded.size());

	string outputFileName;
	int index = textfile.rfind(".");
	outputFileName = textfile.substr( 0, index);
	outputFileName += ".idx";
	writeToFile( outputFileName, encoded);
	exit(0);
}

void ipmt_index_array(string &textfile)
{

}

void ipmt_search(vector<string> &patterns, string &textfile)
{
	string encodedContent, fileContent;
	readFromFile(textfile, encodedContent);
	lzw_decode(encodedContent, fileContent);

	string text, treeRepresentation;
	splitInputData(fileContent, text, treeRepresentation);

	SuffixTree suffixTree(text, treeRepresentation);

	for(string &pat : patterns){
		suffixTree.findOccurrences(pat);
	}
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

void lzw_decode(string &encoded_str, string &ret)
{
	vector<int> encoded;
	stringToVector(encoded_str, encoded);


	map<int, string> dictionary;
	for (int i = 0; i < 256; i++){
		dictionary[i] = string(1, i);
	}

	int prevcode, currcode;
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

}

void vectorToString(vector<int> &v, string & ret)
{
	ostringstream os;
	
	for (vector<int>::iterator it = v.begin() ; it != v.end(); ++it){
		os << *it << '\n';
	}
	ret = os.str();
}

void stringToVector(string &encoded, vector<int> &ret)
{
	ret.clear();

	int val;
	stringstream ss(encoded);
	while ( ss >> val ) {
		ret.push_back(val);
	}
}
