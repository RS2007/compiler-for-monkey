#include <stdbool.h>

int number_of_digits(int number)
{
    int count = 1;
    while (number / 10 != 0)
    {
        count++;
    }
    return count;
}

bool check_odd(int number)
{
    if (number % 2 == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int exponent_int(int a, int n)
{
    int ans = 1;
    while (n > 0)
    {
        int last_bit = (n & 1);
        if (last_bit)
        {
            ans = ans * a;
        }
        a = a * a;
        n = n >> 1;
    }
    return ans;
}