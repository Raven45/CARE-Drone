/* 
 * File:   Map.h
 * Author: aburns2
 *
 * Created on January 12, 2016, 11:59 AM
 */

#ifndef MAP_H
#define	MAP_H

template <class KeyType, class DataType>
class Map {
    
public:
    class Node {
    public:
        Node() {}
        ~Node() {}
        DataType Data;
        KeyType Key;
        Node* Next;
    };
    
    Map() {
        TopOfList = 0;
    }
    
    ~Map() {
        
        Destructor(TopOfList->Next);
        delete TopOfList;
        TopOfList = 0;
    }
    
    bool Insert(KeyType Key, DataType Data, Node* P) {
        
        if (P != 0) {
            return Insert(Key, Data, P->Next);
        }
        else {
            try {
                P->Next = new Node;
                P->Next->Data = Data;
                P->Next->Key = Key;
                P->Next->Next = 0;
                return true;
            }
            catch (...) {
                return false;
            }
        }
    }
    
    DataType& operator[](const KeyType Key) {
        
        try {
            return Seek(Key, TopOfList);
        }
        catch (...) {
            Node R;
            R.Data = 0;
            R.Key = 0;
            R.Next = 0;
            return R;
        }
    }
private:
    Node* TopOfList;  
    
    void Destructor(Node* P) {
        
        while (P->Next != 0) {
            Destructor(P->Next);
            delete P;
            P = 0;
        }
    }
    
    DataType& Seek(KeyType Key, Node* P) {
        
        if (P != 0 && P->Key == Key) {
            return P;
        }
        else {
            return Seek(Key, P->Next);
        }
    }
};

#endif	/* MAP_H */

