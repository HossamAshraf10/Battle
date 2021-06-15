#pragma once

template<class T>
class PriorityQueue
{
private:
    const static int MAX_SIZE = 300;
    T heap[MAX_SIZE];
    int size;
public:
    PriorityQueue();
    static int parent(int index);
    static int leftChild(int index);
    static int rightChild(int index);
    static void swap(T* x, T* y);
    void enqueue(T Node);
    T dequeue();
    bool dequeue(T& t);
    void maxHeapify(int index);
    bool isFull();
    bool isEmpty();
    //  ~priorityQueue();
};

template<typename T>
PriorityQueue<T>::PriorityQueue()
{
    size = 0;
}

template<class T>
int PriorityQueue<T>::parent(int index)
{
    return (index - 1) / 2;
}

template<class T>
int PriorityQueue<T>::leftChild(int index)
{
    return 2 * index + 1;
}

template<class T>
int PriorityQueue<T>::rightChild(int index)
{
    return 2 * index + 2;
}

template<class T>
void PriorityQueue<T>::swap(T* x, T* y)
{
    T temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

template<class T>
bool PriorityQueue<T>::isFull()
{
    if (size >= MAX_SIZE)
        return true;
    return false;
}
template<class T>
bool PriorityQueue<T>::isEmpty()
{
    if (size == 0)
        return true;
    return false;
}

template<class T>
void PriorityQueue<T>::enqueue(T Node)
{
    heap[size] = Node;
    size += 1;

    int i = size - 1;
    while (i != 0 && heap[PriorityQueue<T>::parent(i)] < heap[i])
    {
        PriorityQueue<T>::swap(&heap[PriorityQueue<T>::parent(i)], &heap[i]);
        i = PriorityQueue::parent(i);
    }
}

template<class T>
T PriorityQueue<T>::dequeue()
{
    if (size == 0)
        throw "error";
    T maxPriority = heap[0];
    heap[0] = heap[size - 1];
    size -= 1;
    maxHeapify(0);
    return maxPriority;
}template<class T>
bool PriorityQueue<T>::dequeue(T& t)
{
    if (isEmpty())
        return false;
    T maxPriority = heap[0];
    heap[0] = heap[size - 1];
    size -= 1;
    maxHeapify(0);
    t = maxPriority;
    return true;
}
template<class T>
void PriorityQueue<T>::maxHeapify(int i)
{
    int l = PriorityQueue<T>::leftChild(i);
    int r = PriorityQueue<T>::rightChild(i);
    int largest = i;

    if (r <= size && heap[r] > heap[i])
        largest = r;

    if (l <= size && heap[l] > heap[i])
        largest = l;

    if (l <= size && r <= size && heap[r] > heap[l])
        largest = r;



    if (largest != i)
    {
        T temp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = temp;
        maxHeapify(largest);
    }
}