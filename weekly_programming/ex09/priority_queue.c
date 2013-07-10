
 /**********************************************************************************************
*** Filename:
***         priority_queue.c
*** Copyright (c) 2013 Wistron
*** All rights reserved.
***
*** Description:     
***  1 堆
***
***	 书中的“优先级队列”是基于“完全二叉树”，请基于“完全三叉树”来实现“优先级队列”。 
***	
***	要求使用模板（指定队列中元素的类型为T）定义一个C++类来完成这一任务： 
***	template<class T>
***	class priqueue {
***	    priqueue(int maxsize);      // init set S to empty    
***	   void insert(T t);           // add t to S
***	    T extractmin();             // return smallest in S   
***	};
***
*** Author/Creat Date:
***         Albert Yang, Jun 10,13
***
*** Modification History:
***         Jun 11, 13
*** Note:
***
************************************************************************************************/
#include <iostream>  
using namespace std;  
  
  
template<class T>  
class priqueue {  
private:  
    int n, maxsize;  
    T   *x;  
    void swap(int &i, int &j)//exchange the num
    {   T t = i; i = j; j = t; }  


public:  
    priqueue(int m)//initialize the array
    {   maxsize = m;  
        x = new T[maxsize+1];  
        n = 0;  
    }  

/*
*@p: father node
*@i: child node
*/
    void insert(T t)  
    {   
		int i, p;  
        x[++n] = t; //insert the num at the end 
        for (i = n; i > 1 && x[p=(i+1)/3] > x[i]; i = p)      
			swap(x[p], x[i]);  
                 
    }  
          

/*
*@c=3*i: middle child node
*@c-1: left child node
*@c+1: right child node
*@i: father node
*@min_index: the minimum index of c, c+1 and c-1 
*/
    T extractmin()
    {                
		int i, c;
		T t = x[1];
		x[1] = x[n--];
		T min_index;

		for(i=1;(c=3*i)+1 <= n; i = c)
		{
			min_index = x[c-1]<x[c] ? c-1 : c;
			min_index = x[c+1]<x[min_index] ? c+1 : min_index; 
			

			if(x[i] <= x[min_index])
				break;
			swap(x[i],x[min_index]);

			c++;
		}

        return t;  
    }  
    

    void print()  
    {  
        for (int i = 1; i < n+1; i++) //print the array
             cout << x[i] << " ";  
    }  
      
};  
  
template<class T>  
void pqsort(T v[],T n)  
{   priqueue<T> pq(n);   
    int i,data;
	int min;
	for (i = 0; i < n; i++)  //initialize the array
		pq.insert(v[i]);  
     
    cout<<"after init\n";  
    pq.print(); 


	while(1)
	{
		cout<<"\nplease input insert number>>:";
		cin>>data;
		
		if(data == 0)
			break;
		
		pq.insert(data); //insert the input number
		n++;
		
		pq.print();
	}	    
	
	min = pq.extractmin();

	cout<<"the smallest num is <<:"<<min;
	cout<<"\n";
	cout<<"\nthe result array is<<:\n";
	pq.print();
	cout<<"\n";

}  
  
  
int main()  
{   

	int n = 10;  
    int i, v[n];
    /*
	*initialize with 10 numbers
	*/  
    for (i = n; i < n+10; i++)  
        v[i-n] = i;        
    pqsort(v,n);  

    return 0;  
}

