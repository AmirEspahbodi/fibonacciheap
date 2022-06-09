#include<vector>
#include<math.h>
using namespace std;


string print(string TEXT,  int FORGROUND=0, int BACKGROUND=0){
    /*
    BACKGROUND_COLOR = {
        {0, "Default", "49" },
        {1, "Black", "40" },
        {2, "Red", "41" },
        {3, "Green", "42" },
        {4, "Yellow", "43" },
        {5, "Blue", "44" },
        {6, "Megenta", "45" },
        {7, "Cyan", "46" },
        {8, "Light Gray", "47" },
        {9, "Dark Gray", "100" },
        {10, "Light Red", "101" },
        {11, "Light Green", "102" },
        {12, "Light Yellow", "103" },
        {13, "Light Blue", "104" },
        {14, "Light Magenta", "105" },
        {15, "Light Cyan", "106" },
        {16, "White", "107" }
    };
    FORGROUND_COLOR = {
        {0, "Default", "39" },
        {1, "Black", "30" },
        {2, "Red", "31" },
        {3, "Green", "32" },
        {4, "Yellow", "33" },
        {5, "Blue", "34" },
        {6, "Magenta", "35" },
        {7, "Cyan", "36" },
        {8, "Light Gray", "37" },
        {9, "Dark Gray", "90" },
        {10, "Light Red", "91" },
        {11, "Light Green", "92" },
        {12, "Light Yellow", "93" },
        {13, "Light Blue", "94" },
        {14, "Light Magenta", "95" },
        {15, "Light Cyan", "96" },
        {16, "White", "97" }
    };
    */
    string FORGROUND_COLOR[] = {"39", "30", "31", "32", "33", "34", "35", "36", "37", "90", "91", "92", "93", "94", "95", "96", "97"};
    string BACKGROUND_COLOR[] = {"49", "40", "41", "42", "43", "44", "45", "46", "47", "100", "101", "102", "103", "104", "105", "106", "107"};
    return "\033[0;"+FORGROUND_COLOR[FORGROUND]+";"+BACKGROUND_COLOR[BACKGROUND]+"m"+TEXT+"\033[0m";
}


class Node {
    public:
        int id;
        Node* left;
        Node* right;
        Node* child;
        Node* father;
        int key;
        int degree;
        bool marked;
        Node(int key, int id){
            this->id=id;
            this->key=key;
            this->degree = 0;
            this->child = NULL;
            this->marked = false;
        }
        static void exchangeRef(Node** y, Node** x);
};

void Node::exchangeRef(Node** y, Node** x){
    Node* temp = *y;
    *y = *x;
    *x = temp;
}

class FibonacciHeap{
    public:
        Node* min;
        int n;
        FibonacciHeap(){
            this->min=NULL;
            this->n=0;
        }
    void Fib_Heap_Insert(Node *node);
    void Cut(Node* x, Node* y);
    void Cascading_Cut(Node* y);
    void Fib_Heap_Decrease_Key(Node* x, int key);
    static FibonacciHeap* Fib_Heap_Union(FibonacciHeap* H1, FibonacciHeap* H2);
    void print_heap();
    void Fib_Heap_Link(Node* y, Node** x, Node** start_node, Node** w);
    void consolidate();
    Node* Fib_Heap_Extract_Min();

};

void FibonacciHeap::print_heap(/* string* spc */){
    /*  depth first traversal, so we use stack data structures
        stack is a vector of <node,start_node> To navigate evry list (children or root, no different)*/
    struct node_node
    {
        Node *node1, *node2;
    };
    
    vector<node_node> stack = {{this->min, this->min}};
    /*  current is pointer to <node,_> first element of last entry of stack
        Indicates the list Which we are currently navigating it */
    Node **current = &stack.back().node1;
    while(true){
        this_thread::sleep_for(chrono::milliseconds(1));
        cout<<string(stack.size()-1, '_')<<print(to_string((*current)->key)+"-"+to_string((*current)->degree),((*current)->marked)?10:13)<<endl;
        if ((*current)->child){
            /* if current node hes child we must go for chldren first, and back for Continuation*/
            *current = (*current)->right;
            /* add child to stack */
            stack.push_back({(*current)->left->child, (*current)->left->child});
            /* current needs to be updated now */
            current = &stack.back().node1;
            continue;
        }
        *current = (*current)->right;
        /*  if <node,_> is equal to <_,start_node> it means that this list
            (it is root list or child list, Whatever it is) is scrolled */
        while (*current==stack.back().node2) {
            stack.pop_back();
            /* update current pointer to Previous entry in stack*/
            if (!stack.empty()) current = &stack.back().node1;
            else return;
        }
    }
    /*
    Node *iter=start;
    do {
        // this_thread::sleep_for(chrono::milliseconds(25));
        cout<<*spc<<iter->key<<"-"<<iter->degree<<endl;
        spc->push_back('_');
        if (iter->child) print_heap(H, iter->child, spc);
        spc->pop_back();
        iter = iter->right;
    } while(iter != start);
    */
}

void FibonacciHeap::Fib_Heap_Link(Node* y, Node** x, Node** start_node, Node** w){
    /* y will be x child */

    /* exchange x, y poitners if x.key is biger than y.key*/
    if ((*x)->key>y->key) Node::exchangeRef(&y, x);

    /*  here whatever y is, it will be remove from root list ,
        so if it same as what start_point/w point to , we must move start_node/w pointer to right sibling */
    if ((*w)==y) *w = (*w)->right;
    if ((*start_node)==y) *start_node = (*start_node)->right;

    /* remove y from root list (pike up y) */
    y->right->left = y->left;
    y->left->right = y->right;
    y->left=NULL;
    y->right=NULL;

    /* link y to x */
    y->marked = false;
    y->father = *x;
    (*x)->degree += 1;
    if (!(*x)->child){
        (*x)->child = y;
        y->left = y;
        y->right = y;
        return ;
    }
    (*x)->child->right->left = y;
    y->right = (*x)->child->right;
    y->left = (*x)->child;
    (*x)->child->right = y;

    return;
}

void FibonacciHeap::consolidate(){
    int Dn = sqrt(this->n)+1;
    Node* A[Dn];
    for (int i = 0; i < Dn; i++) A[i] = NULL;
    Node    *w = this->min, /* Pointer to root list nodes */
            *start_node = this->min, /* Points to the node from which we started navigating the root list */
            *x = NULL; /* always point to max(w, A[w.degree]), root list of heap */
    int d=0;
    /* for each node w in the root list of H */
    do {
        x=w;
        w=w->right;
        d=x->degree;
        while (A[d]!=NULL){
            /*  Maybe the full list be scrolled and we will return to the first place.
                so we must check that and skeep if needed */
            if (A[d]==x) break;
            Node *y=A[d];
            this->Fib_Heap_Link(y, &x, &start_node, &w);
            A[d]=NULL;
            d++;
        }
        A[d]=x;
    } while (w != start_node);
    for (int i=0; i<Dn; i++) if (A[i]) if (this->min->key > A[i]->key) this->min=A[i];
}

Node* FibonacciHeap::Fib_Heap_Extract_Min(){
    if (this->min == NULL)
        return NULL;
    if (this->min->child != NULL){
        /* add H.min childrens to root list, actualy instead of H->min */
        Node *start = this->min->child;
        do{
            start->father = NULL;
            start = start->right;
        } while(start != this->min->child);
        this->min->left->right = this->min->child->right;
        this->min->child->right->left = this->min->left;
        this->min->child->right = this->min->right;
        this->min->right->left = this->min->child;
    } else {
        /*if H.min not have child, just remove it*/
        this->min->right->left = this->min->left;
        this->min->left->right = this->min->right;
    }
    Node *hm=this->min;
    /*upadte H.min pointer */
    this->min = this->min->left->right;
    this->n--;

    /* consolidate */
    if (this->min->right != this->min) this->consolidate();
    return hm;
}

void FibonacciHeap::Fib_Heap_Insert(Node *node){
    /*it's clear*/
    node->father = NULL;
    if (this->min == NULL){
        this->min = node;
        node->left = node;
        node->right = node;
    } else {
        this->min->right->left = node;
        node->left = this->min;
        node->right = this->min->right;
        this->min->right = node;
        if (node->key < this->min->key){
            this->min = node;
        }
    }
    this->n++;
}

void FibonacciHeap::Cut(Node* x, Node* y){
    /* remove x from the child list of */
    if (x->right==x) y->child=NULL;
    else {
        if (x==y->child) y->child = x->right;
        x->left->right = x->right;
        x->right->left = x->left;
        x->right=NULL;
        x->left=NULL;
    }
    /* add x to the root of H */
    this->Fib_Heap_Insert(x);
    x->marked=false;
    y->degree-=1;
}

void FibonacciHeap::Cascading_Cut(Node* y){
    if (y->father){
        if (y->marked){
            Node *yfather=y->father;
            this->Cut(y, y->father);
            this->Cascading_Cut(yfather);
        } 
        else y->marked=true;
    }
}

void FibonacciHeap::Fib_Heap_Decrease_Key(Node* x, int key){
    if (x->key<=key){
        cout<<print("new key is greater than current key", 2)<<endl;
        return;
    }
    x->key = key;
    Node *xfather = x->father;
    if (xfather!=NULL && x->key < xfather->key){
        this->Cut(x, xfather);
        this->Cascading_Cut(xfather);
    }
    if (x->key < this->min->key) this->min = x;
}

FibonacciHeap* FibonacciHeap::Fib_Heap_Union(FibonacciHeap* H1, FibonacciHeap* H2){
    FibonacciHeap* NFH = new FibonacciHeap();
    NFH->min = H1->min;
    if (H1->min!=NULL && H2->min!=NULL){
        Node *h1mr = H1->min->right;
        H1->min->right = H2->min->right;
        H2->min->right->left = H1->min;
        H2->min->right = h1mr;
        h1mr->left = H2->min;
    }

    if (H2->min!=NULL && H2->min->key<H1->min->key) NFH->min = H2->min;
    NFH->n = H1->n + H2->n;

    delete H1;
    delete H2;

    return NFH;
}
