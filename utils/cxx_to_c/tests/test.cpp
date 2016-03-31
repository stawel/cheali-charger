
namespace ala {

    typedef int ValueType;

    ValueType f()
    {
    }

    int zmienna = 6;
    int zmienna2 = 6;


    void g();
}


void ala::g()
{
    zmienna++;
    ala::zmienna2 = 6;
}


void fun(){
}

int main()
{
    int h = 3;
    ala::ValueType t = ala::f();

//    h+= zmienna ;
}
