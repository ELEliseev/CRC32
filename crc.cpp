
#include <iostream> // подключаем заголовочный файл iostream
#include <stddef.h>
#include <stdint.h>
#include <iomanip>
#include <fstream>
#include <vector>
///////////////////////uart///////////////////////////////////////////
#include <stdio.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
//////////////////////end uart///////////////////////////////////////////////
using namespace std;
int fd;/*File Descriptor*/
uint32_t out = 0;
uint32_t Crc32(uint8_t *Input_Data, uint32_t LenData)
{
  uint32_t polynom = 0x4C11DB7;
  uint32_t CRC = 0;
  uint32_t MsbCRC = 0;
  uint32_t Initial_crc = 0xFFFFFFFF;
  int bindex = 0;
  CRC = Initial_crc ^ (*Input_Data);
  do
  {
    MsbCRC = (CRC >> 31) & 1;

    if (MsbCRC == 1)
    {
      CRC = (CRC << 1) ^ polynom;
    }
    else
    {
      CRC = (CRC << 1);
    }
    bindex++;
    if (bindex % 32 == 0 && bindex < (32 * LenData - 1))
    {
      CRC ^= *(Input_Data + bindex / 32);
    }
  } while (bindex < 32 * LenData);
  return CRC;
}


//////////////////////////////////////////////////////////////////
int main() // определяем функцию main
{

  ifstream file("./rtos.bin", ios::binary | ios::ate);
  streamsize size = file.tellg();
  file.seekg(0, ios::beg);
  vector<uint8_t> buffer(size);

  if (file.read((char *)buffer.data(), size))
  {
    cout << size << '\n';

    /* worked! */
    file.close();
  }

  /*КОНЕЦ РАБОТЫ С ФАЙЛОМ*/
  cout << "EEliseev CRC!\n"; // выводим строку на консоль
  out = Crc32(buffer.data(), size);
  cout << "0x" << hex << uppercase << out << "\n"; // выводим строку на консоль
  ofstream myfile;
  myfile.open("./crcCode.txt");
  myfile << "0x" << hex << uppercase << out;
  myfile.close();
 // mkdir("./dist", ~umask & 01777);
  system("mkdir -p ./dist");
  ///////////////////////////////////////////////////
  myfile.open("./dist/myversion.bin", ios::binary);
  //myfile << buffer.data() << out;
  myfile.write((char *)buffer.data(), size);
 // myfile.close();
 // myfile.open("./dist/myversion.bin", ios::binary|ios::app);
  uint8_t str; 

  for(uint8_t i =4;i>0;i--){
  
  str = (out>>8*(i-1))&255;
  
  cout << (char)str<< "\n"; // выводим строку на консоль
  myfile  << str;
  //myfile.write((char *)str, sizeof(str));
  }
  myfile.close();

  return 0; // выходим из функции
}
