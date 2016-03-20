

namespace ala {
    void f(char c,int x = 7, int z = 13 );
    void f(int y=8);
}


int main()
{
    ala::f('1');
    ala::f();
}