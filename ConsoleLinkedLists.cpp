// ConsoleLinkedLists.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

template <typename t>
struct TypeDetector {
    static const int isType = 0;
};

template <>
struct TypeDetector<int> {
    static const int isType = 1;
};

template <>
struct TypeDetector<float> {
    static const int isType = 2;
};

template <>
struct TypeDetector<char> {
    static const int isType = 3;
};



template<typename type>
struct Node {

    type* ptr;
    Node<type>* next;
    Node<type>* previous;
};


template<typename type>
class LinkedList {
private:

    int listType;
    unsigned int count;
    Node<type>* headNode;
    Node<type>* lastNode;

    void SwapNodes(Node<type>* one, Node<type>* two) { // Swaps two node values, from  node*
        Node<type>* store = new Node<type>;
        store->ptr = one->ptr;
        one->ptr = two->ptr;
        two->ptr = store->ptr;
        delete(store);
    }

    Node<type>* GetNode(unsigned int index) { // Returns the node at the index (0 based)
        if (index > count - 1) {
            printf("GetNode(Index) - Index outside array bounds");
            return nullptr;
        }
        Node<type>* node = headNode;
        for (unsigned int currentIndex = 0; currentIndex < index; currentIndex++) {
            node = node->next;
        }
        return node;
    }

    void printStr(char* ptr) { //Prints a string from a char ptr
        while (*ptr != '\0') {
            printf("%c", *ptr);
            ptr++;
        }
    }


    void QuickSort(unsigned int mindex, unsigned int maxdex) { // Implementation of Quick Sort

        if (mindex < maxdex && mindex != maxdex) {
            int partition = QSPartition(mindex, maxdex);
            if (partition - 1 > 0 && (partition - 1 - mindex) > 0) {
                QuickSort(mindex, partition - 1);
            }
            if (partition + 1 < maxdex - 1 && (maxdex - partition + 1) > 0) {
                QuickSort(partition + 1, maxdex);
            }


        }
    }

    int QSPartition(unsigned int mindex, unsigned int maxdex) {
        // Get the size of the section to be partitioned
        int size = maxdex + 1;

        // Get the pivot node (the node at the end of the section)
        Node<type>* pivot = GetNode(maxdex);

        // Swap the pivot with the node at the start of the section
        SwapNodes(GetNode(static_cast<int>(mindex)), pivot);

        // Check if the pivot is not null
        if (pivot != nullptr) {
            // Loop to partition the section
            while (mindex < maxdex && maxdex > 0 && mindex < size) {
                // Get nodes at current mindex and maxdex positions
                Node<type>* fromLeft = GetNode(mindex);
                Node<type>* fromRight = GetNode(maxdex - 1);

                int leftComp = 0; // To store comparison result for the left node
                int rightComp = 0; // To store comparison result for the right node

                // Compare nodes based on listType
                switch (listType) {
                case(1 || 2): // Numeric comparisons
                    // Compare left node with pivot
                    if (*(fromLeft->ptr) == *(pivot->ptr)) {
                        leftComp = 0;
                    }
                    else if (*(fromLeft->ptr) > *(pivot->ptr)) {
                        leftComp = 1;
                    }
                    else if (*(fromLeft->ptr) < *(pivot->ptr)) {
                        leftComp = 0;
                    }

                    // Compare right node with pivot
                    if (*(fromRight->ptr) == *(pivot->ptr)) {
                        rightComp = 0;
                    }
                    else if (*(fromRight->ptr) > *(pivot->ptr)) {
                        rightComp = 1;
                    }
                    else if (*(fromRight->ptr) < *(pivot->ptr)) {
                        rightComp = 0;
                    }
                    break;

                case(3): // String comparisons
                    // Compare left node with pivot using strcmp
                    leftComp = strcmp((char*)fromLeft->ptr, (char*)pivot->ptr);

                    // Compare right node with pivot using strcmp
                    rightComp = strcmp((char*)fromRight->ptr, (char*)pivot->ptr);
                    break;
                }

                // Swap nodes if needed and adjust indices
                if (leftComp > 0 && rightComp < 0) {
                    // Swap left and right nodes
                    SwapNodes(fromLeft, fromRight);

                    // Move indices towards the center
                    mindex++;
                    maxdex--;
                }
                else {
                    // Move the mindex if left node is less than or equal to pivot
                    if (leftComp <= 0) {
                        mindex++;
                    }
                    // Move the maxdex if right node is greater than or equal to pivot
                    if (rightComp >= 0) {
                        maxdex--;
                    }
                }
            }
        }

        // Swap the pivot back to its correct position
        Node<type>* max = GetNode(mindex);
        SwapNodes(pivot, max);

        // Return the index where the pivot is now located
        return mindex;
    }

public:

    LinkedList() { //Constructor
        count = 0;                              //Number of nodes
        headNode = nullptr;                     //Root node
        lastNode = nullptr;                     //Node at the end of the list
        listType = TypeDetector<type>::isType;  //Multi type list
        if (listType == 0) {
            throw;
        }
    }

    ~LinkedList() { // De-constructor
        DeleteAll();
    }

    int GetCount() { //Returns how many nodes are in list
        return count;
    }

    void Append(type* ptr) { // Appends a new node to the list
        Node<type>* node = new Node<type>;
        node->ptr = ptr;
        if (count == 0) {
            headNode = node;
            lastNode = node;
            node->next = nullptr;
            node->previous = nullptr;
        }
        else {
            node->previous = lastNode;
            lastNode->next = node;
            node->next = nullptr;
            lastNode = node;
        }
        count++;
    }

    void ReadList() { // Prints the entire list
        Node<type>* current = headNode;
        if (headNode != nullptr) {
            switch (listType) {
            case(1): // Different printing process for each data type
                try { // Integer
                    while (current != nullptr) {
                        printf("%d", *(current->ptr));
                        current = current->next;
                        if (current != nullptr) {
                            printf(",");
                        }
                    }
                }
                catch (const std::exception& e) {};

            case(2): // Float
                while (current != nullptr) {
                        printf("%f", (float)*(current->ptr));
                        current = current->next;
                    if (current != nullptr) {
                        printf(",");
                    }
                }
            case(3): // Char
                while (current != nullptr)
                {
                    printStr((char*)(current->ptr));
                    current = current->next;
                    if (current != nullptr) {
                        printf(",");
                    }

                }
                printf("\n");
            default:
                break;
            }
        
        }
        else {
            printf("No head node present"); // No list nodes
        }
        printf("\n");       
    }

    void Delete(unsigned int index) { // Deletes a node at desired indexs
        if (index < count) {
            int previousNodeIndex = index - 1;
            int nextNodeIndex = index + 1;

            if (previousNodeIndex < 0) { // delete head node
                Node<type>* nextNodePtr = headNode->next;
                delete(headNode);
                headNode = nextNodePtr;
            }
            else { // delete node at index
                Node<type>* previousNodePtr = headNode;
                Node<type>* nextNodePtr;
                Node<type>* deletionNodePtr;
                unsigned int currentNode = 0;
                while (currentNode < previousNodeIndex) {
                    currentNode++;
                    previousNodePtr = previousNodePtr->next;
                }
                deletionNodePtr = previousNodePtr->next;
                previousNodePtr->next = deletionNodePtr->next;
                delete(deletionNodePtr);
            }

        }
    }

    void DeleteAll() { // Deletes all nodes iteratively
        for (unsigned int currentNode = 0; currentNode < count; currentNode++) {
            Delete(0);
        }
    }

    void InvertList() { //Inverts all the words in the list
        Node<type>* store = new Node<type>;
        for (unsigned int currentNode = 0; currentNode < count / 2; currentNode++) {
            Node<type>* start = GetNode(currentNode);
            Node<type>* end = GetNode(count - 1 - currentNode);
            store->ptr = start->ptr;
            start->ptr = end->ptr;
            end->ptr = store->ptr;

        }
        delete(store);
    }

    void Sort() {
        printf("Quick Sort - Total %d\n", count);
        QuickSort(0, count - 1);
        ReadList();
    }


};

int main()
{
    char str[3] = "A";
    char str2[3] = "B";
    char str3[3] = "C";
    char str4[3] = "D";
    char str5[3] = "E";
    char str6[3] = "F";
    char str7[3] = "G";

    char* ptr = str;
    char* ptr2 = str2;
    char* ptr3 = str3;
    char* ptr4 = str4;
    char* ptr5 = str5;
    char* ptr6 = str6;
    char* ptr7 = str7;

    LinkedList<char> list;
    list.Append(ptr6);
    list.Append(ptr3);
    list.Append(ptr2);
    list.ReadList();
    list.Sort();

    int* one = new int(1);
    int* two = new int(2);
    int* three = new int(3);

    LinkedList<int> intList;
    intList.Append(two);
    intList.Append(three);
    intList.Append(one);
    intList.ReadList();
    intList.Sort();
    return 0;

}