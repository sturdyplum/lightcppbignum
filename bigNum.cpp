#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

#define maxBigIntLength 2500

struct bigInt{
    int number[maxBigIntLength];
    bool pos;

    bigInt() {this->pos = true,  memset(this->number, 0, sizeof this->number);}
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

    bool absGreater(bigInt &other)
    {
        int i = maxBigIntLength-1;
        while(this->number[i] == other.number[i] and i >= 0)i--;
        return (this->number[i] > other.number[i]);
    }
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

    bigInt operator+(bigInt& other)
    {
        if((this->pos && other.pos)||(!this->pos && !other.pos))
        {
            bigInt c = addBig(*this,other);
            c.pos = other.pos;
            return c;
        }
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
        other.pos = !other.pos;
        bigInt c = *this + other;
        other.pos = !other.pos;
        return c;
    }

    bigInt operator*(bigInt& other)
    {
        bigInt c;
        c.pos = (other.pos or this->pos) or (!other.pos and !this->pos);
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

    void print()
    {
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
    return 0;
}
