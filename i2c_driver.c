/*i2c_driver.c*/
#include <LPC21xx.H>
#include "hdr.c"
int count;
void i2c_init(void){
	PINSEL0|=0x50;//P0.2->SCL, P0.3->SDA
	I2CONSET=(1<<6);//I2EN=1,AA=0,Master mode selected
	I2SCLH=I2SCLL=75;//100Kbps freq	
}

#define SI ((I2CONSET>>3)&1)
void i2c_write(u8 sa,u8 mr,u8 data){
	/*1. generate start condi*/
	I2CONSET=(1<<5);//STA=1
	I2CONCLR=(1<<3);//SI=0
	while(SI==0);
	I2CONCLR=(1<<5);//STA=0
	
	/*2. send sa+w & check ack*/
	I2DAT=sa;//send SA+W
	I2CONCLR=(1<<3);//SI=0
	while(SI==0);//waiting for SA to xmit
	if(I2STAT==0x20){
		uart0_tx_string("err:SA+W\r\n");
		goto exit;
	}
	
	/*3. send m/r addr & check ack*/
	I2DAT=mr;//send m/r addr
	I2CONCLR=(1<<3);//SI=0
	while(SI==0);
	if(I2STAT==0x30){
		uart0_tx_string("err:memory addr\r\n");
		goto exit;
	}	
	
	/*4. send data & check ack*/
	I2DAT=data;//send data
	I2CONCLR=(1<<3);//SI=0
	while(SI==0);
	if(I2STAT==0x30){
		uart0_tx_string("err:memory addr\r\n");
		goto exit;
	}	
	
	/*5. generate stop condi*/	
	exit:
		I2CONSET=(1<<4);//STO=1
		I2CONCLR=(1<<3);//*SI=0
}

u8 i2c_read(u8 sa,u8 mr){
	u8 temp;
	/*1. generate start condi*/
	I2CONSET=(1<<5);//STA=1
	I2CONCLR=(1<<3);//SI=0
	while(SI==0);
	I2CONCLR=(1<<5);//STA=0
	
	/*2. send sa+w & check ack*/
	I2DAT=sa&0xFE;//send SA+W
	I2CONCLR=(1<<3);//SI=0
	while(SI==0);//waiting for SA to xmit
	if(I2STAT==0x20){
		uart0_tx_string("err:SA+W\r\n");
		goto exit;
	}
	
	/*3. send m/r addr & check ack*/
	I2DAT=mr;//send m/r addr
	I2CONCLR=(1<<3);//SI=0
	while(SI==0);
	if(I2STAT==0x30){
		uart0_tx_string("err:memory addr\r\n");
		goto exit;
	}	
	
	/*4. generate restart condi*/
	I2CONSET=(1<<5);//STA=1
	I2CONCLR=(1<<3);//SI=0
	while(SI==0);
	I2CONCLR=(1<<5);//STA=0
		
	/*5. send sa+r & check ack*/
	I2DAT=(sa|1);//send SA+R//////////////////////////////////////////////////////////////////////////////////////
	I2CONCLR=(1<<3);//SI=0
	while(SI==0);//waiting for SA to xmit
	if(I2STAT==0x48){
		uart0_tx_string("err:SA+R\r\n");
		goto exit;
	}
	
	/*6. read data(from slave) & ret noack*/
	I2CONCLR=(1<<3);//SI=0
	while(SI==0);//waiting for data to receive
	temp=I2DAT;
	
	/*7. generate stop condi*/
	exit:
		I2CONSET=(1<<4);//STO=1
		I2CONCLR=(1<<3);//*SI=0
	
	/*8. return received data*/
	return temp;	
}
 void i2c_detect(unsigned char sa)
 {
         //statr condition//
          I2CONSET=(1<<5);  //STA=1^M
         I2CONCLR=(1<<3);   //SI=0^M
         while(SI==0);
         I2CONCLR=(1<<5);   //STA=0
     
         // send sa+w//
         I2DAT=sa&0xFE;     //send SA+W^M
         I2CONCLR=(1<<3);       //SI=0^M
         while(SI==0);      //waiting for SA to xmit^M
         if(I2STAT==0x20){
         //      uart0_tx_string("err:SA+W\r\n");^M
                 //goto exit;
                 return ;
         }
         else
         {
         uart0_tx_hex(sa);
         uart0_tx_string("  ");
         uart0_tx_interger(count);
         uart0_tx_string("\n\r");
         count ++;
         }

         /*7. generate stop condi*/
         
                 I2CONSET=(1<<4);//STO=1^M
                 I2CONCLR=(1<<3);//*SI=0
 }


 