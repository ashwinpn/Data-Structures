class Node:
    def  __init__(self,key):
        self.val = key
        self.left = None
        self.right = None
    
    def insert(root,node):
        if root is None:
            root  = node
        else:
            if root.val < node.val:
                if root.right is None:
                    root.right = node 
                else:
                    insert(root.right,node)
            
            elif root.val > node.val:
                if root.left is None:
                    root.left = node
                else:
                    insert(root.left,node)
