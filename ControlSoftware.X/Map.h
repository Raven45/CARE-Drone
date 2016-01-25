/*******************************************************************************
 * Copyright 2016 Department of Engineering, Harding University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


 * Created on January 12, 2016, 11:59 AM
*******************************************************************************/

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
    
    DataType& operator[](unsigned int index) {
        
        try {
            return TopOfList[index];
        }
        catch (...) {
            return 0;
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

