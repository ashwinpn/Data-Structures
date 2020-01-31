class Node:
    def __init__(self, data):
        self.data = data
        self.next = None 
        
class LinkedList:
    def __init__(self):
        self.head = None 
        
    def push(self, new_data):
        newNode = Node(new_data)
        newNode.next = self.head 
        self.head = newNode 
        
    def deleteNode(self, key):
        temp = self.head
        
        if temp is not None:
            if temp.data == key:
                self.head = temp.next
                temp = None
                return 
            
        while (temp is not None):
            if temp.data == key:
                break
            prev = temp
            temp = temp.next 
            
        if temp == None:
            return
        
        prev.next = temp.next
        
        temp = None 
