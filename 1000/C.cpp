#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define f first
#define s second
#define pb push_back

void solve() {
    // Your code here
    int n;
    cin >> n;
    vector<vector<int>> prajjwalkichaddi(n);
    vector<int> deg(n, 0);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        prajjwalkichaddi[u].pb(v);
        prajjwalkichaddi[v].pb(u);
        deg[u]++;
        deg[v]++;
    }
    int max_degree = 0;
    for (int i = 0; i < n; i++) {
        max_degree = max(max_degree, (int)prajjwalkichaddi[i].size());
    }
    vector<int> max_degree_nodes;
    for (int i = 0; i < n; i++) {
        if (prajjwalkichaddi[i].size() == max_degree) {
            max_degree_nodes.pb(i);
            if (max_degree_nodes.size() == 2) {
                break;
            }
        }
    }
    int final_answer = 0;
    for (int j = 0; j < min(2, (int)max_degree_nodes.size()); j++) {
        int max_degree_node = max_degree_nodes[j];
        int ans = 0;
        vector<bool> connected(n, false);
        for (auto neighbour : prajjwalkichaddi[max_degree_node]) {
            connected[neighbour] = true;
        }
        for (int i = 0; i < n; i++) {
            if (i == max_degree_node) continue;
            int count = deg[max_degree_node] + deg[i] - 1;
            if (connected[i]) count--;
            ans = max(ans, count);
        }
        final_answer = max(final_answer, ans);
    } 
    cout << final_answer << endl;
} 

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t = 1;
    cin >> t;
    while(t--) {
        solve();
    }
    
    return 0;
}