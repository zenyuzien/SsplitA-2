#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>  
#include <bitset>

using namespace std;

/*
void printBinary(uint32_t value) {
    std::bitset<32> binaryRepresentation(value);
    std::cout <<" "<< binaryRepresentation ;
}*/

void acl_ssa(const string& ip, const string& op )
{
        ifstream inputFile(ip);
        ofstream wr(op);
    
        if (!inputFile.is_open()) {
            cerr << "Error opening file" << endl;
            return ;
        }

       int ip1_1,ip1_2,ip1_3,ip1_4,mask1,mask2,hexd1,hexd2;
       char chr;
       string line,hex1;
       
        while (getline(inputFile, line)) {
        istringstream iss(line);
        
        
       uint32_t IP1,IP2,r1l=0,r2l=0, ONES_1 = UINT32_MAX ,ONES_2= UINT32_MAX, ZER_1, ZER_2;
        
        iss >> chr >> ip1_1 >> chr >> ip1_2 >> chr >> ip1_3 >> chr >> ip1_4 >> chr >> mask1 ;
        IP1 = (ip1_1 << 24)|(ip1_2 << 16)|(ip1_3 << 8)|(ip1_4);//cout<<IP1<<" ";
        iss >> ip1_1 >> chr >> ip1_2 >> chr >> ip1_3 >> chr >> ip1_4 >> chr >> mask2 ;
        IP2 = (ip1_1 << 24)|(ip1_2 << 16)|(ip1_3 << 8)|(ip1_4);//cout<<IP2<<endl;
        iss >> ip1_1 >> chr >> ip1_2 >> ip1_3 >> chr >> ip1_4 ;
        iss >> hex1 ;
        
        istringstream hexer(hex1);
        getline(hexer, hex1, '/');
        hexd1 = stoul(hex1, nullptr, 16);
        getline(hexer, hex1, '/');
        hexd2 = stoul(hex1, nullptr, 16);
        //cout<< mask1<<" "<< mask2<<" \n";
        //printBinary(ONES_1);cout<<endl;
        if(mask1== 0)
        ONES_1 = 0 ;
        else 
        ONES_1 = ONES_1 << (32-mask1);
        
        if(mask2== 0)
        ONES_2 = 0;
        else
        ONES_2 = ONES_2 << (32-mask2);
        
        ZER_1 = (~ONES_1);
        ZER_2 = (~ONES_2);
       
        
        //printBinary(IP1);printBinary(IP1);cout<<endl;
        //printBinary(ONES_1); printBinary(ZER_1);cout<<endl;
        

        r1l = IP1 & ONES_1 ;
        IP1 = IP1 | ZER_1 ; // ip1  = r1r 
        //printBinary(r1l);printBinary(IP1);cout<<endl<<endl;
        //printBinary(IP2);printBinary(IP2);cout<<endl;
        //printBinary(ONES_2); printBinary(ZER_2);cout<<endl;
        r2l = IP2 & ONES_2 ;
        IP2 = IP2 | ZER_2 ; // ip2 = r2r
        //printBinary(r2l);printBinary(IP2);cout<<endl<<endl;
        
        wr<<r1l<<":"<<IP1 << " "<< r2l<<":"<<IP2<<" " << ip1_1<<":"<<ip1_2 <<" "<<ip1_3<<":"<<ip1_4<<" ";
        if(hexd2 == 255)
        wr<< hexd1<<":"<<hexd1<<endl;
        else 
        wr<<"0:255\n";
    }
    
    wr.close();
    inputFile.close();
}

int main()
{
    acl_ssa("C:/Users/rushi/Downloads/SSA-2_updated/acl.rl", "C:/Users/rushi/Downloads/SSA-2_updated/text.txt" );
    cout<<"formating done\n";
    return 0;
}
/*
@192.151.11.16/32 192.151.15.138/32 0 : 65535 0 : 65535 0x00/0x00
@192.151.11.16/32 192.151.15.139/32 0 : 65535 0 : 65535 0x06/0xff
@192.151.11.16/32 192.151.15.140/32 0 : 65535 0 : 65535 0x06/0xff
@192.151.11.134/32 192.151.15.143/32 0 : 65535 0 : 65535 0x00/0x00
@192.151.11.42/32 199.172.118.159/32 0 : 65535 0 : 65535 0x06/0xff
@192.151.11.95/32 199.172.118.159/32 0 : 65535 0 : 65535 0x06/0xff
@192.151.11.99/32 199.172.118.159/32 0 : 65535 0 : 65535 0x06/0xff
*/