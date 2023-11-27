#include <16f877a.h> 
#device *=16 ADC=8
#FUSES NOWDT, HS, NOPUT, NOPROTECT, NODEBUG, NOBROWNOUT, NOLVP
#use delay(clock=20000000)

#include <lcd.h>
#include <math.h>
#include <string.h>

#define LCD_RS_PIN         PIN_D1   // Cac ket noi C.LCD voi vi dieu khien.
#define LCD_RW_PIN      PIN_D2
#define LCD_ENABLE_PIN  PIN_D3
#define LCD_DATA4       PIN_D4
#define LCD_DATA5       PIN_D5
#define LCD_DATA6       PIN_D6
#define LCD_DATA7       PIN_D7  


#define  bat_tat      input(PIN_B3)
#define  nutnhan_dc1  input(PIN_B4)
#define  nutnhan_dc2  input(PIN_B5)
#define  nutnhan1    input(PIN_B6)
#define  nutnhan2    input(PIN_B7)
#define  nut         input(PIN_B0)
#define  cambien     input(PIN_C6)

#define coi_chip(x)      output_bit(PIN_E1,x)
#define net(x)      output_bit(PIN_E2,x)


//-----dc-------
signed int32 duty=0;
int1 tt;
signed int dc=0;
//--------------
//------------cam bien----------
int gtmacdinh=0;
int dem=0;
signed int somax=0;
//----------------------------



//------------bat tat-------------------
void kt_bat_tat()
{
   if(bat_tat==0)
   {
      delay_ms(20);
      if(bat_tat==0)
      {
         tt=~tt;
         while(bat_tat==0);
      }
      
   }
}
//---------------------- dc--------------------------------------
//------------tang dc max 4 -----------
void kt_dc1_tang()
{
   if(nutnhan_dc1==0)
   {
      delay_ms(20);
      if(nutnhan_dc1==0)
      {
         dc++;
         if(dc>4)  dc=4;
      }
      while(nutnhan_dc1==0);
   }
}
//-----giam dc min 0 --------------
void kt_dc2_giam()
{
   if(nutnhan_dc2==0)
   {
      delay_ms(20);
      if(nutnhan_dc2==0)
      {
         dc--;
         if(dc<=0)
         {
            dc=0;
         }
            while(nutnhan_dc2==0);
      }
   }
}  

void hienthi_dc()
{
   lcd_gotoxy(11,1);
   printf(lcd_putc,"dc:%01u",dc);
} 

void chay_ct_dc()
{
         kt_dc1_tang();
         kt_dc2_giam();
         duty=dc*125;
         setup_ccp1(ccp_pwm);
         set_pwm1_duty(duty);
         hienthi_dc();
}

void khoi_dong_dc()
{
   setup_timer_2(T2_div_by_16,255,1);
   setup_ccp1(ccp_pwm);

   set_pwm1_duty(duty);
   output_high(pin_c1);
   output_low(pin_c1); 
}
//-----------------------------------------------------------

//----------cam bien -------------
void hienthi_sp()
{
   lcd_gotoxy(1,2);
   printf(lcd_putc,"so sp:%02u",dem);
}

void hienthi_somax()
{
   lcd_gotoxy(1,1);
   printf(lcd_putc,"sp max:%02u",somax);
}

void demso()
{
   if(cambien != gtmacdinh)
   {
      if(cambien==1)
      {
         dem++;
         output_high(PIN_E0);  // net sang
      }
      else if(cambien==0)
      {
         output_low(PIN_E0);  // net tat
      }
         gtmacdinh=cambien;  // chong vat can
         hienthi_sp();
   }
   



      // tang sp max
      if(nutnhan1 == 0)
      {
         delay_ms(20);
         if(nutnhan1==0)
         {
            somax++;
            while(nutnhan1==0);
         }
        
      }
      // giam sp max
      if(nutnhan2 == 0)
      {
         delay_ms(20);
         if(nutnhan2==0)
         {
            somax -=1;
            if(somax<=0)
            {
               somax=0;
            }
            while(nutnhan2==0);
         }  
      }
      hienthi_somax();
}

void sosanh_sp()
{
   if(dem==somax)
   {

      lcd_gotoxy(1,2);
      lcd_putc("da day sp");
      coi_chip(1); 
      delay_ms(20);
      lcd_gotoxy(1,2);
      lcd_putc("         ");
      delay_ms(20);  
   }
}

#INT_EXT //chi thi khai bao ngat(Chuong trinh se thuc hien ham ngay sau chi thi)
void nutnhan()  // nut nhan ve 0
{
   if(nut==0)
   {
      delay_ms(20);
      if(nut==0)
      {
         lcd_gotoxy(1,2);
         printf(lcd_putc,"so sp:%02u",dem=0);
         coi_chip(0);
         printf(lcd_putc,"          ");
         while(nut==0);
      } 
   }
}


void main()
{  

//--------nut rest rb0------------------
   enable_interrupts(global);      //Cho phep ngat toan cuc
   enable_interrupts (INT_EXT);    //Cho phep ngat ngoai 
   enable_interrupts(INT_EXT_H2L); //ngat khi co su thay doi trang thau tu H (muc cao) xuong L (muc thap)

  // ext_int_edge(H_TO_L);
//-------------------------

   lcd_init(); 
   lcd_putc('\f');
  
   output_low(PIN_E0);
   net(0);
   coi_chip(0);

   
   //------dc----------------   
      tt=0;
      khoi_dong_dc();
 
   //-------------------   
   while(TRUE)
   {

         
     kt_bat_tat();
     if(tt==1)
     {
         hienthi_somax();
         hienthi_sp();
         hienthi_dc();
         demso();
         sosanh_sp();
         net(0);
         chay_ct_dc();
     }    
     else if(tt==0)
     {
         lcd_clear();   // Xoa man hinh
         dem=0;
         somax=5;
         dc=0;
         net(1);
     }
    }    
}

  

