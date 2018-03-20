#include "TrafficFlow.hpp"


int main(int argc,char *argv[])
{
        TrafficFlow t;
        string s;

        system("Color A");
        cout<<"\t\t******WELCOME TO TRAFFIC FLOW CONTROL SYSTEM******\t\t\n";


        if (argc < 2)
        {

                cout << "Press any key to continue" << endl;
                cin>>s;
                 t.generateConfig(s);
                 return 0;
        }

        return 0;
}
