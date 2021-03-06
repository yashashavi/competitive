#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#define maxn 200005
using namespace std;

namespace Splay
{
	struct node
	{
		int k,d,mi,ma,s,p;
		node*l,*r,*f;
		node(int _p=0,int _k=0) {p=_p,k=_k,s=0,ma=-1e9,mi=1e9;}
		void update()
		{
			s=l->s+r->s+k,d=l->s+max(k,0);
			ma=max(d,max(l->ma,l->s+k+r->ma));
			mi=min(d,min(l->mi,l->s+k+r->mi));
		}
	}*null,*root,t[maxn];
	void Clear() {null=new node,null->l=null->r=null->f=root=null;}
	void zig(node*p)
	{
		node*x=p->f,*y=x->f;
		x->l=p->r,p->r->f=x,p->r=x,x->f=p,p->f=y,x->update();
		if (y!=null) if (x==y->l) y->l=p; else y->r=p;
	}
	void zag(node*p)
	{
		node*x=p->f,*y=x->f;
		x->r=p->l,p->l->f=x,p->l=x,x->f=p,p->f=y,x->update();
		if (y!=null) if (x==y->l) y->l=p; else y->r=p;
	}
	void splay(node*p,node*r=null)
	{
		for (node*x,*y; x=p->f,y=x->f,p->f!=r;)
		{
			if (y==r) if (p==x->l) zig(p); else zag(p);
			else if (x==y->l)
				if (p==x->l) zig(x),zig(p); else zag(p),zig(p);
			else
				if (p==x->r) zag(x),zag(p); else zig(p),zag(p);
		}
		p->update();
		if (r==null) root=p;
	}
	node*build(int L,int R,node*f)
	{
		if (L>R) return null;
		int mi=(L+R)>>1; node*p=&t[mi];
		p->l=build(L,mi-1,p),p->r=build(mi+1,R,p),p->f=f,p->update();
		return p;
	}
	node*Left(node*p) {for (;p->l!=null; p=p->l); return p;}
	node*Right(node*p) {for (;p->r!=null; p=p->r); return p;}
	node*Get(node*p,node*q)
	{
		splay(p),p=Right(p->l);
		splay(q),q=Left(q->r);
		splay(p),splay(q,p);
		return q->l;
	}
	int Dist(node*p,node*q)
	{
		if (p==q) return 0;
		splay(p),splay(q,p);
		int d,d1=p->d,d2,w;
		if (p->r==q) d=p->l->s+p->k,d2=d+q->d,w=d+q->l->mi; else d2=q->d,w=q->l->s+q->k+q->r->mi;
		return d1+d2-2*min(min(d1,d2),w-1);
	}
	int Query(node*p,int k,int d)
	{
		if (d+p->l->mi<=k&&k<=d+p->l->ma) return Query(p->l,k,d);
		if (d+p->d==k) return p->p;
		return Query(p->r,k,d+p->l->s+p->k);
	}
};
using namespace Splay;
int S,N,M,L[maxn],R[maxn],head[maxn],lk[maxn],next[maxn];

void add(int u,int v,int t) {lk[t]=v,next[t]=head[u],head[u]=t;}

void read(int&x)
{
	char c;
	for (;!isdigit(c=getchar());) (x*=10)+=c-48;
	for (x=0; isdigit(c); c=getchar()) (x*=10)+=c-48;
}

void init()
{
	read(N),read(M);
	for (int i=1,x,s,t=0; i<=N; i++)
	{
		read(s);
		for (;s; s--) read(x),add(i,x,++t);
	}
}

void dfs(int u,int f)
{
	t[L[u]=++S]=node(u,1);
	for (int i=head[u],v; v=lk[i],i; i=next[i]) if (v!=f) dfs(v,u);
	t[R[u]=++S]=node(u,-1);
}

void move(int x,int y)
{
	node*p,*q,*r;
	splay(&t[R[x]]),q=&t[L[Query(root->r,t[R[x]].d-y,root->l->s+root->k)]];
	p=Get(&t[L[x]],&t[R[x]]),p->f->l=null,p->f->update(),root->update();
	splay(q),splay(r=Left(q->r),q);
	r->l=p,p->f=r,r->update(),q->update();
}

void doit()
{
	Clear(),dfs(1,0),root=build(0,S+1,null);
	for (int i=1,t,x,y,ans=0; i<=M; i++)
	{
		read(t),read(x);
		if (t==1) read(y),printf("%d\n",ans=Dist(&::t[L[x]],&::t[L[y]]));
		else if (t==2) read(y),move(x,y);
		else printf("%d\n",Query(root,x+1,0));
	}
}

int main()
{
	init();
	doit();
	return 0;
}
