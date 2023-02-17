#include "bits/stdc++.h"
using namespace std;
#define ll long long
#define pb push_back
#define pob pop_back()
#define mp make_pair
#define fastio                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL)
#define pf push_front
#define pof pop_front()
#define mod 1000000007
#define add_m(a, b) (((a % mod) + (b % mod)) % mod)
#define sub_m(a, b) (((a % mod) - (b % mod) + mod) % mod)
#define mul_m(a, b) (((a % mod) * (b % mod)) % mod)
/*
Don't worry, notics that if we had some (usable) roads also, then we could
tale the weight of that edge == 0 and find the spannign tree
*/

vector<ll> topoSort(vector<vector<ll>> &adj)
{
    ll v = adj.size(), indeg[v];
    for (ll i = 0; i < v; i++)
        indeg[i] = 0;
    for (ll i = 0; i < v; i++)
    {
        for (ll c : adj[i])
            indeg[c]++;
    }

    vector<ll> ans;
    queue<ll> q;
    for (ll i = 0; i < v; i++)
        if (indeg[i] == 0)
            q.push(i);

    while (!q.empty())
    {
        ll a = q.front();
        ans.push_back(a);
        q.pop();

        for (ll c : adj[a])
        {
            indeg[c]--;
            if (indeg[c] == 0)
                q.push(c);
        }
    }
    return ans;
}

void dfsOfGraph(vector<vector<ll>> &adj, ll ver, vector<bool> &visited, stack<ll> &ans)
{
    visited[ver] = true;
    for (ll nbr : adj[ver])
    {
        if (!visited[nbr])
            dfsOfGraph(adj, nbr, visited, ans);
    }
    ans.push(ver);
}
void dfs_simple(vector<vector<ll>> &adj, ll ver, vector<bool> &visited, ll id, ll coins[], ll scc_id[], ll &scc_cst)
{
    visited[ver] = true;
    scc_id[ver] = id;
    scc_cst += coins[ver];

    for (ll nbr : adj[ver])
    {
        if (!visited[nbr])
            dfs_simple(adj, nbr, visited, id, coins, scc_id, scc_cst);
    }
}

ll kosaraju(vector<vector<ll>> &adj, vector<vector<ll>> &adjT, ll coins[])
{
    ll n = adj.size();
    stack<ll> st;
    vector<bool> visited(n);
    for (ll i = 0; i < n; i++)
    {
        if (!visited[i])
            dfsOfGraph(adj, i, visited, st);
    }

    for (ll i = 0; i < n; i++)
        visited[i] = false;

    ll scc_id[n], i = 0;
    vector<ll> scc_cost;
    while (true)
    {
        while (!st.empty() && visited[st.top()])
            st.pop();
        if (st.empty())
            break;

        ll scc_cst = 0;
        dfs_simple(adjT, st.top(), visited, i, coins, scc_id, scc_cst);
        scc_cost.push_back(scc_cst);
        i++;
    }
    ll dag_size = scc_cost.size();
    vector<vector<ll>> dag(dag_size);

    for (ll ver = 0; ver < n; ver++)
    {
        for (ll nbr : adj[ver])
        {
            if (scc_id[nbr] == scc_id[ver])
                continue;
            dag[scc_id[ver]].push_back(scc_id[nbr]);
        }
    }

    // dp[vertex] = maximum coins earned by ending journey at the scc (vertex)
    vector<ll> topo = topoSort(dag), dp(dag_size);
    ;
    ll max_coins = 0;

    for (ll ver : topo)
    {
        if (dp[ver] == 0)
            dp[ver] = scc_cost[ver]; // if it can't be reached from anywhere else. this is a base case
        max_coins = max(max_coins, dp[ver]);
        for (ll nbr : dag[ver])
            dp[nbr] = max(dp[nbr], dp[ver] + scc_cost[nbr]);
    }
    return max_coins;
}
void f()
{
    ll n, m, u, v;
    cin >> n >> m;
    ll coins[n];
    for (ll i = 0; i < n; i++)
        cin >> coins[i];
    vector<vector<ll>> adj(n), adjT(n);

    for (ll i = 0; i < m; i++)
    {
        cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
        adjT[v].push_back(u);
    }
    cout << kosaraju(adj, adjT, coins) << "\n";
}

signed main()
{
    fastio;
    f();

    return 0;
}