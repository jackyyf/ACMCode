#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<vector>
#include<algorithm>

using namespace std;

#define LIM 262144

int n;
long long ans[LIM];
int fa[LIM];
int size[LIM];

int FA(const int cur) {
	if(fa[cur] == cur) return cur;
	return fa[cur] = FA(fa[cur]);
}

void UNION(int a, int b) { // A merge to B
	//if(FA(a) == FA(b)) return ;
	size[FA(b)] += size[FA(a)];
	fa[FA(a)] = FA(b);
}

struct Edge {
	int from, to, cost;
	inline bool operator< (const Edge &comp) const {
		return cost > comp.cost; // DESC
	}

} edge[LIM];

int main() {
	register int i;
	while(scanf("%d\n", &n) == 1) {
		for(i = 1; i < n; ++ i) {
			fa[i] = i;
			size[i] = 1;
			ans[i] = 0;
			scanf("%d%d%d", &edge[i].from, &edge[i].to, &edge[i].cost);
		}
		fa[n] = n; size[i] = 1; ans[i] = 0;
		sort(edge + 1, edge + n);
		for(i = 1; i < n; ++ i) {
			int a = edge[i].from, b = edge[i].to;
			long long ans1 = ans[FA(a)] + size[FA(b)] * (long long)edge[i].cost, ans2 = ans[FA(b)] + size[FA(a)] * (long long)edge[i].cost;
			if(ans1 > ans2) { // A <- B
				UNION(a, b);
				ans[FA(a)] = ans1;
			} else { // B <- A
				UNION(b, a);
				ans[FA(b)] = ans2;
			}
		}
		printf("%lld\n", ans[FA(1)]);
	}
	return 0;
}
