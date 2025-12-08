// 
// Joseph Staria: Project 3b
//

#include <iostream>

using namespace std;

extern "C" int DoSum(int, int);
extern "C" int DoSub(int, int);
extern "C" int DoFac(int);
extern "C" float* FillArray();

int main()
{
    std::cout << "Hello This Adds Numbers: \n";

    while (true) {
        int num, plus = 0;

        cout << "Enter Two Numbers: "; cin >> num >> plus;

        cout << "Your total summed was: " << DoSum(num, plus) << endl;
        cout << "Your total subtracted was: " << DoSub(num, plus) << endl;

        cout << "Enter Number: "; cin >> num;
        cout << "Your total factorial was: " << DoFac(num) << endl;

        cout << "Printed ASM array values: \n";

        float* array = FillArray();

    }

    return 0;
}