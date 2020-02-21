# Finding the Lowest Common Ancestor (LCA) for a binary tree.
class node:
    def __init__(self,val):
        self.key = val 
        self.left = None 
        self.right = None 
        
def findPath(root,path,k):
    if root is None:
        return False
    path.append(root.key)
    if root.key == k:
        return True
    if ((root.left != None and findPath(root.left, path, k)) or
            (root.right!= None and findPath(root.right, path, k))): 
        return True
        
    path.pop()
    return False
    
def lca(root,n1,n2):
    path1 = []
    path2 = []
    
    if (not findPath(root, path1, n1) or not findPath(root, path2, n2)): 
        return -1
    i = 0 
    while(i < len(path1) and i < len(path2)): 
        if path1[i] != path2[i]: 
            break
        i += 1
    return path1[i-1]
