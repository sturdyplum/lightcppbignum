#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

#define maxBigIntLength 2500//size of numbers

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

    bigInt operator/()
    {
        bigInt c;
        c.pos = (other.pos or this->pos) or (!other.pos and !this->pos);
        if(other.absGreater(*this)) return c;
        if(other == *this)
        {
            c.number[0] = 1;
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
    bigInt a(1);
    for(int i = 1; i <= 100; i++)
    {
        bigInt b(i);
        a = a*b;
    }
    a.print();
    return 0;
}
