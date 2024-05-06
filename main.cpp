#include <iostream>
#include <cmath>
#include <list>
#include <vector>
using namespace std;

class Data {
private:
    int sourceVertex;
    int destinationVertex;
    int edgeCost;
public:
    void setSourceVertex(int sV) {
        this->sourceVertex = sV;
    }
    void setDestinationVertex(int dV) {
        this->destinationVertex = dV;
    }
    void setEdgeCost(int eC) {
        this->edgeCost = eC;
    }
    int getSourceVertex() {
        return sourceVertex;
    }
    int getDestinationVertex() {
        return destinationVertex;
    }
    int getEdgeCost() {
        return edgeCost;
    }
    Data() {
        sourceVertex = 0;
        destinationVertex = 0;
        edgeCost = 0;
    }
    void print() {
        cout << sourceVertex << " - " << destinationVertex << " -> " << edgeCost << endl;
    }
    bool operator==(const Data& second) {
        return (this->edgeCost == second.edgeCost);
    }
    bool operator<(const Data& second) {
        return (this->edgeCost < second.edgeCost);
    }
    bool operator>(const Data& second) {
        return (this->edgeCost > second.edgeCost);
    }
};

template <typename T> class Node {
private:
    T* data;
    Node<T>* leftChild;
    Node<T>* rightChild;
    Node<T>* parent;
public:
    Node(T* data) {
        this->data = data;
        leftChild = nullptr;
        rightChild = nullptr;
        parent = nullptr;
    }
    void print() {
        data->print();
    }
    bool operator==(const Node<T>& second) {
        return (this->getData() == second.getData());
    }
    bool operator<(const Node<T>& second) {
        return (this->getData() < second.getData());
    }
    bool operator>(const Node<T>& second) {
        return (this->getData() > second.getData());
    }
    void setData(T* data) {
        this->data = data;
    }
    void setLeftChild(Node<T>* LC) {
        this->leftChild = LC;
    }
    void setRightChild(Node<T>* RC) {
        this->rightChild = RC;
    }
    void setParent(Node<T>* parent) {
        this->parent = parent;
    }
    T* getData() {
        return data;
    }
    Node<T>* getLC() {
        return leftChild;
    }
    Node<T>* getRC() {
        return rightChild;
    }
    Node<T>* getParent() {
        return parent;
    }
};

template <typename T> class BinaryHeap {
private:
    Node<T>* root;
    int numberOfElements;
    int height;
    Node<T>* last;
    void updateHeight() {
        height = ceil(log2(numberOfElements + 1));
    }
public:
    void setRoot(Node<T>* root) {
        this->root = root;
    }
    void setNumberOfElements(int numElements) {
        this->numberOfElements = numElements;
    }
    void setHeight(int H) {
        this->height = H;
    }
    Node<T>* getRoot() {
        return root;
    }
    int getNumberOfElements() {
        return numberOfElements;
    }
    int getHeight() {
        return height;
    }
    BinaryHeap() {
        root = nullptr;
        numberOfElements = 0;
        height = 0;
    }
    BinaryHeap(T* data) {
        Node<T>* newNode = new Node<T>(data);
        root = newNode;
        numberOfElements = 1;
        height = 1;
    }

    void swapNodes(Node<T>* a, Node<T>* b) {
        T* temp = a->getData();
        a->setData(b->getData());
        b->setData(temp);
    }

    void insertElement(T* data) {
        Node<T> *nodeToInsert = new Node<T>(data);
        if (root == nullptr) {
            root = nodeToInsert;
        } else {
            string path = binaryNum(numberOfElements + 1);
            path = path.substr(1); // Remove the first character because it is always '1' and represents the root
            Node<T> *current = root;
            for (int i = 0; i < path.length() - 1; ++i) {  // Go until the second last character
                if (path[i] == '0') {
                    current = current->getLC();
                } else {
                    current = current->getRC();
                }
            }
            if (path.back() == '0') {
                current->setLeftChild(nodeToInsert);
            } else {
                current->setRightChild(nodeToInsert);
            }
            nodeToInsert->setParent(current); // Link parent
        }
        numberOfElements++;
        updateHeight();
        while (nodeToInsert->getParent() != nullptr &&
               (nodeToInsert->getData()) < (nodeToInsert->getParent()->getData())) {
            swapNodes(nodeToInsert, nodeToInsert->getParent());
            nodeToInsert = nodeToInsert->getParent();  // Move up to the parent
        }
    }

    string binaryNum(int size) {
        string binarynumber = "";
        while (size != 0) {
            binarynumber = (char)(size % 2 + '0') + binarynumber;
            size = size / 2;
        }
        return binarynumber;
    }

    Node<T>* getLastNode() {
        string path = binaryNum(numberOfElements);
        path = path.substr(1); // Skip the root bit
        Node<T>* current = root;
        for (char step : path) {
            current = (step == '0') ? current->getLC() : current->getRC();
        }
        return current;
    }


    /*Node<T> deleteMin() {
        Node<T>* smallest = root;
        if (numberOfElements == 0) {
            return nullptr;
        }
        string path = binaryNum(numberOfElements);
        path = path.substr(1); // Remove the first character because it is always '1' and represents the root
        Node<T> *max = root;
        for (int i = 0; i < path.length(); ++i) {
            if (path[i] == '0') {
                max = max->getLC();
            } else {
                max = max->getRC();
            }
        }
        max->setRightChild(smallest->getRC());
        max->setLeftChild(smallest->getLC());
        max->getParent()->setLeftChild(nullptr);
        max->getParent()->setRightChild(nullptr);

    }*/
    Node<T>* deleteMin() {
        if (root == nullptr) {
            return nullptr; // Heap is empty
        }

        Node<T>* minNode = root; // This is the minimum element to be returned later.

        if (numberOfElements == 1) {
            root = nullptr; // If there's only one element, just clear the root.
            numberOfElements--;
            return minNode;
        }

        // Get the last node in the heap.
        Node<T>* lastNode = getLastNode();

        // Move the last node data to the root.
        root->setData(lastNode->getData());

        // Remove the last node by adjusting its parent's pointer.
        Node<T>* parentOfLast = lastNode->getParent();
        if (parentOfLast->getLC() == lastNode) {
            parentOfLast->setLeftChild(nullptr);
        } else {
            parentOfLast->setRightChild(nullptr);
        }

        numberOfElements--; // Decrement the size of the heap.
        updateHeight(); // Update the height of the heap.

        // Restore the heap property by downheapifying the new root.
        downheapify(root);

        return minNode; // Return the original root node.
    }


    void downheapify(Node<T>* node) {
        while (node != nullptr) {
            Node<T>* smallest = node;
            if (node->getLC() != nullptr && *(node->getLC()->getData()) < *(smallest->getData())) {
                smallest = node->getLC();
            }
            if (node->getRC() != nullptr && *(node->getRC()->getData()) < *(smallest->getData())) {
                smallest = node->getRC();
            }

            if (smallest == node) break; // Heap property satisfied

            swapNodes(node, smallest);
            node = smallest;
        }
    }

};




const int VERTEXCOUNT = 5;

template <typename T>
void runPrims(int G[VERTEXCOUNT][VERTEXCOUNT], BinaryHeap<T>* binaryHeap) {
    vector<int> visited;
    visited.push_back(0);
    vector<int> unvisited;
    for (int i = 1; i < VERTEXCOUNT; i++) {
        unvisited.push_back(i);
    }
    for (int i = 0; i < VERTEXCOUNT; i++) {
        for (int j = 0; j < VERTEXCOUNT; j++) {
            if (G[visited[i]][j] != 0) {
                Data *newData = new Data();
                newData->setSourceVertex(visited[0]);
                newData->setDestinationVertex(j);
                newData->setEdgeCost(G[visited[0]][j]);
                binaryHeap->insertElement(newData);
            }
        }
    }
    binaryHeap->deleteMin();

    for (int j = 0; j < VERTEXCOUNT; ++j) {
        if (G[0][j] != 0) {
            Data* newData = new Data();
            newData->setSourceVertex(0);
            newData->setDestinationVertex(j);
            newData->setEdgeCost(G[0][j]);
            binaryHeap->insertElement(newData);
        }
    }


    while (binaryHeap->getRoot() != nullptr) {
        Node<Data>* minEdgeNode = binaryHeap->deleteMin();
        if (!minEdgeNode) break;

        Data* minEdge = minEdgeNode->getData();
        int src = minEdge->getSourceVertex();
        int dest = minEdge->getDestinationVertex();
        int cost = minEdge->getEdgeCost();

        if (!visited[dest]) {
            cout << src << " - " << dest << " -> " << cost << endl;
            visited[dest] = true;

            for (int j = 0; j < VERTEXCOUNT; ++j) {
                if (!visited[j] && G[dest][j] != 0) {
                    Data* newData = new Data();
                    newData->setSourceVertex(dest);
                    newData->setDestinationVertex(j);
                    newData->setEdgeCost(G[dest][j]);
                    binaryHeap->insertElement(newData);
                }
            }
        }
    }
}






int main() {
    int G[5][5] = {{0, 3, 65, 0, 0},
                   {3, 0, 85, 20, 45},
                   {65, 85, 0, 41, 77},
                   {0, 20, 41, 0, 51},
                   {0, 45, 77, 51, 0}};

    BinaryHeap<Data> binaryHeap;  // Create a binary heap instance
    runPrims(G, &binaryHeap);  // Run Prim's algorithm


}
