#include<vector>
#include<queue>


namespace seq {
    void bfs_seq(const std::vector<std::vector<int> >& graph, std::vector<bool>& used, int start_vertex_id, std::vector<int>& result) {
        std::queue<int> q;
        result[start_vertex_id] = 0;
        q.push(start_vertex_id);
        while (!q.empty())  {
            int v = q.front();
            q.pop();
            for (int i = 0; i < graph[v].size(); i++) {
                int u = graph[v][i]; 
                if (!used[u]) {
                    used[u] = true;
                    q.push(u);
                }
                result[u] = std::min(result[u], result[v] + 1);
            }
        }
    }
}