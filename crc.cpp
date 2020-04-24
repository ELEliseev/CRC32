
#include <iostream>               // подключаем заголовочный файл iostream
#include <stddef.h>
#include <stdint.h>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;
 stringstream stream;
 //char BUFFER[20]={0};
 int out=0;
uint32_t Crc32(uint8_t  *Input_Data,uint32_t LenData)
{
    uint32_t polynom = 0x4C11DB7;
    uint32_t CRC=0;
    uint32_t MsbCRC=0;
    uint32_t Initial_crc = 0xFFFFFFFF;
    int bindex = 0;
    CRC = Initial_crc ^ (*Input_Data);
do{   
    MsbCRC=(CRC>>31)&1; 

if(MsbCRC==1){
CRC=(CRC<<1)^polynom;
}
else{
CRC=(CRC<<1);  
}
bindex++;
if(bindex%32==0&&bindex<(32*LenData-1)){
    CRC^=*(Input_Data+bindex/32);
}
}
while(bindex<32*LenData);
return CRC ; 
}
int main()                          // определяем функцию main
{     

ifstream file("/home/evgeniy/workspace/bootCopy/Debug/bootCopy.bin", ios::binary  |ios::ate );
streamsize size = file.tellg();
file.seekg(0, ios::beg);
vector <uint8_t> buffer(size);

// if (file.read((char*)(buffer.data()), size))
if (file.read((char*)buffer.data(), size))
{
    cout <<  size<<'\n';
    
    /* worked! */
    file.close();
}

uint8_t bu[9]={
1,2,3,4,5,6,7,8,0xA0
};
   /*КОНЕЦ РАБОТЫ С ФАЙЛОМ*/
     cout << "EEliseev CRC!\n";  // выводим строку на консоль
    //cout << reinterpret_cast<char *>(buffer.data())<<"\n";  // выводим строку на консоль
  //  cout <<  (char*)(buffer.data())<<'\n';
 //   cout <<  (char*)(buffer.data())<<'\n';
  
    out= Crc32(buffer.data(), size);
     stream << "0x"<<  hex << out << "\n";
    string result( stream.str() );
      cout << result;  // выводим строку на консоль
   ofstream myfile;
  myfile.open ("/home/evgeniy/workspace/bootCopy/Debug/example.bin");
  for(int i=0;i<size;i++){
  myfile << buffer[i];
  }
  myfile.close();
    // cout << (uint32_t*)&buffer;
     // stream << "0x"<<  hex << BUFFER << "\n";
   // string result( stream.str() );
   //   cout << result;  // выводим строку на консоль
    return 0;                       // выходим из функции
}                                   

