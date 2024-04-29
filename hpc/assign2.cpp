#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void seq_bubbleSort(int arr[], int n) {   
    for(int i=0; i<n; i++)
    {
        for(int j=0; j < n-i-1; j++)
        {
            if(arr[j] > arr[j+1])
            {
                swap(arr[j], arr[j+1]);
            }
        }
    }

}

void p_bubbleSort(int arr[], int n) {
    for(int i=0; i<n; i++)
    {
        if(i%2 == 0)
        {
            #pragma omp parallel for
            for(int j=0; j<n-1; j+=2)
                if(arr[j] > arr[j+1])
                    swap(arr[j], arr[j+1]);
        }
        else 
        {
            #pragma omp parallel for
            for(int j=1; j<n-1; j+=2)
                if(arr[j] > arr[j+1])
                    swap(arr[j], arr[j+1]);
        }
    }
}

void merge(int arr[], int start1, int end1, int start2, int end2) {
    int size = end1-start1+1 + end2-start2+1;
    int temp_arr[size];

    int i=start1, j=start2, k=0;
    while(i <= end1 && j <= end2)
    {
        if(arr[i] < arr[j])
        {
            temp_arr[k] = arr[i];
            i++;
            k++;
        }
        else 
        {
            temp_arr[k] = arr[j];
            j++;
            k++;
        }
    }
    while(i <= end1)
    {
        temp_arr[k] = arr[i];
        i++;
        k++;
    }
    while(j <= end2)
    {
        temp_arr[k] = arr[j];
        j++;
        k++;
    }


    for(int i=0; i<size; i++)
        arr[start1+i] = temp_arr[i];
}

void seq_mergeSort(int arr[], int start, int end) {
    if(end-start+1 <= 1)
        return;

    int mid = (start+end)/2;
    seq_mergeSort(arr, start, mid);
    seq_mergeSort(arr, mid+1, end);
    merge(arr, start, mid, mid+1, end);
}

void p_mergeSort(int arr[], int start, int end) {
    if(end-start+1 <= 1)
        return;

    int mid = (start+end)/2;
    #pragma omp parallel sections
    {
        #pragma omp section
            p_mergeSort(arr, start, mid);
        #pragma omp section       
            p_mergeSort(arr, mid+1, end);
    }
    merge(arr, start, mid, mid+1, end);
}

void copy(int dst[], int src[], int n) {
    for(int i=0; i<n; i++)
        dst[i] = src[i];
}

void print(int arr[], int n) {
    for(int i=0; i<n; i++)
        cout<<arr[i]<<" ";
    cout<<endl;
}

int main() {

    // Get the Input array
    int n;
    cout<<"Enter the size of array: ";
    cin>>n;
    int arr[n], temp_arr[n];

    for(int i=0; i<n; i++)
        arr[i] = rand() % 100;

    cout<<"\nRandom array generated is"<<endl;
    print(arr, n);    
    
    // Sequential bubble sort and time required
    copy(temp_arr, arr, n);
    auto start = chrono::high_resolution_clock::now();
    seq_bubbleSort(temp_arr, n);
    auto stop = chrono::high_resolution_clock::now();
    cout<<"\nSequential bubble sort output is "<<endl;
    print(temp_arr, n);
    cout<<"Time taken for sequential bubble sort = "<<chrono::duration_cast<chrono::microseconds>(stop-start).count()<<" microseconds."<<endl;

    // Parallel bubble sort and time required
    copy(temp_arr, arr, n);
    start = chrono::high_resolution_clock::now();
    p_bubbleSort(temp_arr, n);
    stop = chrono::high_resolution_clock::now();
    cout<<"\nParallel bubble sort output is "<<endl;
    print(temp_arr, n);
    cout<<"Time taken for parallel bubble sort = "<<chrono::duration_cast<chrono::microseconds>(stop-start).count()<<" microseconds."<<endl;

    // Sequential merge sort and time required
    copy(temp_arr, arr, n);
    start = chrono::high_resolution_clock::now();
    seq_mergeSort(temp_arr, 0, n-1);
    stop = chrono::high_resolution_clock::now();
    cout<<"\nSequential merge sort output is "<<endl;
    print(temp_arr, n);
    cout<<"Time taken for sequential merge sort = "<<chrono::duration_cast<chrono::microseconds>(stop-start).count()<<" microseconds."<<endl;


    // Parallel merge sort and time required
    copy(temp_arr, arr, n);
    start = chrono::high_resolution_clock::now();
    p_mergeSort(temp_arr, 0, n-1);
    stop = chrono::high_resolution_clock::now();
    cout<<"\nParallel merge sort output is "<<endl;
    print(temp_arr, n);
    cout<<"Time taken for parallel merge sort = "<<chrono::duration_cast<chrono::microseconds>(stop-start).count()<<" microseconds."<<endl;

    return 0;
}
