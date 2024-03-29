#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int cmp(const void * a, const void * b)
{
    if (fabs(*(const double *)a) < fabs(*(const double *)b)) return -1;
    if (fabs(*(const double *)a) > fabs(*(const double *)b)) return 1;
    return 0;
}

int inverse_cmp(const void * a, const void * b) /// for sorting in reverse order in fuction generate_double_array()
{
    if (fabs(*(const double *)a) > fabs(*(const double *)b)) return -1;
    if (fabs(*(const double *)a) < fabs(*(const double *)b)) return 1;
    return 0;
}

void print_array(int len, double * a)
{
    for (int i = 0; i < len; ++i)
        printf("%.5f ", a[i]);
    printf("\n\n");
}

void shell_sort(int n, double * a, int * p_cnt_of_comparisons, int * p_cnt_of_swaps)
{
    int cnt_of_comparisons = 0, cnt_of_swaps = 0;
    for (int h = n / 2; h > 0; h /= 2) ///h - bias/offset
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = i + h; j < n; j+= h)
            {
                cnt_of_comparisons++;
                if (fabs(a[j]) < fabs(a[i]))
                {
                    cnt_of_swaps++;
                    double tmp = a[i];
                    a[i] = a[j];
                    a[j] = tmp;
                }
            }
        }
    }
    *p_cnt_of_comparisons = cnt_of_comparisons;
    *p_cnt_of_swaps = cnt_of_swaps;
}


void sift_down(int n, int i, double * heap, int * p_cnt_of_comparisons, int * p_cnt_of_swaps) ///i - index of element that should be sifted
{
    while (1)
    {
        /// sons of i: 2*i+1 ans 2*i+2
        int max_son = i; ///index of the greatest son
        if (2 * i + 1 < n && fabs(heap[2 * i + 1]) > fabs(heap[i]))
            max_son = 2 * i + 1;
        if (2 * i + 2 < n && fabs(heap[2 * i + 2]) > fabs(heap[max_son])) ///if right son exists and is the greatest
            max_son = 2 * i + 2;
        (*p_cnt_of_comparisons) += 2;
        if (max_son == i) ///heap is correct (no sons or they are smaller)
            return;
        double tmp = heap[i];
        heap[i] = heap[max_son];
        heap[max_son] = tmp;
        ++*p_cnt_of_swaps;
        i = max_son;
    }
}

void heap_sort(int n, double * a, int * p_cnt_of_comparisons, int * p_cnt_of_swaps)
{
    ///build a heap
    for (int i = n /2; i >= 0; --i)
        sift_down(n, i, a, p_cnt_of_comparisons, p_cnt_of_swaps);
    int heap_size = n; ///it will decrease to 0
    ///sort using a heap to choose the greatest
    while (heap_size > 1)
    {
        double tmp = a[0];
        a[0] = a[heap_size - 1];
        a[heap_size - 1] = tmp;
        heap_size--; ///add maximun to the end and sift new root
        ++*p_cnt_of_swaps;
        sift_down(heap_size, 0, a, p_cnt_of_comparisons, p_cnt_of_swaps);
    }
}

void generate_double_array(int n, double * a, double * b, double * c, int type) ///1 - sorted, 2 - reverse order, 3,4 - random order
{
    union
    {
        double d;
        unsigned char a[sizeof(double)];
    } u;
    for (int i = 0; i < n; ++i)
    {
        do
        {
            for (int p = 0; p < sizeof(double); ++p) ///generates 8 bytes that will be interpreted as double
                u.a[p] = rand();
        } while (!isfinite(u.d));
        a[i] = u.d;
    }
    if (type == 1)
        qsort(a, n, sizeof(double), cmp);
    else if(type == 2)
        qsort(a, n, sizeof(double), inverse_cmp);
    for (int i = 0; i < n; ++i)
        b[i] = c[i] = a[i];
}

int check(int n, double * origin, double * result)
{
    for (int i = 0; i < n; ++i)
        if (origin[i] != result[i]) return 0;
    return 1;
}

void test(int n)
{
    double * origin = malloc(sizeof(double) * n); ///for checking result of sort 1
    double * set1 = malloc(sizeof(double) * n);
    double * set2 = malloc(sizeof(double) * n);
    srand(time(NULL));
    for (int type_of_data = 1; type_of_data <= 4; ++type_of_data)
    {
        generate_double_array(n, origin, set1, set2, type_of_data);
        qsort(origin, n, sizeof(double), cmp);
        switch (type_of_data)
        {
            case 1:
                printf("-> sorted:\n");
                break;
            case 2:
                printf("-> reverse order\n");
                break;
            default: printf("-> random\n");
        }
        ///1
        int cnt1 = 0, cnt2 = 0;
        shell_sort(n, set1, &cnt1, &cnt2);
        if (!check(n, origin, set1))
            fprintf(stderr, "Error: Wrong Shell sort\n");
        printf("Shell sort: comp = %d, swaps = %d\n", cnt1, cnt2);
        ///2
        cnt1 = cnt2 = 0;
        heap_sort(n, set2, &cnt1, &cnt2);
        if (!check(n, origin, set2))
            fprintf(stderr, "Error: Wrong heap sort\n");
        printf("Heap sort: comp = %d, swaps = %d\n\n", cnt1, cnt2);
    }
    free(origin);
    free(set1);
    free(set2);
}


int main()
{
    int n;
    scanf("%d", &n);
    test(n);
    return 0;
}

