#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef hand_mode
#define start(n); test(n);
#else
#define start(n); gen(n);
#endif // hand_mode

void shell_sort(int n, double * a, int * p_cnt_of_comparisons, int * p_cnt_of_swaps)
{
    int cnt_of_comparisons = 0, cnt_of_swaps = 0;
    for (int h = n / 2; h > 0; h /= 2)
    {
        for (int i = 0; i < n - h; ++i)
        {
            for (int j = i + h; j < n; j++)
            {
                cnt_of_comparisons++;
                if (a[j] < a[i])
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


void test(int n)
{
    double * a = malloc(sizeof(double) * n);
    for (int i = 0; i < n; ++i)
        scanf("%lf", &a[i]);
    int cnt1 = 0, cnt2 = 0;
    shell_sort(n, a,&cnt1, &cnt2);
    printf("comp = %d, swaps = %d\n", cnt1, cnt2);
    print_array(n, a);
    free(a);
}
int print_array(int len, double * a)
{
    for (int i = 0; i < len; ++i)
        printf("%f ", a[i]);
    printf("\n");
}
//int check()

int main()
{
    int n; scanf("%d", &n);
    start(n);
    return 0;
}
