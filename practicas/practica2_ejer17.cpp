/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

void generar_procesos(int altura);

int main()
{
    int n;
    cout << "Ingrese altura del arbol: "; 
    cin >> n;
    
    generar_procesos(n);
    
    return 0;
}

void generar_procesos(int altura)
{
    
    if(altura <= 0)
    {
        return;
    }
    
    int pid = fork();
    
    if(pid == 0)
    {
        
        generar_procesos(altura-1);
        generar_procesos(altura-1);
    }
    else if (pid < 0)
    {
        wait(NULL);
    }
    else
    {
        exit(1);
    }
}