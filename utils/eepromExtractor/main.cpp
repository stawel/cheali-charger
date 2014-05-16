#include <vector>
#include <iostream>
#include <cstdio>

#include "extractor.h"

#include "8.1.5/extractor.h"

using namespace std;

std::vector<Extractor*> e;

void init()
{
    e.push_back(get_8_1_5());
}

#define MAX 4096
int main(int argc, char * argv[])
{
    init();
    if(argc != 2) {
        cerr << "Error, run:\n eepromExtractor eeprom.bin\n";
    } else {
        char buf[MAX];
        FILE * pFile;
        size_t result;
        cerr << "reading: "<< argv[1] <<"\n";
        pFile = fopen ( argv[1] , "rb" );
        result = fread (buf,1,MAX,pFile);
        cerr << "size: " << result << "\n";
        for(std::vector<Extractor*>::iterator i=e.begin();i != e.end(); i++) {
            cerr << "testing: " << (*i)->getName() << " size: "<< (*i)->getSize() << "\n";
            if(result < (*i)->getSize()) {
                cerr << "ERROR: epprom size to small \n";
            }
            (*i)->load(buf, result);
            if((*i)->isValid()) {
                cerr << "epprom found!\ntoString:\n";
                cout << (*i)->toString();
            } else {
                cerr << "not valid!\n";
            }
        }
    }
}