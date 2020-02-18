from collections import defaultdict

class Graph:
    def __init__(self):
        self.graph = defaultdict(list)
        
    def addEdge(self, u, v):
        self.graph[u].append(v)
        
    def DFSUtil(self,v,visited):
        visited[v] = True
        print(v, end=" ")
        
        for i in self.graph[v]:
            if not visited[i]:
                self.DFSUtil(i,visited)
                
                
    def DFS(self, v):
        visited = [False]*len(self.graph)
        self.DFSUtil(v,visited)
