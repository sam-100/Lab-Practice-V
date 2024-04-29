// Implement Min, Max, Sum and Average operations using Parallel Reduction. Measure the performance of sequential and parallel algorithms.

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <omp.h>

using namespace std;

int seq_min(const vector<int> &arr) {
    int result = INT32_MAX;
    for(int i=0; i<arr.size(); i++)
        result = min(result, arr[i]);
    return result;
}

int p_min(const vector<int> &arr) {
    int result;

    #pragma omp parallel for reduction(min:result)
    for(int i=0; i<arr.size(); i++)
        result = min(result, arr[i]);
    return result;
}

int seq_max(const vector<int> &arr) {
    int result = INT32_MIN;

    for(int i=0; i<arr.size(); i++)
        result = max(result, arr[i]);
    return result;
}

int p_max(const vector<int> &arr) {
    int result = INT32_MIN;

    #pragma omp parallel for reduction(max:result)
    for(int i=0; i<arr.size(); i++)
        result = max(result, arr[i]);
    return result;
}

int seq_sum(const vector<int> &arr) {
    int result = 0; 

    for(int i=0; i<arr.size(); i++)
        result += arr[i];
    return result;
}

int p_sum(const vector<int> &arr) {
    int result = 0; 

    #pragma omp parallel for reduction(+:result)
    for(int i=0; i<arr.size(); i++)
        result += arr[i];
    return result;
}

int seq_avg(const vector<int> &arr) {
    double result = 0; 

    for(int i=0; i<arr.size(); i++)
        result += arr[i];
    return result/arr.size();
}

int p_avg(const vector<int> &arr) {
    double result = 0; 

    #pragma omp parallel for reduction (+:result)
    for(int i=0; i<arr.size(); i++)
        result += arr[i];
    return result/arr.size();
}

void perform_and_display_analysis(vector<int> arr, int (*seq_operation)(const vector<int>&), int (*p_operation)(const vector<int>&), string op_name) {
    int seq_result, p_result;
    
    auto start = chrono::high_resolution_clock::now();
    seq_result = seq_operation(arr);
    auto stop = chrono::high_resolution_clock::now();
    int seq_time = chrono::duration_cast<chrono::microseconds>(stop-start).count();
    
    start = chrono::high_resolution_clock::now();
    p_result = p_operation(arr);
    stop = chrono::high_resolution_clock::now();
    int p_time = chrono::duration_cast<chrono::microseconds>(stop-start).count();
    
    if(seq_result != p_result)
    {
        // show error ... 
        cout<<"ERROR: The result of sequential and parallel code is different!!!"<<endl;
        cout<<"Sequential result = "<<seq_result<<endl;
        cout<<"Parallel result = "<<p_result<<endl;
        return;
    }

    // displaying the result
    cout<<"The result of "<<op_name<<" operation is "<<seq_result<<endl;
    cout<<"The sequential algorithm takes "<<seq_time<<" microseconds."<<endl;
    cout<<"The parallel algorithm takes "<<p_time<<" microseconds."<<endl;
    cout<<endl;
}

void print(const vector<int> &arr) {
    for(int i=0; i<arr.size(); i++)
        cout<<arr[i]<<" ";
    cout<<endl;
}

vector<int> generateRandomArray(int n, int start, int end) {
    vector<int> arr;
    for(int i=0; i<n; i++)
        arr.push_back(rand() % (end-start) + start);
    return arr;
}

int main(int argc, char **argv) {
    int n;
    cout<<"Enter the size of array to be generated: ";
    cin>>n; 
    vector<int> arr = generateRandomArray(n, 0, 100);
    cout<<"Radnom array generated is: "<<endl;
    print(arr);
    cout<<endl<<endl;

    perform_and_display_analysis(arr, seq_min, p_min, "minimum");
    perform_and_display_analysis(arr, seq_max, p_max, "maximum");
    perform_and_display_analysis(arr, seq_sum, p_sum, "sum");
    perform_and_display_analysis(arr, seq_avg, p_avg, "average");

    return 0;
}