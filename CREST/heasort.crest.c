#include <stdio.h>
#include <crest.h>

void swap(int *x, int *y)
{
        int _x = *x ;
        *x = *y ;
        *y = _x ;
}

void heapify(int arr[], int n, int i)
{
        int largest = i;
        int l = 2*i + 1;
        int r = 2*i + 2;

        if (l < n && arr[l] > arr[largest])
                largest = l;

        if (r < n && arr[r] > arr[largest])
                largest = r;

        if (largest != i)
        {
                swap(&(arr[i]), &(arr[largest]));
                heapify(arr, n, largest);
        }
}

void heapSort(int arr[], int n)
{
        for (int i = n / 2 - 1; i > 0; i--)
                heapify(arr, n, i);

        for (int i=n-1; i >= 0; i--)
        {
                swap(&(arr[0]), &(arr[i])) ;
                heapify(arr, i, 0);
        }
}

void printArray(int arr[], int n)
{
        for (int i=0; i<n; ++i)
                printf("%d ", arr[i]) ;
        printf("\n") ;
}

int main()
{
        int n = 6 ;
        CREST_int(n);
        if(n<=0 || n>1000){
                printf("N should not be negative number\n");
        }
        else{
                int arr[n];

                for(int i=0; i<n; ++i)
                        CREST_int(arr[i]);

                heapSort(arr, n);

                printArray(arr, n);
        }
}
