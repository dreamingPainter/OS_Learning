# include<stdio.h>
# include<math.h>
/*
void run(int *nums,int *left,int *right);
int My_qSort(int* nums, int numsSize);

int My_qSort(int* nums, int numsSize) {
    int *left,*right;
    right = nums+numsSize-1;
    left = nums;
    run(nums,left,right);
}

void run(int *nums,int *left,int *right)
{
    int base = *left;
    int *end = right;
    int *start = left;
    int tmp;
    while(right>left)
    {
        while (left<right&&base<=(*right))//高位小于基数中断
            right--;
        while(left<right&&base>(*left))//低位大于基数中断
            left++;
        //将小于base的高位数和大于base的低位数交换
        tmp = *left;
        *left = *right;
        *right = tmp;
    }
    if(*left>*end)//减少遍历次数
    {
        tmp = *left;
        *left = *end;
        *end = *left;        
    }
    else left++;
    if(left)
        run(nums,start,left-1);
    run(nums,left+1,end);

}


*/

void swap(int *x, int *y) {
    int t = *x;
    *x = *y;
    *y = t;
}

void quick_sort_recursive(int arr[], int start, int end) {
    if (start >= end)
        return;
    int mid = arr[end];
    int left = start, right = end - 1;
    while (left < right) {
        while (arr[left] < mid && left < right)
            left++;
        while (arr[right] >= mid && left < right)
            right--;
        swap(&arr[left], &arr[right]);
    }
    if (arr[left] >= arr[end])
        swap(&arr[left], &arr[end]);
    else
        left++;
    if (left)
        quick_sort_recursive(arr, start, left - 1);
    quick_sort_recursive(arr, left + 1, end);
}

void quick_sort(int arr[], int len) {
    quick_sort_recursive(arr, 0, len - 1);
}

# define count 11
int main()
{
    int list[count]={1,3,2,4,5,6,7,8,6,2,222};
    quick_sort(list,count);
    for (size_t i = 0; i < count; i++)
    {
        printf("%d ",list[i]);
    }
    
}