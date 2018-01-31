// Binqueue.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <queue> 
#include <ctime> 
#include <utility>
using namespace std;
#define MAXDIST 0x3f3f3f3f
#define MAXARR 300000
class Bintree;
clock_t start,stop;
double duration;
struct memini{
	Bintree*node;
	memini*next;
	memini():node(NULL),next(NULL){}
};
struct mempool{
	memini*unuse;
	memini*use;
	memini*unuse_end;
	memini*use_end;
	mempool():unuse(NULL),use(NULL),unuse_end(NULL),use_end(NULL){}
	void init(int arr);
	inline Bintree*get();
	void deleteall();
}my;
struct MinHeapNode {
	int v;
	int dist;
	MinHeapNode(int _v=-1, int _dist=-1) :v(_v), dist(_dist) {}
	inline bool operator >(const MinHeapNode&f)const
	{
		return dist > f.dist;
	}
	inline bool operator <(const MinHeapNode&f)const
	{
		return dist < f.dist;
	}
	friend class Bintree;
	friend class Binary_queue;
};
class Bintree { 
private:
	Bintree* leftchild;
	Bintree* sibling;
	
	
public:
	friend class mempool;
	MinHeapNode data;
	Bintree* father;
	Bintree() :leftchild(NULL), sibling(NULL),father(NULL) {}
	Bintree(const MinHeapNode &_data):leftchild(NULL),sibling(NULL),father(NULL),data(_data){}
	void assign(const MinHeapNode& a)
	{
		leftchild=sibling=father=NULL;
		data=a;
	}
	friend class Binary_queue;
};
class Binary_queue {
private:
	//enum{basic_size=100};
	Bintree* forest[200];
	Bintree* st[300000];
	int size;
public:
	Binary_queue() :size(0)
	{
		memset(st, 0, sizeof(st));
	}
	Binary_queue(int _size) :size(_size)
	{
	}
	void clear()
	{
		size = 0;
		memset(st, 0, sizeof(st));
	}
	inline bool is_empty()
	{
		return !static_cast<bool>(size);
	}
	inline bool contain(int v)
	{
		return static_cast<bool>(st[v]);
	}
	void decrease(int v, int key)
	{
		Bintree*node = st[v],*child=NULL;
		node->data.dist = key;
		child = node;
		while (child->father&&child->data.dist < child->father->data.dist)
		{
			swap(child->data, child->father->data);
			child = child->father;
		}
	}
	Bintree*combine(Bintree*T1,Bintree*T2)const//T1->root
	{
		if (T1->data > T2->data)
		{
			return combine(T2, T1);
		}
		T2->father = T1;
		T2->sibling = T1->leftchild;
		T1->leftchild = T2;
		return T1;
	}
	inline int get_size()const {
		return size;
	}
	Binary_queue& merge(Binary_queue&a)
	{
		Bintree *T1,*T2,*carry=NULL;
		size += a.get_size();
		for (int i = 0, j = 1;j <= size;i++, j <<= 1)
		{
			T1 = forest[i];T2 = a.forest[i];
			switch (!!T1+2*!!T2+4*!!carry)
			{
			case 0:
			case 1:break;
			case 2://0 1 0
				forest[i] = T2;
				a.forest[i] = NULL;
				break;
			case 3://110
				carry = combine(T1, T2);
				forest[i] = a.forest[i] = NULL;
				break;
			case 4://001
				forest[i] = carry;
				carry = NULL;
				break;
			case 5://101
				carry = combine(T1, carry);
				forest[i] = NULL;
				break;
			case 6://011
				carry = combine(T2, carry);
				a.forest[i] = NULL;
				break;
			case 7://111
				forest[i] = carry;
				carry = combine(T1, T2);
				a.forest[i] = NULL;
				break;
			}
		}
		return *this;
	}
	void push(const MinHeapNode&ele)
	{
		Bintree*T1 = NULL, *T = NULL;
		//Bintree tmp(ele);		
		st[ele.v] = my.get();//new Bintree(ele);
		
		//my.get();//new Bintree(ele);	

		st[ele.v]->assign(ele);
		T = st[ele.v];
		size++;
		for (int i = 0,j = 1;j <= size&&T;i++, j <<= 1)
		{
			T1 = forest[i];
			switch (!!T1+2*!!T)
			{
			case 0:case 1:break;
			case 2://010
				forest[i] = T;
				T = NULL;
				break;
			case 3://011
				forest[i] = NULL;
				T = combine(T1, T);
				break;
			default:
				break;
			}
		}
	}
	const MinHeapNode& top() const
	{
		if (!size)
		{
			cerr << "NULL" << endl;
			assert(0);
		}
		int res = 0;
		while (!forest[res])
		{
			res++;
		}
		for (int i = res + 1, j = 1 << i;j <= size;j <<= 1, i++)
		{
			if (forest[i] && forest[i]->data.dist < forest[res]->data.dist)
			{
				res = i;
			}
		}
		return forest[res]->data;
	}
	void pop() 
	{
		//cout << "FUCK" << endl;
	//	system("pause");
		if (!size)
		{
			cerr << "Delete NULL\n";
			return;
		}
		Bintree*deletedtree, *oldtree;
		int mintree = 0;
		//cout << "&&&&&" << endl;
		while (!forest[mintree])
		{
			mintree++;
		}
		//cout << mintree << "^^^" << endl;
		for (int i = mintree + 1, j = 1 << i;j <= size;j <<= 1, i++)
		{
			if (forest[i] && forest[i]->data.dist < forest[mintree]->data.dist)
			{
				mintree = i;
			}
		}
		//cout << mintree << "&&&&" <<endl;
		oldtree = forest[mintree];
		deletedtree = oldtree->leftchild;
		if(deletedtree)
		{
			deletedtree->father = NULL;
		}
		oldtree->leftchild = NULL;
		st[oldtree->data.v] = NULL;
		//delete oldtree;

		Binary_queue deletedqueue((1 << mintree) - 1);
		for (int i = mintree - 1;i >= 0;i--)
		{
			deletedqueue.forest[i] = deletedtree;
			deletedtree = deletedtree->sibling;
			if(deletedtree)
			{
				deletedtree->father = NULL;
			}
			deletedqueue.forest[i]->sibling = NULL;
		}
		
		forest[mintree] = NULL;
		size -= (deletedqueue.get_size() + 1);
	//	cout<<"^^^^^^***^&&"<<endl;
	//	system("pause");
		merge(deletedqueue);
	}
};

void mempool::init(int arr)
{
	unuse=new memini;
	unuse->node=new Bintree;
	unuse_end=unuse;
	use_end=NULL;
	for(int i=1;i<arr;i++)
	{
		unuse_end->next=new memini;
		unuse_end=unuse_end->next;
		unuse_end->node=new Bintree;
	}
}
inline Bintree* mempool::get()
{
	memini*tmp;
	if(!use)
	{
		use=unuse;
		unuse=unuse->next;
		use->next=NULL;
		use_end=use;
		return use->node;
	}
	else{
		tmp=unuse;
		unuse=unuse->next;
		tmp->next=use;
		use=tmp;
		return tmp->node;
	}
}
void mempool::deleteall()
{
	if(use)
	{
		unuse_end->next=use;
		unuse_end=use_end;
		use_end=NULL;
		use=NULL;
	}
}


/////////
struct AdjListNode {
	int dest;
	int weight;
	struct AdjListNode *next;
};
typedef struct AdjListNode *AdjList;
struct Graph {
	int V;
	AdjList *arr;
};
struct Graph *CreateGraph(int v)
{
	struct Graph *g;
	int i;

	g = (struct Graph*)malloc(sizeof(struct Graph));
	g->arr = (AdjList*)malloc((v + 5) * sizeof(struct AdjListNode));

	for (i = 0; i <= v; i++) {
		g->arr[i] = NULL;
	}
	return g;
}


int dist[MAXARR];
bool vis[MAXARR];

int dijkstra(int u, int fina,Graph*gra)
{
	Binary_queue *ff=new Binary_queue;
	//priority_queue<MinHeapNode,vector<MinHeapNode>,less<MinHeapNode>>ff;
	dist[u] = 0;
	//system("pause");
	ff->push(MinHeapNode(u, 0));
	//system("pause");
	MinHeapNode mtop,ins;
	int v, dis;
	int cnt=0;
	//cout<<"YYYYYYY"<<endl;
	while (!ff->is_empty())
	{
		mtop = ff->top();ff->pop();
		v = mtop.v;dis = mtop.dist;
		if(vis[v])
		{
			continue;
		}
		vis[v] = 1;
		if (v == fina)
		{
			//cout<<cnt<<endl;
			//cout<<ff->get_size()<<' '<<cnt<<endl;
			//delete ff;
			return dis;
		}
		AdjList p = gra->arr[v];
		//cnt++;
		while (p != NULL)
		{
			//cnt++;
			int tp = p->dest;
			if (!vis[tp] && (dist[v] + p->weight < dist[tp]))
			{
				dist[tp] = dist[v] + p->weight;
				if(ff->contain(tp))
				{
					ff->decrease(tp,dist[tp]);
				}
				else{
					ff->push(MinHeapNode(tp, dist[tp]));
				}
			}
			p = p->next;
		}
		//cout<<cnt<<endl;
	}
	return dist[fina];
}
vector<pair<int,int> >pa[300000];
int main()
{
	my.init(3000000);
	freopen("city.txt", "r", stdin);
	int v, e,fr,to,di;
	AdjList p, q;
	cin >> v >> e;
	for(int i=0;i<e;i++)
	{

		cin>>fr>>to>>di;
		pa[fr].push_back(make_pair(to,di));
	}
	for(int i=1;i<=v;i++)
	{
		sort(pa[i].begin(),pa[i].end());
	}
	struct Graph*gra = CreateGraph(v);
	for(int i=1;i<=v;i++)
	{
		cout<<"i"<<i<<endl;
		for(int j=0;j<pa[i].size();j++)
		{
			p = new AdjListNode;
			p->dest = pa[i][j].first;
			p->weight = pa[i][j].second;
			p->next = NULL;
			if (gra->arr[i] == NULL)	gra->arr[i] = p;
			else {
				q = gra->arr[i];
				while (q->next != NULL)q = q->next;
				q->next = p;
			}
		}
	}
	cout<<"^^^^^^"<<endl;
	/*for (int i = 0;i < e;i++)
	{
		//cout << i << endl;
		cin >> fr >> to >> di;
		p = new AdjListNode;
		p->dest = to;
		p->weight = di;
		p->next = NULL;
		if (gra->arr[fr] == NULL)	gra->arr[fr] = p;
		else {
			q = gra->arr[fr];
			while (q->next != NULL)q = q->next;
			q->next = p;
		}
	}*/
	//system("pause");
	int query,a,b;
	cin>>query;
	cout<<query<<endl;
	for(int i=0;i<query;i++)
	{
		memset(dist, 0x3f3f, sizeof(dist));
		memset(vis, 0, sizeof(vis));
		cin >> a >> b;
		//cout<<"^^^^^"<<endl;
		start=clock();
		int ans=dijkstra(a,b,gra);
		stop=clock();
		duration+=((double)(stop-start))/CLOCKS_PER_SEC;
		if(ans==0x3f3f3f3f)
		{
			cout<<"INF"<<endl;
		}
		else{
			cout<<ans<<endl;
		}
		cout<<i<<' '<<duration<<endl;
		//cout << dijkstra(a, b, gra)<<'\n';
		//cout<<"******"<<endl;
		my.deleteall();
	}
	
	//system("pause");
}





