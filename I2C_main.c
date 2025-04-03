#include <LPC21xx.H>
#include <string.h>
#define sw1 ((IOPIN0>>13)&1)
#define sw2 ((IOPIN0>>14)&1)
#define sw3 ((IOPIN0>>15)&1)
#define Al (1<<26);
unsigned char H,M,S,D,DA,MN,Y,dd;
#define MRM  ((H>>5)&1)
char d[6][4];
unsigned char setf=0;
unsigned char tmp1;
unsigned int temp,HC=0;
char c=0;
int alaram[4]={0x01,0x01,0x01};
int date(void);
int month(void);
int year(void);
unsigned char int_to_bcd(int val);
int time (void);
int breaks(void);
int main()
{     
    strcpy(d[0],"sun");
    strcpy(d[1],"mon");
    strcpy(d[2],"tue");
    strcpy(d[3],"wed");
    strcpy(d[4],"thu");
    strcpy(d[5],"fri");
    strcpy(d[6],"sat");
    IODIR0=Al;
    i2c_init();
    lcd_init();
    uart0_init(9600);
    //config_eint0();
    //config_for_eint0();
    unsigned char hour12=0;
    hour12=0x11;
    hour12|=(1<<6)|(1<<5);
    i2c_write(0xd0,0x0,0x57);
    i2c_write(0xd0,0x1,0x59);
    i2c_write(0xd0,0x2,0x12);
    i2c_write(0xd0,0x3,0x00);
    i2c_write(0xd0,0x4,0x31);
    i2c_write(0xd0,0x5,0x00);
    i2c_write(0xd0,0x6,0x99);
    i2c_write(0xd0,0x2,hour12);  
    while(1)
    {
        uart0_tx_interger(c);
        if(c>1)
        {
        lcd_cmd(0x01);
        lcd_cmd(0x80);
        lcd_string("Enter Hours");
        HC=time();    
        i2c_write(0xd0,0x2,int_to_bcd(HC));
        lcd_cmd(0x01);
        lcd_string("Enter Mins");
        HC=time();  
        i2c_write(0xd0,0x1,(int_to_bcd(HC)));
        lcd_cmd(0x01);
        lcd_string("Enter Sec");
        HC=time();  
        i2c_write(0xd0,0x0,(int_to_bcd(HC)));
        lcd_cmd(0x01);
        lcd_string("Enter Date");
        HC=date(); 
        i2c_write(0xd0,0x4,(int_to_bcd(HC)));
        lcd_cmd(0x01);
        lcd_string("Enter Month");
        HC=month();
        i2c_write(0xd0,0x5,(int_to_bcd(HC)));
        lcd_cmd(0x01);
        lcd_string("Enter year");
        HC=year();
        i2c_write(0xd0,0x6,(int_to_bcd(HC)));  
        lcd_cmd(0x01);
        c=0;
        }
        S=i2c_read(0xd0,0x0);
        M=i2c_read(0xd0,0x1);
        H=i2c_read(0xd0,0x2);
        DA=i2c_read(0xd0,0x03);
        D=i2c_read(0xd0,0x04);
        MN=i2c_read(0xd0,0x05);
        Y=i2c_read(0xd0,0x06);
        if(MRM==0)
        {
            lcd_cmd(0x8c);
            lcd_string("AM");
        }
        else
        {
            lcd_cmd(0x8c);
            lcd_string("PM");
        }
        H=H&0x1F;
        lcd_cmd(0x80);
        //hours
        lcd_data((H/0x10)+48);
        lcd_data((H%0x10)+48);
        lcd_data(':');
        //min
        lcd_data((M/0x10)+48);
        lcd_data((M%0x10)+48);
        lcd_data(':');
        //sec
        lcd_data((S/0x10)+48);
        lcd_data((S%0x10)+48);
        //day
        lcd_cmd(0xc0);
        lcd_data((D/0x10)+48);
        lcd_data((D%0x10)+48);
        lcd_data('/');
        //month
        lcd_data((MN/0x10)+48);
        lcd_data((MN%0x10)+48);
        lcd_data('/');
        //yr
        lcd_data((Y/0x10)+48);
        lcd_data((Y%0x10)+48);
        //DOW
        lcd_cmd(0xcc);
        dd=((DA%0x10)+48);
        lcd_string(d[dd-48]);
        //alrm
        if(alaram[0]==0x01 &&alaram[1]==0x01 &&alaram[2]==0x01)
        {
            IOSET0=Al;
            delay_ms(200);
            IOCLR0=Al;
            delay_ms(200);
        }
        
    }
        
}
unsigned char int_to_bcd(int val) {
    return ((val / 10) << 4) | (val % 10);
}
int time()
{
    int time=0;
    int c=0;
    while(c==0)
    {
            lcd_cmd(0xc0);
            lcd_data((time/10)+48);
            lcd_data((time%10)+48);
        if(sw1==0)
        {
            while(sw1==0);
            time++;
            if(time==59)
                time=0;
            
        }
        else if(sw2==0)
        {
            delay_ms(50);
            while(sw2==0);
            c++;
        }
            
    }
    return time;
}
int month()
{
    int dt=0,c=0;
    while(1)
    {
            lcd_cmd(0xc0);
            lcd_data((dt/10)+48);
            lcd_data((dt%10)+48);
        if(sw1==0)
        {
            while(sw1==0);
            dt++;
            
        }
        if(sw2==0)
        {
            while(sw1==0);
            break;            
        }
    }
    return dt;
    
}
int date()
{
    int dt=0,c=0;
    while(1)
    {
            lcd_cmd(0xc0);
            lcd_data((dt/10)+48);
            lcd_data((dt%10)+48);
        if(sw1==0)
        {
            while(sw1==0);
            dt++;
            if(dt==12)
                dt=1;
            
        }
        if(sw2==0)
        {
            while(sw1==0);
            break;            
        }
    }
    return dt;
    
}
int year()
{
    int dt=24,c=0;
    while(1)
    {
            lcd_cmd(0xc0);
            lcd_data((dt/10)+48);
            lcd_data((dt%10)+48);
        if(sw1==0)
        {
            while(sw1==0);
            dt++;

            
        }
        if(sw2==0)
        {
            while(sw1==0);
            break;            
        }
    }
    return dt;
    
}

