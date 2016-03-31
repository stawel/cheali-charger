

#define fun(x) f(x)


namespace ala {
void f(int);
    int var;
}

int main(){
    ala::fun(ala::var);
}


