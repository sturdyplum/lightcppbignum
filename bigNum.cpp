#include <iostream>
#include <cstring>
#include <cstdio>
#include <random>
#include <time.h>

using namespace std;

#define maxBigIntLength 300//size of numbers

struct bigInt{
    int number[maxBigIntLength];//number is stored here backwards
    bool pos;//pos = true if number is positive

	//default constructor
    bigInt() {this->pos = true,  memset(this->number, 0, sizeof this->number);}

	//integer constructor
    bigInt(int i)
    {
        if(i < 0) this->pos = false;
        else this->pos = true;
        i = i<0?i*-1:i;
        memset(this->number, 0, sizeof this->number);
        int index = 0;
        while(i != 0)
        {
            this->number[index++] = i%10;
            i/=10;
        }
    }

	//string constructor
    bigInt(string s)
    {
        this->pos= true;
        int n = s.length(), i = 0;
        if(s[0] == '-') this->pos = false, i++;
        memset(this->number, 0, sizeof this->number);
        for(; i < n; i++)
        {
            this->number[i] = s[n-i-1]-'0';
        }
    }

	//returns true if |this| > |other|
    bool absGreater(bigInt &other)
    {
        int i = maxBigIntLength-1;
        while(this->number[i] == other.number[i] and i >= 0)i--;
        if(i < 0) return false;
        return (this->number[i] > other.number[i]);
    }

	//adds a and b assums that they are either both positive or bot negative
    static bigInt addBig(bigInt &a, bigInt &b)
    {
        bigInt c;
        for(int i = 0; i < maxBigIntLength; i++)
        {
            if(i+1 != maxBigIntLength) c.number[i+1] += (a.number[i]+b.number[i]+c.number[i])/10;
            c.number[i] = (a.number[i]+b.number[i]+c.number[i]) % 10;
        }
        return c;
    }

	//subtracts b from a assumes that both are positive and that a is greater than b
    static bigInt subBig(bigInt &a, bigInt &b)
    {
        bigInt c;
        memset(c.number, 0, sizeof c.number);
        for(int i = 0; i < maxBigIntLength-1; i++)
        {
            c.number[i] = a.number[i]-b.number[i]+c.number[i];
            if(c.number[i] < 0) c.number[i]+= 10, c.number[i+1]--;
        }
        return c;
    }

	//handles addition operator
    bigInt operator+(bigInt& other)
    {
		//if both are positive or both are negative then just add and make the sign equal to either
        if((this->pos && other.pos)||(!this->pos && !other.pos))
        {
            bigInt c = addBig(*this,other);
            c.pos = other.pos;
            return c;
        }//if one is positive and one is negative, subtract the one with the smallest absolute value from the one with the greatest and then make the sign equal to whatever the greatest is
        else if(this->absGreater(other))
        {
            bigInt c = subBig(*this,other);
            c.pos = this->pos;
            return c;
        }
        else
        {
            bigInt c = subBig(other,*this);
            c.pos = other.pos;
            return c;
        }

    }

    bigInt operator-(bigInt& other)
    {
		//switch the sign of other then add then switch it back
        other.pos = !other.pos;
        bigInt c = *this + other;
        other.pos = !other.pos;
        return c;
    }

    bigInt operator*(bigInt& other)
    {//warning do not multiply 2 numbers whos n is > maxBigIntLength/2 or you will get a wrong answer
        bigInt c;
        c.pos = (other.pos and this->pos) or (!other.pos and !this->pos);
        for(int i = 0; i < maxBigIntLength/2; i++)
        {
            int carry = 0, digit, j = i;
            for(; j < maxBigIntLength/2 + i; j++)
            {
                digit = c.number[j]+(this->number[i]*other.number[j-i])+carry;
                carry = digit/10;
                c.number[j] = digit % 10;
            }
            if(carry)
            {
                digit = c.number[j] + carry;
                carry = digit/10;
                c.number[j] = digit%10;
            }
        }
        return c;
    }

    void operator>>(int a)
    {//left shift a (divide by 10^a)
        int i;
        for(i = 0; i < maxBigIntLength - a; i++)
        {
            this->number[i] = this->number[i+a];
           // cout << number[i];
        }
        for(;i < maxBigIntLength; i++)
        {
            this->number[i] = 0;
        }
    }

    bigInt operator/(bigInt &other)
    {//while i have been able to improve this it now runs in (9n)^2 I might be able to improve this further and make it just n^2
        bigInt c;
        c.pos = (other.pos and this->pos) or (!other.pos and !this->pos);
        if(other.absGreater(*this)) return c;
        if(other == *this)
        {
            c.number[0] = 1;
            return c;
        }
        bigInt temp = other, temp2 = *this;
        bigInt zero(0);
        bigInt ans(0);
        int t2 = maxBigIntLength-1, t1 = t2;
        while(this->number[t2] == 0) t2--;
        while(other.number[t1] == 0) t1--;
        string x(max(t2-t1,1),'0');
        x[0] = '1';
        bigInt mul(x);
        while(mul > zero)
        {
            temp = other*mul;
            while(temp2.absGreater(temp) or temp2 == temp)
            {
                temp2 = temp2 - temp;
                ans = ans + mul;
            }
            mul>>1;
        }
        return ans;

    }

    bool probablePrime(int k)
    {
        bigInt one(1),two(2),three(3);
        bigInt n = *this;
        if(n == one) return false;
        if(n == two or n == three) return true;
        if(n.number[0]%2 == 0) return false;
        bigInt d=n-one, hold = d;
        while(d.number[0]%2 == 0)
        {
            d = d/two;
        }
        for(int i = 0; i < k; i++)
        {
            bigInt a = genRand(two,hold);
            bigInt x = power(a,d,n);
            if(x == one or x == hold) return true;
            while(!(d == hold))
            {
                x = (x*x)%n;
                d = d * two;
                if(x == one) return false;
                if(x == hold) return true;
            }
        }
        return false;

    }
    static bigInt genRand(bigInt &low, bigInt &high)
    {
        bigInt range = high-low;
        bigInt temp, val;
        int index = 0;
        while((temp < range or temp == range) and index < maxBigIntLength)
        {
            int x = rand()%10;
            temp.number[index] = x;
            if(temp < range or temp == range)
            {
                val.number[index] = temp.number[index];
                index++;
            }
        }
        return val + low;
    }

    static bigInt power(bigInt &n, bigInt pow, bigInt &mod)
    {
        bigInt zero, one(1), two(2);
        if(pow == zero) return one;
        if(pow == one) return n;
        if(pow.number[0] % 2 == 0)
        {
            pow = pow/two;
            bigInt temp = power(n,pow,mod);
            temp = temp * temp;
            return temp %mod;
        }
        else
        {
            bigInt temp = power(n,(pow/two),mod);
            bigInt temp2 = power(n,(pow/two)+one,mod);
            bigInt c = (temp * temp2)%mod;
            return c;
        }
    }
    bool operator==(bigInt &other)
    {//if signs are not equal then return false otherwise step through number array making sure all indicies are equal
        if(this->pos != other.pos) return false;
        for(int  i = 0; i < maxBigIntLength; i++)
        {
            if(this->number[i] != other.number[i]) return false;
        }
        return true;
    }

    bool operator>(bigInt &other)
    {//check signs then check right most non equal number in the number array
        if(this->pos and !other.pos) return true;
        if(other.pos and !this->pos) return false;
        if(this->pos and other.pos) return this->absGreater(other);
        return other.absGreater(*this);
    }

    bool operator<(bigInt &other)
    {//flip and check that also not equal too (could probably speed this up if implemented on its own since you wouldn't have to run equals)
        if(!(*this == other) and !(*this > other)) return true;
        return false;
    }

    bigInt operator%(bigInt & other)
    {
        bigInt divisor = *this/other, temp = divisor*other, ans = *this - temp;
        return ans;

    }

    void print()
    {//prints out the big int without leading zeros
        int i = maxBigIntLength-1;
        if(!this->pos) printf("-");
        while(this->number[i] == 0 and i>=0) i--;
        if(i == -1){
            printf("0");
            return;
        }
        for(;i>=0;i--)printf("%d",this->number[i]);
    }
};

int main()
{
    srand(time(NULL));
    bigInt l("961748941");
    if(l.probablePrime(5)) cout << "ASDSAD";
    return 0;
}