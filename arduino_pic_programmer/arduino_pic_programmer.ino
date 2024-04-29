

#define ISP_PORT  PORTC
#define ISP_DDR   DDRC
#define ISP_PIN   PINC
#define ISP_MCLR  3
#define ISP_DAT   1
#define ISP_CLK   0

#define  ISP_MCLR_1 ISP_PORT |= (1<<ISP_MCLR);
#define  ISP_MCLR_0 ISP_PORT &= ~(1<<ISP_MCLR);
#define  ISP_MCLR_D_I ISP_DDR &= ~(1<<ISP_MCLR);
#define  ISP_MCLR_D_0 ISP_DDR |= (1<<ISP_MCLR);

#define  ISP_DAT_1 ISP_PORT |= (1<<ISP_DAT);
#define  ISP_DAT_0 ISP_PORT &= ~(1<<ISP_DAT);
#define  ISP_DAT_V (ISP_PIN&(1<<ISP_DAT))
#define  ISP_DAT_D_I ISP_DDR &= ~(1<<ISP_DAT);
#define  ISP_DAT_D_0 ISP_DDR |= (1<<ISP_DAT);

#define  ISP_CLK_1 ISP_PORT |= (1<<ISP_CLK);
#define  ISP_CLK_0 ISP_PORT &= ~(1<<ISP_CLK);
#define  ISP_CLK_D_I ISP_DDR &= ~(1<<ISP_CLK);
#define  ISP_CLK_D_0 ISP_DDR |= (1<<ISP_CLK);



#define  ISP_CLK_DELAY  5


void isp_send (unsigned int data, unsigned char n);

void isp_send (unsigned int data, unsigned char n)
{
unsigned char i;
ISP_DAT_D_0
//_delay_us(3*ISP_CLK_DELAY);
for (i=0;i<n;i++)
  {
  if (data&0x01)
    {
    ISP_DAT_1
    }
  else
    {
    ISP_DAT_0
    }
  _delay_us(ISP_CLK_DELAY);
  ISP_CLK_1
//  _delay_us(ISP_CLK_DELAY);
  data = data >> 1;
  ISP_CLK_0
  ISP_DAT_0
//  _delay_us(ISP_CLK_DELAY);
  }
  delayMicroseconds(1);
}



unsigned int isp_read_16 (void)
{
unsigned char i;
unsigned int out;
out = 0;
ISP_DAT_D_I
//_delay_us(3*ISP_CLK_DELAY);
for (i=0;i<16;i++)
  {
  ISP_CLK_1
  _delay_us(ISP_CLK_DELAY);
  ISP_CLK_0
  _delay_us(ISP_CLK_DELAY);
  out = out >> 1;
  if (ISP_DAT_V)
    out = out | 0x8000;
  }
  delayMicroseconds(1);
 return out;
}

unsigned int isp_read_14s (void)
{
unsigned char i;
unsigned int out;
out = isp_read_16();
out = out &0x7FFE;
out = out >> 1;
return out;
}

void isp_read_pgm (unsigned int * data, unsigned char n)
{
unsigned char i;
//_delay_us(3*ISP_CLK_DELAY);
for (i=0;i<n;i++)  
  {
  isp_send(0x04,6);
  data[i] = isp_read_14s();
  isp_send(0x06,6);
  }
}

void enter_prog_mode(){
  delayMicroseconds(100);
  ISP_MCLR_0
  delayMicroseconds(100);
  digitalWrite(A2, 1);
  delayMicroseconds(1000);
}

void leave_prog_mode(){
  delayMicroseconds(100);
   digitalWrite(A2, 0);
  delayMicroseconds(100);
  ISP_MCLR_1
  delayMicroseconds(100);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  delay(500);
  
  ISP_CLK_D_0
  ISP_DAT_D_0
  ISP_DAT_0
  ISP_CLK_0
  ISP_MCLR_D_0
  ISP_MCLR_1
  DDRC |= (1<<2);
  digitalWrite(A2, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*Serial.println("Send something");
  while(!Serial.available());
  Serial.read();*/

while(!Serial.available());
char op = Serial.read();


leave_prog_mode();
enter_prog_mode();

if(op=='w'){
  
//erase PGM
isp_send(0x02, 6);
isp_send(0xFFFF, 16);

isp_send(0x09, 6);
delayMicroseconds(7000);

}
  
  leave_prog_mode();
  enter_prog_mode();

if(op == 'w'){
//write PGM
for(int i=0; i<128; i++){
  for(int i=0; i<16; i++){
    unsigned int d = 0;
    unsigned int c;
    while(!Serial.available()); c = Serial.read(); d = c;
    while(!Serial.available()); c = Serial.read(); d |= c<<8;
    isp_send(0x02, 6);
    isp_send(d<<1, 16);
    isp_send(0x08,6);
  delayMicroseconds(4000);
      isp_send(0x06,6);
    Serial.write(0);
  }
}

}
  
  leave_prog_mode();
  enter_prog_mode();

//read PGM
for(int i=0; i<128; i++){
  
  unsigned int datab[16];
  isp_read_pgm(datab, 16);
  for(int i=0; i<16; i++){
    Serial.write(datab[i]&0xFF);
    Serial.write((datab[i]>>8)&0xFF);
    //Serial.print(datab[i], HEX);
    //Serial.print(" ");
  }
  //Serial.println();
}



  leave_prog_mode();
  enter_prog_mode();


if(op=='w'){

  //write CFG
isp_send(0x00, 6);
isp_send(0xFFFF, 16);

  isp_send(0x06,6);
  isp_send(0x06,6);
  isp_send(0x06,6);
  isp_send(0x06,6);
  isp_send(0x06,6);
  isp_send(0x06,6);
  isp_send(0x06,6);//7 skips
  unsigned int d = 0x0000;
  unsigned int c;
  while(!Serial.available()); c = Serial.read(); d = c;
  while(!Serial.available()); c = Serial.read(); d |= c<<8;
  isp_send(0x02, 6);
  isp_send(d<<1, 16);
  
  isp_send(0x08,6);
  
delayMicroseconds(7000);

}

  leave_prog_mode();
  enter_prog_mode();
//read cfg
isp_send(0x00, 6);
isp_send(0x0000, 16);
//Serial.println("\n");
  unsigned int datab[16];
  isp_read_pgm(datab, 16);
  for(int i=0; i<8; i++){
    Serial.write(datab[i]&0xFF);
    Serial.write((datab[i]>>8)&0xFF);
    //Serial.print(datab[i], HEX);
    //Serial.print(" ");
  }
  //Serial.println();

  leave_prog_mode();

  
  digitalWrite(A2, 1);//power on
  
}
