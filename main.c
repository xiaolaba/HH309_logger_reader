//--------------------------- main.c
//JUL-30-2011, xiaolaba
//last update 2018-NOV-17, xiaolaba

// 2018-NOV-17, library has been update, https://www.teuniz.net/RS-232/
// November 22, 2017 new version

// all function named RS232_ as prefix
// fix the mian.c source code here, fucntion call add prefix "RS232_"
// MODE change from "N81" ro "8N1" 
// upload to Github, include library source code as original author license GNU GPL.

// how to compile
// install minGW, http://sourceforge.net/projects/mingw/files/
// Win10,
// run dosbox.bat,
// command line, set path=c:\MinGW\bin;%path%
// or command line, set_path.bat
// command line, compile.bat
// 
// Gcc, 沒有GDB 除錯資料的編譯 
//   gcc -static -o RS232.exe main.c RS232_xiaolaba.c
// 
//用G++, 包含GDB 除錯資料的編譯 g
//   ++ -g -static -o RS232.exe main.c RS232_xiaolaba.c


 
#include <stdio.h> //wordpress 自動去除 < > 及內容, 解決辦法, 前後加空格
#include <stdlib.h>
#include <time.h>

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 


void delay_ms(int milli_seconds) 
{ 
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 
 
#include "rs232.h" //wordpress 自動轉換雙引號為 半角符號, 無法解決
//原因參考以下
//http://farlee.info/archives/wordpress-format-quote-mark.html
 
//測試用, 打開 com port, 發送3byte資料, 關閉 com port

 
int main()
{
//int RS232_OpenComport(int comport_number, int baudrate, const char * mode)

//RS232_OpenComport(4, 9600, "N81"); //win32/ win64 底下有問題, 需要改library

//** change com port number array with one the first dummy element, or write COM#-1
//** Windows COM PORT number is COM1,COM2, index is -1
//  RS232_OpenComport(6-1, 115200, "8N1"); //win32/ win64 底下有問題, 需要改library

  int i, n,
      com_port_number = 3,
      baud = 9600;

  printf("wanna open COM%d\n", com_port_number);
  RS232_OpenComport(com_port_number, baud, "8N1"); //win32/ win64 底下有問題, 需要改library


/*  
  char i;
 
  for(i=0x30;i<0x40;i++){
    RS232_SendByte(3, i);
  }
*/

  unsigned char buf[4096];
  //Example code that demonstrates how to use the library to receive characters and print them to the screen:
  while(1)
  {
    RS232_SendByte(com_port_number, 'A');
    delay(3);    //delay 1 sec
  
    n = RS232_PollComport(com_port_number, buf, 4095);

    if(n > 0)
    {

      buf[n] = 0;   // always put a "null" at the end of a string!

      int byte_vlaue = 0;

      for(i=0; i < n; i++)
      {
        byte_vlaue = buf[i];
        printf("%02x ", byte_vlaue); // print each byte hex
      }
      printf("received %i bytes\n", n);

      for(i=0; i < n; i++)
      {
        byte_vlaue = buf[i];
        if(byte_vlaue < 32 || byte_vlaue > 127)  // replace unreadable control-codes by dots
        {
          byte_vlaue = '.';
        }
        printf("%c  ", byte_vlaue); // print each byte as ASCII
      }
      printf("ascii\n");

//      printf("received %i bytes: %s\n", n, (char *)buf);
  
    }

    RS232_SendByte(com_port_number, 'e');
    delay_ms(500); //must
    n = RS232_PollComport(com_port_number, buf, 4095);
    if (n==1)
    {
        if (buf[0] == 'r') {
          printf("earsing HH309 memory... ");
//          delay_ms(10);
          RS232_SendByte(com_port_number, 'r');
          delay_ms(5);  //must
          RS232_SendByte(com_port_number, 'a');
          delay_ms(5);  //must

// orignal log, but seems no need
/*
          for(i=0; i < 8; i++) 
          {
              RS232_SendByte(com_port_number, 0x00);  
              delay_ms(10);
          }        
*/
          delay(3);    //delay 5 sec
          printf("when hearing [beep], job done\n");

        }
    }
    
  } 
  
  RS232_CloseComport(3);

}





 
//--------------------------- main.c