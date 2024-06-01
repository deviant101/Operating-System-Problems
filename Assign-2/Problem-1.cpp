#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>
using namespace std;

string items[8]={"eggs","bread","butter","cheese","jam","honey","chocolate","milk"};
int prices[8]={20,80,100,150,150,120,25,180};

void purchaseItems(vector<vector<int>>& cart){
    for(int i=0; i<8; i++)
        cout<<i<<" - "<<items[i]<<"\t\t"<<prices[i]<<endl;
    cout<<"8 - Exit"<<endl;

    while(true){
        int choice;
        cout<<"\nEnter the item number: ";
        cin>>choice;
        if(choice>=0 && choice<=7){
            int quantity=0;
            cout<<"Enter the quantity: ";
            cin>>quantity;
            cart[0].push_back(choice);
            cart[1].push_back(quantity);
        }
        else if(choice==8)
            break;
        else
            cout<<"Invalid choice"<<endl;
    }
}

struct receipt{

    vector<vector<int>> items_quantity;
    float total;
    float tax;
    float discount;
    float final_total;

    receipt(vector<vector<int>> items_quantity){
        this->items_quantity=items_quantity;
        this->total=0;
        this->tax=0;
        this->discount=0;
        this->final_total=0;
    }
    void display(){
        for(int i=0; i<items_quantity[0].size(); i++){
            cout<<items[items_quantity[0][i]]<<" : "<<items_quantity[1][i]<<endl;
        }
    }
};

void purchasedItemsBill(receipt *obj){
    cout<<"Thread - 1\n";
    cout<<"\nPurchased Items Bill\n\n";
    for(int i=0; i<obj->items_quantity[0].size(); i++){
        cout<<items[obj->items_quantity[0][i]]<<" = "<<obj->items_quantity[1][i]<<" x "<<prices[obj->items_quantity[0][i]]<<" = ";
        int val=prices[obj->items_quantity[0][i]]*obj->items_quantity[1][i];
        cout<<val<<endl;
        obj->total+=val;
    }
    cout<<"\nTotal : "<<obj->total<<endl<<endl;
    obj->final_total=obj->total;
}

void calculateTax(receipt *obj){
    sleep(0.5);
    cout<<"\nThread - 2\n";
    while(obj->total==0);
    sleep(0.5);

    obj->tax=obj->total*0.08;
    cout<<"\nTax : "<<obj->tax<<endl;
    cout<<"Taxed Sum : "<<obj->total<<" + "<<obj->tax<<" = "<<obj->total+obj->tax<<endl<<endl;
    obj->final_total+=obj->tax;
}

void calculateDiscount(receipt *obj){
    sleep(1);
    cout<<"\nThread - 3\n";

    while(obj->total==0);
    sleep(0.5);

    if(obj->total>250){
        obj->discount=obj->total*0.1;
        cout<<"\nDiscount : "<<obj->discount<<endl;
        cout<<"Discounted Sum : "<<obj->total<<" - "<<obj->discount<<" = "<<obj->total-obj->discount<<endl;
    }
    obj->final_total-=obj->discount;

}

void sortItems(receipt *obj){
    sleep(1.5);
    cout<<"\nThread - 4\n";
    for(int i=0; i<obj->items_quantity[0].size(); i++){
        for(int j=i+1; j<obj->items_quantity[0].size(); j++){
            if(prices[obj->items_quantity[0][i]]*obj->items_quantity[1][i] < prices[obj->items_quantity[0][j]]*obj->items_quantity[1][j]){
                swap(obj->items_quantity[0][i], obj->items_quantity[0][j]);
                swap(obj->items_quantity[1][i], obj->items_quantity[1][j]);
            }
        }
    }

    sleep(0.5);
    cout<<"Items\tPrices\n";
    for(int i=0; i<obj->items_quantity[0].size(); i++){
        cout<<items[obj->items_quantity[0][i]]<<"\t"<<prices[obj->items_quantity[0][i]]*obj->items_quantity[1][i]<<endl;
    }

    cout<<"\nFinal Total : "<<obj->final_total<<endl;
}



int main(){
    
    vector<vector<int>> cart(2);
    purchaseItems(cart);

    receipt r(cart);

    pthread_t threads[4];
    pthread_create (&threads[0],NULL,(void* (*)(void*))purchasedItemsBill, &r);
    pthread_create (&threads[1],NULL,(void* (*)(void*))calculateTax, &r);
    pthread_create (&threads[2],NULL,(void* (*)(void*))calculateDiscount, &r);
    pthread_create (&threads[3],NULL,(void* (*)(void*))sortItems, &r);
    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    pthread_join(threads[2],NULL);
    pthread_join(threads[3],NULL);

    return 0;
}