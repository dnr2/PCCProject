#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include <cmath>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>		
#include <limits>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

class SuffixTree
{
		
	public:		
		
		static const int INF = 1<<30;
	
		int last;
		string str;
		
		struct Node;
		
		struct Edge
		{
			int a,b;
			Node *end;
			SuffixTree* tree;
			Edge(int a, int b, Node *end, SuffixTree* tree) : a(a), b(b), end(end), tree(tree) {}
			char getFirst() const { return tree->str[a]; }
			int size() const { return min(tree->last,b)-a+1; }
			friend ostream& operator<<(ostream& out, Edge& e);
		};
		
		struct Node
		{
			map<char,Edge*> edges;
			Node* suffix;
			int a,b;
			SuffixTree* tree;
			Node(SuffixTree* tree) : tree(tree) {}
			void add(Edge* e) { edges[e->getFirst()] = e; }
			Edge* operator[](char c) { return edges[c]; }
			int numChildren() { return edges.size(); }
			friend ostream& operator<<(ostream& out, Node&);
		};
		
		Node *root, *sentinel;

		Node* getRoot() { return root; }
		const string& getStr() const { return str; }
		
		void setPrefix(Node* n, Edge* e, int len)
		{
			map<char,Edge*>::iterator it = n->edges.begin();
			for (; it != n->edges.end(); ++it) 
			{
				Edge *edge = it->second;
				setPrefix(edge->end,edge,len+min(edge->b,last)-edge->a+1);
			}
			if (e) { n->b = min(e->b,last); n->a = n->b - len + 1; }
			else { n->a = 0; n->b = -1; }
		}

		Node* testAndSplit(Node* s, int k, int p, char c)
		{
			if (k > p) return s == sentinel ? NULL : (*s)[c] == NULL ? s : NULL;
			Edge* e = (*s)[str[k]];
			if (c == str[e->a+p-k+1]) return NULL;
			Node* r = new Node(this);
			Edge* re = new Edge(e->a+p-k+1,e->b,e->end,this);
			r->add(re);
			Edge* se = new Edge(e->a,e->a+p-k,r,this);
			s->add(se);
			return r;
		}
		
		Node* canonize(Node* s, int& k, int p)
		{
			if (p < k) return s;
			if (s == sentinel) { s = root; ++k; if (p < k) return s; }
			Edge* e = (*s)[str[k]];
			while (e->b - e->a <= p - k)
			{
				k = k + e->b - e->a + 1;
				s = e->end;
				if (k <= p) e = (*s)[str[k]];
			}
			return s;
		}
		
		Node* update(Node* s, int& k, int i)
		{
			Node *oldr = root, *r = testAndSplit(s,k,i-1,str[i]);
			while (r)
			{
				Node* rp = new Node(this);
				Edge* e = new Edge(i,INF,rp,this);
				r->add(e);
				if (oldr != root) oldr->suffix = r;
				oldr = r;
				s = canonize(s->suffix,k,i-1);
				r = testAndSplit(s,k,i-1,str[i]);	    
			}
			if (oldr != root) oldr->suffix = s;
			return s;
		}
		void buildTree()
		{
			root = new Node(this);
			sentinel  = new Node(this);
			root->suffix = sentinel;
			Node* s = root;
			int k = 0;
			last = -1;
			for (int i = 0; i < (int)str.size(); ++i)
			{
				++last;
				s = update(s,k,i);
				s = canonize(s,k,i);
			}
		}
		
		int fix(int x) const { return x == INF ? last : x; }
		
		friend ostream& operator<<(ostream& out, SuffixTree& t);
		
		ostream& prettyFormat(ostream& out, Node* n, int tab) const
		{
			out << string(tab,' ') << "\"" << *n << "\"" << endl;
			map<char,Edge*>::iterator it = n->edges.begin();
			for (; it != n->edges.end(); ++it)
			{
				Edge* e = it->second;
				cout << string(tab,' ') << it->first << " : " << *e << " = " << e->a << ',' << fix(e->b) << ',' << e->size() << endl;
				prettyFormat(out,e->end,tab+2);
			}
			return out;
		}

		SuffixTree(const string& str) : str(str) 
		{
			buildTree();
			setPrefix(root,NULL,0);
		}
		
};

ostream& operator<<(ostream& out, SuffixTree::Edge& e) { return out << e.tree->str.substr(e.a,e.size()); }
ostream& operator<<(ostream& out, SuffixTree::Node& n) { return out << n.tree->str.substr(n.a,min(n.b,n.tree->last)-n.a+1); }	
ostream& operator<<(ostream& out, SuffixTree& t) { return t.prettyFormat(out, t.root, 0); }

int main(int argc, char** argv)
{
    SuffixTree t(argv[1]);
    cout << t << endl;
    return 0;
}


#endif