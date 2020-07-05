
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

void initializeUART()
{
  /*---------- Setting the Attributes of the serial port using termios structure --------- */
		
		struct termios SerialPortSettings;	/* Create the structure                          */
		tcgetattr(fd, &SerialPortSettings);	/* Get the current attributes of the Serial port */
		cfsetispeed(&SerialPortSettings,B115200); /* Set Read  Speed as 9600                       */
		cfsetospeed(&SerialPortSettings,B115200); /* Set Write Speed as 9600                       */
		SerialPortSettings.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
		SerialPortSettings.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	  SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
		SerialPortSettings.c_cflag |=  CS8;      /* Set the data bits = 8                                 */
		SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
		SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
		SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
		SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
		SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

		if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		    printf("\n  ERROR ! in Setting attributes");
		else
                    printf("\n  BaudRate = 115200 \n  StopBits = 1 \n  Parity   = none");
}


//////////////////////////////////////////////////////////////////
const char* hex_char_to_bin(char c)
{
    // TODO handle default / error
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
}


//////////////////////////////////////////////////////////////////
int main() // определяем функцию main
{

  ifstream file("./v2boot.bin", ios::binary | ios::ate);
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
  ///////////////////////////////////////////////////
  myfile.open("./myversion.bin", ios::binary);
  //myfile << buffer.data() << out;
  myfile.write((char *)buffer.data(), size);
  myfile.close();
  myfile.open("./myversion.bin", ios::binary|ios::app);
  uint8_t str; 

  for(uint8_t i =4;i>0;i--){
  
  str = (out>>8*(i-1))&255;
  
  cout << (char)str<< "\n"; // выводим строку на консоль
  myfile  << str;
  //myfile.write((char *)str, sizeof(str));
  }
  myfile.close();
//   /////////////////uart////////////////////////////////////////

		
// 		printf("\n +----------------------------------+");
// 		printf("\n |        Serial Port Write         |");
// 		printf("\n +----------------------------------+");
//     fd = open("/dev/ttyACM0",O_RDWR | O_NOCTTY  | O_NDELAY);
// //fd = open("/dev/ttyACM0",O_RDWR | O_NOCTTY | O_NDELAY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
//                                                         /* O_RDWR Read/Write access to serial port           */
//                                                         /* O_NOCTTY - No terminal will control the process   */
//                                                         /* O_NDELAY -Non Blocking Mode,Does not care about-  */
//                                                         /* -the status of DCD line,Open() returns immediatly */                                        				
//         	if(fd == -1)						/* Error Checking */
//             	   printf("\n  Error! in Opening ttyACM0  ");
//         	else
//             	   printf("\n  ttyACM0 Opened Successfully ");
// initializeUART();
// /*------------------------------- Write data to serial port -----------------------------*/

// 		char write_buffer[] = "A";	/* Buffer containing characters to write into port	     */	
// 		int  bytes_written  = 0;  	/* Value for storing the number of bytes written to the port */ 
    
//     bytes_written = write(fd,&size,4);//количество байт прошивки
//     usleep(3000);
//    uint32_t len=512;
//     for(uint32_t i=0;i<size;i+=512){
//       if((size-i)<512){
//         len=(size-i);
//       }
//       else{
//         len=512;
//       }
// 		bytes_written = write(fd,(buffer.data()+i),len);/* use write() to send data to port                                            *
//                                                                 /* "fd"                   - file descriptor pointing to the opened serial port */
//                                                                 /*	"write_buffer"         - address of the buffer containing data	            */
//                                                                 /* "sizeof(write_buffer)" - No of bytes to write                               */	
// printf("\n  %d Bytes written to ttyACM0", bytes_written);
// if (bytes_written==-1){
//   printf("\n +ЗАПИСЬ НЕ УДАЛАСЬ+\n\n");
//   break;
  
// }
// usleep(1000000);
// }
//   	bytes_written = write(fd,&out,4);//4 байта CRC прошивки
//     printf("\n  %s written to ttyACM0",write_buffer);
// 		printf("\n  %d Bytes written to ttyACM0", bytes_written);
// 		printf("\n +----------------------------------+\n\n");

// 		close(fd);/* Close the Serial port */			               
//   /////////////////end uart///////////////////////////////////////////
  return 0; // выходим из функции
}
