#include <iostream>
#include <vector>

using namespace std;

vector<float> input_numbers();

int main()
{
    vector<float> numbers = input_numbers();
    float sum_of_numbers;

    for(int i; i<numbers.size(); i++)
    {
        sum_of_numbers += numbers[i];
    }
    
    cout << "Promedio de los numeros: " << sum_of_numbers/numbers.size() << endl;
    return 0;
}


vector <float> input_numbers()
{
    vector<float> numbers;
    float new_number;
    
    while(true)
    {
        cin >> new_number;
        if(new_number == 0)
        {
            return numbers;
        }
        numbers.push_back(new_number);
    }
}

