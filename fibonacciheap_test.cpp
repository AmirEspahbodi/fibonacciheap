#include<iostream>
#include<fstream>
#include<sstream>
#include"fibonacciheap.h"

using namespace std;

int main(int argc, char const *argv[])
{
    cout<<print("Create a file named 'fibonacciheapinit.txt', The numbers on each line of this file are shown a fibonacci heap\nfor create new fibonacci heap Create a file called 'fibonacciheapnew.txt' that has only one line", 2)<< endl;
    ifstream infile("fibonacciheapinit.txt");
    string line;
    vector<FibonacciHeap*> FBHS;
    while (getline(infile, line)) {
        FBHS.push_back(new FibonacciHeap());
        istringstream ss(line);
        int num;
        while (ss >> num){
            FBHS.back()->Fib_Heap_Insert(new Node(num));
        }
    }
    string command;
    // FBHS[0]->print_heap();
    
    cout<<"extract min fibo heap 0 ---------------------- "<<endl;
    Node* FBHS0MIN = FBHS[0]->Fib_Heap_Extract_Min();
    cout<<"FBHS0MIN = "<<FBHS0MIN->key<<endl;
    // FBHS[0]->print_heap();

    cout<<"extract min fibo heap 0 ---------------------- "<<endl;
    FBHS0MIN = FBHS[0]->Fib_Heap_Extract_Min();
    cout<<"FBHS0MIN = "<<FBHS0MIN->key<<endl;
    // FBHS[0]->print_heap();

    cout<<"extract min fibo heap 0 ---------------------- "<<endl;
    FBHS0MIN = FBHS[0]->Fib_Heap_Extract_Min();
    cout<<"FBHS0MIN = "<<FBHS0MIN->key<<endl;
    // FBHS[0]->print_heap();


    cout<<print("enter h\n", 2);    
    while(command != "q"){
        cin>>command;
        if (command=="h") {}
        else if (command=="nf") {}
        else if (command=="") {}
        else if (command=="") {}
        else if (command=="") {}
        else if (command=="") {}
        else if (command=="") {}
    }
    infile.close();
    return 0;
}
