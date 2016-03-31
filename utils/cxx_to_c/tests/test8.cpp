

namespace ala {
    void f(char c,int x = 7, int z = 13 );
    bool f(int y=8);
}//test


int main()
{
    ala::f('1');
    if(!::ala::f());
}