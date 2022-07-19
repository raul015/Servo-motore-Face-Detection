#include <iostream>
#include <fcntl.h>
#include <termios.h>
#define STDIN_FILENO 0
#include <stdlib.h>
#include <stdio.h>
#include "dynamixel_sdk.h"                                  // Uses Dynamixel SDK library

#define ADDR_MX_TORQUE_ENABLE           24                  
#define ADDR_MX_GOAL_POSITION           30
#define ADDR_MX_PRESENT_POSITION        36


#define PROTOCOL_VERSION                1.0                 
#define DXL_ID                          5                   
#define BAUDRATE                        57600
#define DEVICENAME                      "/dev/ttyUSB0"      
                                                            

#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define DXL_MINIMUM_POSITION_VALUE      0               // Dynamixel will rotate beetweeb this
#define DXL_MAXIMUM_POSITION_VALUE      1023              
#define DXL_MOVING_STATUS_THRESHOLD     5                  // Dynamixel moving status threshold,

#define ESC_ASCII_VALUE                 0x1b

int getch()
{
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}


int main()
{
  // Inizializza l'istanza di PortHandler
  // Set the port path
  // Get methods and members of PortHandlerLinux or PortHandlerWindows
dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
  // Initialize PacketHandler instance
  // Set the protocol version
  // Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
  dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

  int dxl_comm_result = COMM_TX_FAIL;             // indica quale errore si è verificato durante 
  						   //  la comunicazione del pacchetto

 int prova1;
 float sens = 3.41;
  std::cout<< "Inserisci valore da 0 a 1023: \n"; 
  std::cin>> prova1;
  float angolo= prova1/sens;
  int dxl_goal_position = prova1; 



  uint8_t dxl_error = 0;                          // Dynamixel error
  uint16_t dxl_present_position = 0;              // Present position

  // Il controller apre la porta per comunicazione in modo seriale con Dynamixel
  // termina se non riesce a comunicare
  if (portHandler->openPort())
  {
    printf("Succeeded to open the port!\n");
  }
  else
  {
    printf("Failed to open the port!\n");
    printf("Press any key to terminate...\n");
    getch();
    return 0;
  }

  // Set port baudrate sulla porta aperta nel primo if
  if (portHandler->setBaudRate(BAUDRATE))
  {
    printf("Succeeded to change the baudrate!\n");
  }
  else
  {
    printf("Failed to change the baudrate!\n");
    printf("Press any key to terminate...\n");
    getch();
    return 0;
  }

  // Enable Dynamixel Torque (coppia abilitata) 
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS)
  {
    printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
  }
  else if (dxl_error != 0)
  {
    printf("%s\n", packetHandler->getRxPacketError(dxl_error));
  }
  else
  {
    printf("Dynamixel has been successfully connected \n");
  }
 
//dxl_comm_result==0;

// inizio movimento dato
  while(1)
  {
    printf("Press any key to continue! (or press ESC to quit!)\n");
    if (getch() == ESC_ASCII_VALUE)
      break;

    // Write goal position
    
    dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_GOAL_POSITION, dxl_goal_position, &dxl_error);
    
    if (dxl_comm_result != COMM_SUCCESS)
    {
      printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
    }
    else if (dxl_error != 0)
    {
      printf("%s\n", packetHandler->getRxPacketError(dxl_error));
    }
    

    do
    {
      // Read present position
      dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, ADDR_MX_PRESENT_POSITION, &dxl_present_position, &dxl_error);
      
      if (dxl_comm_result != COMM_SUCCESS)
      {
        printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
      }
      else if (dxl_error != 0)
      {
        printf("%s\n", packetHandler->getRxPacketError(dxl_error));
      }
        float angolo1= dxl_present_position/sens;

 printf("[ID:%01d]\nPosizione obiettivo:%d  Posizione attuale:%d\n", DXL_ID, dxl_goal_position, dxl_present_position);
         	printf( "Angolo Obiettivo: %f      Angolo Attuale: %f \n",angolo,angolo1);
           printf("\n");
    }while((abs(dxl_goal_position - dxl_present_position) > DXL_MOVING_STATUS_THRESHOLD));
    
  std::cout<< "Inserisci un valore da 0 a 1023 : \n"; 
  std::cin>> prova1; 
  angolo= prova1/sens;
  dxl_goal_position= prova1;
  
    


  }// fine del ciclo while esterno

  // Disable Dynamixel Torque (coppia disabilitata)
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
  if (dxl_comm_result != COMM_SUCCESS)
  {
    printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
  }
  else if (dxl_error != 0)
  {
    printf("%s\n", packetHandler->getRxPacketError(dxl_error));
  }
 

  // Close port la porta viene eliminata 
  portHandler->closePort();

  return 0;
}
