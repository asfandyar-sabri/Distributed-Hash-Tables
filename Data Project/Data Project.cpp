
#include<cmath>
#include<time.h>
#include "List_Circular.h"
#include <functional>

int main()
{
    int isp, mach;
    int F_isp;
    int opt;
    hash<string> str_hash;

    cout << "\nEnter identifier space: ";
    cin >> isp;

    F_isp = pow(2, isp);


    cout << "Your machine range is between 0 to " << F_isp - 1 << endl;

    cout << "\nHow many machines you want to make: ";
    cin >> mach;

    while (mach > F_isp)
    {
        cout << "\nMachine range is 0 to " << F_isp << " according to the identifier space.\nPlease enter again: ";
        cin >> mach;
    }

    int* r_mach = new int[mach];

    srand(time(0));
    do
    {
        cout << "\nPress '0' to stop\n";
        cout << "Press '1' if you want to generate random machines\n";
        cout << "Press '2' if you want to enter machine ID's\n";
        cout << endl;

        cin >> opt;
        switch (opt)
        {
            case 0:
                break;

            case 1:
            {

                for (int i = 0; i < mach; i++)
                {
                    int num = rand() % F_isp;

                    r_mach[i] = num;

                    for (int j = 0; j < i; j++)
                    {
                        if (num == r_mach[j])
                        {
                            i--;
                        }
                    }
                }

                cout << "Machine ID's are: ";

                for (int i = 0; i < mach; i++)
                {
                    cout << r_mach[i] << "    ";
                }
                cout << endl << endl;
                break;
            }

            case 2:
            {
                cout << "\nPlease Enter " << mach << " numbers: ";

                for (int i = 0; i < mach; i++)
                {
                    int num;
                    cin >> num;

                    r_mach[i] = num;
                    
                    if (num > F_isp-1 || num<0)
                    {
                        cout << "\nMachine ID must be in the range 0 to " << F_isp - 1 << " ." << endl;
                        i--;
                    }

                    for (int j = 0; j < i; j++)
                    {
                        if (num == r_mach[j])
                        {
                            cout << "\nMachine ID already present\nPlease enter again: ";
                            i--;
                        }

                    }
                }

                cout << "\nMachine ID's are: ";
                for (int i = 0; i < mach; i++)
                {
                    cout << r_mach[i] << "    ";
                }
                cout << endl;

                break;
            }

            default:
                cout << "\nInvalid Input.\nPlease enter again ";

            } 
        } while (opt != 0);

        List<int> obj(isp);

        for (int i = 0; i < mach; i++)
        {
            obj.insert_at_middle(r_mach[i]);
        }
        obj.display();
        obj.update_tables();
        for (int i = 0; i < mach; i++)
        {
            obj.print_routing_table(r_mach[i]);
        }
        int input;
        string value;
        int id;
        long long key;
        int option2;

        do 
        {
            cout << "\n\nPress '1' to enter data into Circular DHT\n";
            cout << "Press '2' to remove data from any machine \n";
            cout << "Press '3' to print routing table of any machine\n";
            cout << "Press '4' to print AVL tree of any machine\n";
            cout << "Press '5' to insert new machine\n";
            cout << "Press '6' to remove existing machine\n";
            cout << "Press '0' to stop\n";

            cin >> input;

            switch (input)
            {
            case 0:
                break;

            case 1:
            {
                cout << "\nEnter Data: ";
                cin.ignore();
                getline(cin, value);

                long long key = str_hash(value);
                

                cout << key << endl;
                obj.insert_to_AVL(key, value);
                break;
            }
            case 2:
                cout << "Enter key to remove from machine: ";
                cin >> key;
                obj.delete_data(key);
                break;
            case 3:
                cout << "Enter machine id for printing routing table: ";
                cin >> id;
                obj.print_routing_table(id);
                break;
            case 4:
                cout << "Enter machine id for printing AVL tree: ";
                cin >> id;
                obj.print_AVL(id);
                break;
            case 5:
                cout << "Press 1 to assign id automatically"<<endl;
                cout << "Press 2 to assign id manually"<<endl;
                cout << "Enter option: ";
                cin >> option2;
                if (option2 == 1) {
                    bool flag = true;
                    while (flag) {
                        id = rand() % F_isp;
                        flag = false;
                        for (int j = 0; j < mach; j++)
                        {
                            if (id == r_mach[j])
                                flag = true;
                        }
                    }
                    int* temp = new int[mach + 1];
                    for (int i = 0; i < mach; i++)
                        temp[i] = r_mach[i];
                    temp[mach] = id;
                    mach++;
                    r_mach = temp;
                    delete temp;
                    obj.insert_at_middle(id);
                    obj.Add_machine(id);
                    obj.update_tables();

                }
                else if (option2 == 2) {
                    bool flag = true;
                    while (flag) {
                        flag = false;
                        cout << "Enter machine id: ";
                        cin >> id;
                        while (id > F_isp - 1 || id < 0) {
                            cout << "id out of range, Enter again: ";
                            cin >> id;
                        }
                        for (int i = 0; i < mach; i++) {
                            if (id == r_mach[i]) {
                                cout << "id already exists" << endl;
                                flag = true;
                            }
                        }
                    }
                    int* temp = new int[mach + 1];
                    for (int i = 0; i < mach; i++)
                        temp[i] = r_mach[i];
                    temp[mach] = id;
                    mach++;
                    r_mach = temp;
                    delete temp;
                    obj.insert_at_middle(id);
                    obj.Add_machine(id);
                    obj.update_tables();
                }
                else {
                    cout << "Wrong option chosen" << endl;
                }
                break;
            case 6:
                cout << "Enter machine id for removing: ";
                cin >> id;
                obj.Remove_machine(id);
				for (int i = 0; i < mach; i++) { 
					if (r_mach[i] == id)                         
						r_mach[i] = -1; 
				}
                break;
            default:
                cout << "\nInvalid Input\n";
                cout << "Enter again \n";
                break;
            }
        
        } while (input != 0);

    return 0;
}