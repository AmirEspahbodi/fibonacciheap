#include<iostream>
#include<vector>
#include<limits>
#include<chrono>
#include<thread>
#include<math.h>
#include<unordered_map>
#include"dijkstra_fibonacciheap.h"
using namespace std;

struct adjacency_list_element {
    Node *node;
    int d;
};

struct edge {
    Node *v1, *v2;
};

void print_current_result(
    unordered_map<string, int> *names_ids, vector<Node*> *nodes,
    vector<vector<adjacency_list_element>> *adjacency_list, FibonacciHeap* FBH ){
        cout<<"names_ids: "<<endl;
        for (auto& it: (*names_ids)) cout << it.first <<" "<<it.second << endl;

        cout<<"nodes: "<<endl;
        for (Node* node: (*nodes)) cout << node->id << " " << node->key << endl;

        cout<<"adjacency_list: "<<endl;
        for (int i=0 ; i<(*adjacency_list).size(); i++) {
            cout<<"for "<<i<<"th node : ";
            for (adjacency_list_element al: (*adjacency_list)[i])
                cout<<al.node->id<<" "<<al.node->key<<" | ";
            cout<<endl;
        }

        cout<<"fibonacci heap: "<<endl;
        FBH->print_heap();
}

void input_pre_proc(
    unordered_map<string, int> *names_ids, vector<Node*> *nodes, vector<string> *fre, 
    vector<vector<adjacency_list_element>> *adjacency_list, FibonacciHeap** FBH ){
    int n, p, id=0;
    string inp, proc1;
    cin>>p>>n;
    vector<string> line_strs;
    getchar();
    for (;p;p--) {
        getline(std::cin, inp);
        for (int i=0; i<inp.size(); i++){
            if (inp[i]=='.' && (inp[i+1]==',' || inp[i+1]==':')){ 
                proc1.push_back(inp[i]);
                line_strs.push_back(proc1);
                if ((*names_ids)[proc1]==0){
                    // cout<<proc1<<endl;
                    (*names_ids)[proc1] = id+1; // if 0 => cus a bug on 0th element repeat
                                                // we must di "-1" on every (*names_ids)[proc1] 
                    nodes->push_back(new Node(numeric_limits<int>::max(),id));
                    adjacency_list->push_back({});
                    id++;
                }
                proc1.clear();
                if (inp[i+1]==':') break;
                i+=2;
            }
            else proc1.push_back(inp[i]);
        }
        // for (int i=0; i<line_strs.size(); i++) cout<<line_strs.at(i)<<"|";
        // cout<<endl;
        for (int i=0; i<line_strs.size(); i++) {
            int stri_id = (*names_ids)[line_strs[i]]-1;
            for (int j=0; j<line_strs.size(); j++){
                adjacency_list_element al = {nodes->at((*names_ids)[line_strs[j]]-1),1};
                if (stri_id==al.node->id) continue;
                adjacency_list->at(stri_id).push_back(al);
            }
        }
        line_strs.clear();
    }

    Node *vinit = (*nodes)[(*names_ids)["Erdos, P."]-1];

    for (adjacency_list_element al: (*adjacency_list).at(vinit->id))
        al.node->key=1;

    for (;n;n--){
        getline(std::cin, inp);
        (*fre).push_back(inp);
    }

    (*FBH) = new FibonacciHeap();
    for(Node* node: *nodes){
        if (node->id == vinit->id) continue;
        (*FBH)->Fib_Heap_Insert(node);
    }
}

void post_proc(
        int scenario, unordered_map<string, int> *names_ids,
        vector<Node*> *nodes, vector<string> *fre ){
    cout<<"Scenario "<<scenario<<endl;
    Node *node;
    for (string str: *fre){
        node=(*nodes)[(*names_ids)[str]-1];
        string key =  ( node->key>=numeric_limits<int>::max() ||
                        node->key<=-numeric_limits<int>::max()
                        )
                        ? "infinity"
                        : to_string(node->key);
        cout<<str<<" "<<key<<endl;
    }
}

vector<edge> dijkstra(Node *vinit, FibonacciHeap *FBH, vector<vector<adjacency_list_element>> *L){
    vector<edge> F;
    int n=(*L).size(), i;
    Node *touch[n], *vnear;
    for (i=0; i<n; i++) touch[i] = vinit;
    for (int j=0 ; j<n-1 ; j++){
        
        vnear = FBH->Fib_Heap_Extract_Min();
        
        F.push_back({touch[vnear->id], vnear});

        vector<adjacency_list_element> vnear_al = (*L)[vnear->id];

        for (i=0; i<vnear_al.size(); i++){
            if ( vnear->key + vnear_al[i].d < vnear_al[i].node->key ){
                vnear_al[i].node->key = vnear->key + vnear_al[i].d;
                touch[vnear_al[i].node->id] = vnear;
            }
        }
    }
    return F;
}

void start(){
    int scenarios;
    cin>>scenarios;
    for (int i=1; i<=scenarios; i++){
        unordered_map<string, int> names_ids;
        vector<Node*> nodes;
        vector<vector<adjacency_list_element>> adjacency_list;
        vector<string> fre;
        FibonacciHeap* FBH;

        input_pre_proc(&names_ids, &nodes, &fre, &adjacency_list, &FBH);
        /*
        cout<<" procces input result -------------------------------- "<<endl;
        print_current_result(&names_ids, &nodes, &adjacency_list, FBH);
        cout<<" -------------------------------- "<<endl;
        */

        Node *vinit=nodes[names_ids["Erdos, P."]-1];
        dijkstra(vinit, FBH, &adjacency_list);
        /*
        cout<<" after running dijkstra -------------------------------- "<<endl;
        print_current_result(&names_ids, &nodes, &adjacency_list, FBH);
        cout<<" -------------------------------- "<<endl;
        */

        // cout<<" final result -------------------------------- "<<endl;
        post_proc(i,&names_ids, &nodes, &fre);
        
        for (i=0; i<nodes.size(); i++) delete nodes[i];
        delete FBH;
    }
}

int main(int argc, char const *argv[])
{
    start();
    return 0;
}
 