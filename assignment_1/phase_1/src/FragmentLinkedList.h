#ifndef FRAGMENT_LINKED_LIST
#define FRAGMENT_LINKED_LIST

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include "IList.h"

template <class T>
class FragmentLinkedList : IList<T>
{
public:
    class Node;
    class Iterator;

protected:
    Node** fragmentPointers;
    int fragmentMaxSize;
    int count;
    int fragPNum;
    int fragPCap;
public:
    FragmentLinkedList(int fragmentMaxSize = 5)
    {
        this->count = 0;
        this->fragPCap = 2;
        this->fragPNum = 2;
        this->fragmentMaxSize = fragmentMaxSize;
        this->fragmentPointers = new Node * [2];
        this->fragmentPointers[0] = NULL;
        this->fragmentPointers[1] = NULL;
    }
    ~FragmentLinkedList();
    virtual void addFragment()
    {
        Node** newFragArray = new Node * [this->fragPCap + 1];
        for (int i = 0; i < this->fragPCap; ++i)
            newFragArray[i] = this->fragmentPointers[i];
        newFragArray[this->fragPCap] = NULL;
        delete[] this->fragmentPointers;
        this->fragmentPointers = newFragArray;
        this->fragPCap++;
    }
    virtual void add(const T& element);
    virtual void add(int index, const T& element);
    virtual T removeAt(int index);
    virtual bool removeItem(const T& item);
    virtual bool empty();
    virtual int size();
    virtual void clear();
    virtual T get(int index);
    virtual void set(int index, const T& element);
    virtual int indexOf(const T& item);
    virtual bool contains(const T& item);
    virtual std::string toString();

    virtual void print()
    {
        if (fragmentPointers[0] == NULL)
            std::cout << "There are nothing in here!!" << std::endl;
        else
        {
            Node* temp = fragmentPointers[0];
            while (temp != NULL)
            {
                std::cout << temp->data << " - ";
                temp = temp->next;
            }
        }
        std::cout << std::endl;
    }
    virtual void printFragPCap()
    {
        std::cout << "fragPCap is : " << this->fragPCap << std::endl;
    }
    virtual void printFragPNum()
    {
        std::cout << "fragPNum is : " << this->fragPNum << std::endl;
    }
    virtual int distance(Node* a, Node* b)
    {
        int c = 0;
        while (a != b)
        {
            a = a->next;
            c++;
        }
        return c;
    }
    virtual void printFragment(int index)
    {
        if (this->count == 0)
        {
            std::cout << "From printFragment(): There is nothing in the list!!" << std::endl;
            return;
        }
        if (index == this->fragPNum - 1)
        {
            std::cout << "From printFragment(): This is the end of the list." << std::endl;
            return;
        }
        if (index >= this->fragPNum)
        {
            std::cout << "From printFragment(): Out of range!!" << std::endl;
            return;
        }
        std::cout << "Fragment " << index << " contain: ";
        Node* ptr;
        if (index == this->fragPNum - 2)
        {
            ptr = this->fragmentPointers[this->fragPNum - 2];
            while (ptr != this->fragmentPointers[this->fragPNum - 1])
            {
                std::cout << ptr->data << ", ";
                ptr = ptr->next;
            }
            std::cout << ptr->data;
        }
        else
        {
            ptr = this->fragmentPointers[index];
            while (ptr->next != this->fragmentPointers[index + 1])
            {
                std::cout << ptr->data << ", ";
                ptr = ptr->next;
            }
            std::cout << ptr->data;
        }
        std::cout << std::endl;
    }
    Iterator begin(int index = 0);
    Iterator end(int index = -1);

public:
    class Node
    {
    private:
        T data;
        Node* next;
        Node* prev;
        friend class FragmentLinkedList<T>;

    public:
        Node()
        {
            next = 0;
            prev = 0;
        }
        Node(Node* next, Node* prev)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node* next, Node* prev)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    class Iterator
    {
    private:
        FragmentLinkedList<T>* pList;
        Node* pNode;
        int index;

    public:
        Iterator(FragmentLinkedList<T>* pList = 0, bool begin = true);
        Iterator(int fragmentIndex, FragmentLinkedList<T>* pList = 0, bool begin = true);
        Iterator& operator=(const Iterator& iterator);
        T& operator*();
        bool operator!=(const Iterator& iterator);
        void remove();
        void set(const T& element);
        Iterator& operator++();
        Iterator operator++(int);
    };
};

//done
template <class T>
FragmentLinkedList<T>::~FragmentLinkedList()
{

    Node* curr = this->fragmentPointers[0];
    while (this->fragmentPointers[0] != NULL)
    {
        curr = this->fragmentPointers[0];
        this->fragmentPointers[0] = this->fragmentPointers[0]->next;
        delete curr;
    }

    delete[] this->fragmentPointers;
}

//done
template <class T>
void FragmentLinkedList<T>::add(const T& element) /* done */
{
    //T copy = element;
    Node* temp = new Node(element, NULL, NULL);
    if (empty())
    {
        fragmentPointers[0] = temp;
        fragmentPointers[1] = temp;
    }
    else
    {
        if (this->distance(this->fragmentPointers[this->fragPNum - 2], this->fragmentPointers[this->fragPNum - 1]) == this->fragmentMaxSize - 1)
        {
            if (this->fragPNum == this->fragPCap)
            {
                this->addFragment();
            }
            this->fragmentPointers[this->fragPNum - 1]->next = temp;
            temp->prev = this->fragmentPointers[this->fragPNum - 1];
            this->fragmentPointers[this->fragPNum - 1] = this->fragmentPointers[this->fragPNum - 1]->next;
            this->fragmentPointers[this->fragPNum] = temp;

            this->fragPNum++;
        }
        else
        {
            this->fragmentPointers[this->fragPNum - 1]->next = temp;
            temp->prev = this->fragmentPointers[this->fragPNum - 1];
            this->fragmentPointers[this->fragPNum - 1] = this->fragmentPointers[this->fragPNum - 1]->next;
        }
    }
    this->count++;
}

//done
template <class T>
void FragmentLinkedList<T>::add(int index, const T& element)
{
    if (index > this->count || index < 0)
        throw std::out_of_range("The index is out of range!");
    if (index == this->count)
    {
        this->add(element);
        return;
    }
    Node* temp = new Node(element, NULL, NULL);
    if (index == 0)
    {
        this->fragmentPointers[0]->prev = temp;
        temp->next = this->fragmentPointers[0];

        if (this->distance(this->fragmentPointers[this->fragPNum - 2], this->fragmentPointers[this->fragPNum - 1]) == this->fragmentMaxSize - 1)
        {
            if (this->fragPCap == this->fragPNum)
                this->addFragment();

            for (int i = this->fragPNum - 2; i > 0; --i)
            {
                this->fragmentPointers[i] = this->fragmentPointers[i]->prev;
            }
            this->fragmentPointers[this->fragPNum] = this->fragmentPointers[this->fragPNum - 1];
            this->fragPNum++;
        }
        else
        {
            for (int i = this->fragPNum - 2; i > 0; --i)
            {
                this->fragmentPointers[i] = this->fragmentPointers[i]->prev;
            }
        }

        this->fragmentPointers[0] = temp;
    }
    else
    {
        int fragIndex = index / this->fragmentMaxSize;  //index of the fragment that will contain new element
        int inFragIndex = index % this->fragmentMaxSize;//index of the new element in side fragment

        Node* curr = this->fragmentPointers[fragIndex];
        int i = 0;
        while (i != inFragIndex && i < this->fragmentMaxSize)
        {
            curr = curr->next;
            i++;
        }

        if (this->distance(this->fragmentPointers[this->fragPNum - 2], this->fragmentPointers[this->fragPNum - 1]) == this->fragmentMaxSize - 1)
        {
            if (this->fragPCap == this->fragPNum)
                this->addFragment();

            for (int i = this->fragPNum - 2; i > fragIndex; --i)
            {
                this->fragmentPointers[i] = this->fragmentPointers[i]->prev;
            }
            this->fragmentPointers[this->fragPNum] = this->fragmentPointers[this->fragPNum - 1];
            this->fragPNum++;
        }
        else
        {
            for (int i = this->fragPNum - 2; i > fragIndex; --i)
            {
                this->fragmentPointers[i] = this->fragmentPointers[i]->prev;
            }
        }

        curr->prev->next = temp;
        temp->prev = curr->prev;
        temp->next = curr;
        curr->prev = temp;

        if (inFragIndex == 0)
            this->fragmentPointers[fragIndex] = this->fragmentPointers[fragIndex]->prev;
    }
    this->count++;
}

//done
template <class T>
T FragmentLinkedList<T>::removeAt(int index)
{
    if (index >= this->count || index < 0)
        throw std::out_of_range("The index is out of range!");

    int fragIndex = index / this->fragmentMaxSize;
    int inFragIndex = index % this->fragmentMaxSize;

    //find the index's node
    Node* curr = this->fragmentPointers[fragIndex];
    int c = 0;
    while (c != inFragIndex)
    {
        curr = curr->next;
        c++;
    }

    if (fragIndex != this->fragPNum - 2) //the node is not in the last fragment
    {
        //operation fromt (the fragment next to the fragment containning index's node) to (the penultimate fragment) 
        for (int i = fragIndex + 1; i < this->fragPNum - 2; ++i)
            this->fragmentPointers[i] = this->fragmentPointers[i]->next;

        //operation in the last fragment
        if (this->fragmentPointers[this->fragPNum - 2] == this->fragmentPointers[this->fragPNum - 1])
        {
            this->fragmentPointers[this->fragPNum - 1] = NULL;
            this->fragPNum--;
        }
        else
        {
            this->fragmentPointers[this->fragPNum - 2] = this->fragmentPointers[this->fragPNum - 2]->next;
        }

        //removing the node
        if (index == 0)
        {
            curr->prev = NULL;
            curr->next->prev = NULL;
            this->fragmentPointers[0] = curr->next;
            curr->next = NULL;
        }
        else
        {
            if (inFragIndex == 0)
                this->fragmentPointers[fragIndex] = this->fragmentPointers[fragIndex]->next;

            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            curr->prev = NULL;
            curr->next = NULL;
        }
    }
    else //the node is in the last fragment
    {
        if (this->fragmentPointers[this->fragPNum - 2] == this->fragmentPointers[this->fragPNum - 1]) //the two last fragPointers is pointing to the same node
        {
            this->fragmentPointers[this->fragPNum - 1] = NULL;
            this->fragmentPointers[this->fragPNum - 2] = this->fragmentPointers[this->fragPNum - 2]->prev;

            if (index != 0)
            {
                this->fragmentPointers[this->fragPNum - 2]->next = NULL;
                this->fragPNum--;
            }
        }
        else
        {
            if (index == this->count - 1)
            {
                this->fragmentPointers[this->fragPNum - 1] = curr->prev;
                curr->prev->next = NULL;
                curr->prev = NULL;
                curr->next = NULL;
            }
            else
            {
                if (inFragIndex == 0)
                    this->fragmentPointers[fragIndex] = this->fragmentPointers[fragIndex]->next;
                if (index == 0)
                {
                    curr->next->prev = curr->prev;
                    curr->prev = NULL;
                    curr->next = NULL;
                }
                else
                {
                    curr->prev->next = curr->next;
                    curr->next->prev = curr->prev;
                    curr->prev = NULL;
                    curr->next = NULL;
                }
            }
        }
    }

    //return data and delete node;
    T temp = curr->data;
    delete curr;
    this->count--;
    return temp;
}

//done
template <class T>
bool FragmentLinkedList<T>::removeItem(const T& item)
{
    //find the index's node
    Node* curr = this->fragmentPointers[0];
    int c = 0;
    while (curr != NULL && curr->data != item)
    {
        curr = curr->next;
        c++;
    }

    if (curr == NULL)
        return false;
    else this->removeAt(c);
    return true;
}

//done
template <class T>
bool FragmentLinkedList<T>::empty()
{
    return !fragmentPointers[0];
}

//done
template <class T>
int FragmentLinkedList<T>::size()
{
    return this->count;
}

//done
template <class T>
void FragmentLinkedList<T>::clear()
{
    Node* ptr = this->fragmentPointers[0];
    while (this->fragmentPointers[0] != NULL)
    {
        ptr = this->fragmentPointers[0];
        this->fragmentPointers[0] = this->fragmentPointers[0]->next;
        delete ptr;
    }

    for (int i = 0; i < this->fragPNum; ++i)
        this->fragmentPointers[i] = NULL;

    this->fragPNum = 2;
    this->count = 0;
}

//done
template <class T>
T FragmentLinkedList<T>::get(int index)
{
    if (index >= this->count || index < 0)
        throw std::out_of_range("The index is out of range!");
    Node* ptr = this->fragmentPointers[0];
    int c = 0;
    while (ptr != NULL && c != index)
    {
        ptr = ptr->next;
        c++;
    }

    if (ptr != NULL)
        return ptr->data;
    else throw std::out_of_range("The index is out of range!");
}

//done
template <class T>
void FragmentLinkedList<T>::set(int index, const T& element)
{
    if (index >= this->count || index < 0)
        throw std::out_of_range("The index is out of range!");
    Node* ptr = this->fragmentPointers[0];
    int c = 0;
    while (ptr != NULL && c != index)
    {
        ptr = ptr->next;
        c++;
    }

    if (ptr != NULL)
        ptr->data = element;
}

//done
template <class T>
int FragmentLinkedList<T>::indexOf(const T& item)
{
    Node* ptr = this->fragmentPointers[0];
    int index = 0;
    while (ptr != NULL && ptr->data != item)
    {
        ptr = ptr->next;
        index++;
    }

    if (ptr == NULL)
        return -1;
    else return index;
}

//done
template <class T>
bool FragmentLinkedList<T>::contains(const T& item)
{
    Node* ptr = this->fragmentPointers[0];
    while (ptr != NULL && ptr->data != item)
    {
        ptr = ptr->next;
    }

    if (ptr == NULL)
        return false;
    else return true;
}

//done
template <class T>
std::string FragmentLinkedList<T>::toString()
{
    std::stringstream ss;
    ss << "[";
    Node* ptr = this->fragmentPointers[0];

    if (this->count == 0)
        ss << "]";

    // TODO
    else
    {
        while (ptr->next != NULL)
        {
            ss << ptr->data << ", ";
            ptr = ptr->next;
        }
        ss << ptr->data;
        ss << "]";
    }
    // END: TODO

    return ss.str();
}


/************************* Iterator *************************/

//done
template <class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::begin(int index)
{
    return Iterator(index, this, true);
}

//done
template <class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::end(int index)
{
    return Iterator(index, this, false);
}

//done
template <class T>
FragmentLinkedList<T>::Iterator::Iterator(FragmentLinkedList<T>* pList, bool begin)
{
    this->pList = pList;
    if (begin == true)
    {
        if (pList == NULL)
        {
            this->pNode = NULL;
            this->index = -1;
        }
        else
        {
            this->pNode = pList->fragmentPointers[0];
            this->index = 0;
        }
    }
    else
    {
        this->pNode = NULL;
        if (pList == NULL)
        {
            this->index = 0;
        }
        else
        {
            this->index = pList->size();
        }
    }
}

//done
template <class T>
FragmentLinkedList<T>::Iterator::Iterator(int fragmentIndex, FragmentLinkedList<T>* pList, bool begin)
{
    if (fragmentIndex > pList->fragPNum - 1)
        throw std::out_of_range("The index is out of range!");
    this->pList = pList;
    if (begin == true)
    {
        if (pList == NULL)
        {
            this->pNode = NULL;
            this->index = -1;
        }
        else
        {
            this->pNode = pList->fragmentPointers[fragmentIndex];
            this->index = fragmentIndex * pList->fragmentMaxSize;
        }
    }
    else
    {
        if (pList == NULL)
        {
            this->pNode = NULL;
            this->index = 0;
        }
        else
        {
            if (fragmentIndex == pList->fragPNum - 2 || fragmentIndex == pList->fragPNum - 1 || fragmentIndex == -1)
            {
                this->pNode = NULL;
                this->index = pList->size();
            }
            else
            {
                this->pNode = pList->fragmentPointers[fragmentIndex + 1];
                this->index = (fragmentIndex + 1) * pList->fragmentMaxSize;
            }
        }
    }
}

//done
template <class T>
typename FragmentLinkedList<T>::Iterator& FragmentLinkedList<T>::Iterator::operator=(const Iterator& iterator)
{
    this->pList = iterator.pList;
    this->pNode = iterator.pNode;
    this->index = iterator.index;
    return *this;
}

//done
template <class T>
T& FragmentLinkedList<T>::Iterator::operator*()
{
    if (this->pNode == NULL)
        throw std::out_of_range("Segmentation fault!");
    return this->pNode->data;
}

//done
template <class T>
bool FragmentLinkedList<T>::Iterator::operator!=(const Iterator& iterator)
{
    if (this->pNode != iterator.pNode)
        return true;
    return false;
}

//done
template <class T>
void FragmentLinkedList<T>::Iterator::remove()
{
    if (this->pNode == NULL)
        throw std::out_of_range("Segmentation fault!");

    if (this->pNode == this->pList->fragmentPointers[0])
    {
        this->pNode = NULL;
        this->index = -1;
        this->pList->removeAt(0);
    }
    else
    {
        this->pNode = this->pNode->prev;
        this->pList->removeAt(this->index);
        this->index--;
    }
}

//done
template <class T>
void FragmentLinkedList<T>::Iterator::set(const T& element)
{
    if (this->pNode == NULL)
        throw std::out_of_range("Segmentation fault!");
    this->pNode->data = element;
}

//done
template <class T>
typename FragmentLinkedList<T>::Iterator& FragmentLinkedList<T>::Iterator::operator++()
{
    if (this->index == pList->size())
        throw std::out_of_range("Segmentation fault!");
    if (this->index == -1)
    {
        this->pNode = this->pList->fragmentPointers[0];
        this->index = 0;
    }
    else
    {
        this->pNode = this->pNode->next;
        this->index++;
    }
    return *this;
}

//done
template <class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::Iterator::operator++(int)
{
    if (this->index == pList->size())
        throw std::out_of_range("Segmentation fault!");
    Iterator previous = *this;
    if (this->index == -1)
    {
        this->pNode = this->pList->fragmentPointers[0];
        this->index = 0;
    }
    else
    {
        this->pNode = this->pNode->next;
        this->index++;
    }
    return previous;
}

#endif