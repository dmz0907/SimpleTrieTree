#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstring>

using namespace std;

struct CharNode;
typedef vector<CharNode> NodeList;
typedef NodeList::iterator Iter;

struct CharNode{
	CharNode(unsigned int _code = 0, int _id = -1)
		:	code(_code),
			id(_id),
			child(NULL)
	{
	}
	
	unsigned int code;
	int id;
	NodeList* child;
};

class TrieTree{
public:
	TrieTree() : m_nodes(65536) 
	{
	}
	~TrieTree()
	{
		remove_all();	
	}
	
	size_t remove_all();
	size_t count_all();
	
	bool add(const char* str, int id)
	{
		size_t len = strlen(str);
		return len ? add(str, len, id) : false;
	}
	bool add(const char* str, size_t len, int id);
	
	int find(const char* str) const
	{
		size_t len = strlen(str);
		return len ? find(str, len) : -1;
	}
	int find(const char* buf, size_t len) const;
private:
	unsigned int NextChar(const unsigned char* &p) const;
	size_t deep_remove(NodeList* nl);
	size_t deep_count(NodeList* nl);

	NodeList m_nodes;
};

size_t TrieTree::remove_all()
{
	size_t count = 0;
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		if(m_nodes[i].id != -1)
		{
			count++;	
		}
		count += deep_remove(m_nodes[i].child);
	}
	return count;
}

size_t TrieTree::deep_remove(NodeList* nl)
{
	size_t count = 0;
	if(nl)
	{
		for(Iter it = nl->begin(); it != nl->end(); it++)
		{
			if(it->id != -1)
			{
				count++;
			}
			count += deep_remove(it->child);
		}
		delete nl;
	}
	return count;
}

size_t TrieTree::count_all()
{
	size_t count = 0;
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		if(m_nodes[i].id != -1)
		{
			count++;
		}
		count += deep_count(m_nodes[i].child);
	}
	return count;
}

size_t TrieTree::deep_count(NodeList* nl)
{
	size_t count = 0;
	if(nl)
	{
		for(Iter it = nl->begin(); it != nl->end(); it++)
		{
			if(it->id != -1)
			{
				count++;
			}
			count += deep_count(it->child);
		}
	}
	return count;
}

unsigned int TrieTree::NextChar(const unsigned char* &p) const
{
	unsigned int ret = *p++;
	//GBK编码：中文字符占两个字节，最高位都为1；英文字符占一个字节，最高位都为0
	if (ret & 0x80){
		ret <<= 8;
		ret |= *p++;
	}else{
		ret = tolower(ret);
	}
	
	return ret;
}

bool TrieTree::add(const char* buf, size_t len, int id)
{
	const unsigned char* p = (const unsigned char*)buf;
	const unsigned char* end = p + len;
	unsigned int ch = NextChar(p);
	CharNode* prev = &m_nodes[ch];
	
	while(p != end && (ch = NextChar(p)) != 0)
	{
		CharNode node(ch);
		if(prev->child == NULL)
		{
			prev->child = new NodeList;
		}
		NodeList* v = prev->child;
		
		Iter it = v->begin();
		while(it != v->end() && it->code < ch)
		{
			it++;
		}
		if(it == v->end() || it->code > ch)
		{
			it = v->insert(it, node);
		}
		
		prev = &(*it);
	}
	
	prev->id = id;
	return true;
}

int TrieTree::find(const char* buf, size_t len) const
{
	const unsigned char* p = (const unsigned char*)buf;
	const unsigned char* end = p + len;
	unsigned int ch = NextChar(p);
	const CharNode* prev = &m_nodes[ch];
	
	while(p != end && (ch = NextChar(p)) != 0)
	{
		if(prev->child == NULL)
		{
			return -1;
		}
		NodeList* v = prev->child;
		Iter it = v->begin();
		//可二分
		while(it != v->end() && ch != it->code)
		{
			it++;
		}
		if(it == v->end())
		{
			return -1;
		}
		prev = &(*it);
	}
	
	return prev->id;
}

int main()
{
	TrieTree trietree;
	cout<<trietree.count_all()<<endl;
	
	int idx = 1;
	trietree.add("dmz", idx++);
	trietree.add("abc", idx++);
	trietree.add("efg", idx++);
	trietree.add("dma", idx++);
	trietree.add("中国", idx++);
	trietree.add("深圳", idx++);
	trietree.add("深ab圳", idx++);
	cout<<trietree.count_all()<<endl;
	
	cout<<"efg, "<<trietree.find("efg")<<endl;
	cout<<"中国, "<<trietree.find("中国")<<endl;
	cout<<"深ab圳, "<<trietree.find("深ab圳")<<endl;
	
	return 0;
}











