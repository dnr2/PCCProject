#include "ipmt_algorithm.h"

void lzw_encode(const string &text, vector<int> &ret)
{
	string s = "";
	char lit = '\0';



}

int main(int argc, char** argv) {
	string str, strbuf; 
	while( getline( cin, strbuf)){
		str += strbuf;
		str += "\n";
	}

	string encoded, decoded;
	lz77_encode(str, encoded);
	
	//cout << "Encoded\t" << encoded << endl;
	lz77_decode(encoded, decoded);
	
	//cout << "Decoded\t" << decoded << endl;

	cout << str.length() << endl;
	cout << decoded.length() << endl;
	cout << encoded.length() << endl;
	
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