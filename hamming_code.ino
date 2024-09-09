// C++ code
//



#define clk_in 8
#define clk_out 9
#define TX 7
#define RX 6
#define BIT_TIME 20
#define BAUD_RATE 9600

void setup()
{
  Serial.begin(BAUD_RATE);
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);
  pinMode(clk_out, OUTPUT);
  pinMode(clk_in, INPUT);
}

void loop()
{
layer2_Hamming47_tx(); 
layer2_Hamming47_rx();
layer1_usart_tx(); 
layer1_usart_rx(); 

}

static char t_data[] = "Priel & Shaked";

//for layer1_tx
static int t_buffer[14] = {0};
static int t_bit;
static int t_counter = 0;
unsigned static long t_time_ref = 0 ;
unsigned static long curr_time = 0;
static int currClk = 0;

//for layer1_rx
//static int r_buffer[8] = {0};
static int r_counter = 0;
static int  r_clk_ref = 0;
static int  r_clk_now = 0;
static int char_counter = 0;
static char r_data[15];
static int r_hamming_data[14];

//for layer2_CRC_tx 
static int t_crc_data[14];
//static int t_crc4[4] = {0};

//for layer2_CRC_rx
//static int r_crc4[4] = {0};
static char r_char;
//static int r_char_counter;

static int i = 0;

static int j = 0;
static int bit_counter = 0;

//static int generator[5] = {1, 0, 0, 1, 1};
//static int status = 0;

//flags
static int layer_1_tx_busy = 0;
static int layer_2_tx_request = 0;
static int layer_2_tx_wait = 0;
static int layer_1_rx_busy = 1;

//flags refs
//static int ref_layer_1_tx_busy = 0;
//static int ref_layer_2_tx_request = 0;
static int ref_layer_1_rx_busy = 1;

int letter;
int index_of_letter=0;
int nibble1[4];
int nibble2[4];
int parity_bits[6];

int e1_1,e2_1,e3_1;
int e1_2,e2_2,e3_2;
int final_result[8];
int index;
char result[15];
int char_index=0;


//************************************************************************************



void layer2_Hamming47_tx(){
  
  if(t_data[j] == '\0'){  //if we done sending the frame 
    j = 0;
  }
  
  if(layer_2_tx_request == 0 && layer_1_tx_busy == 0){
    //we check that layer 2 didn't request yet so it begins to work on a new letter
    //and we check that layer 1 is not busy so layer 2 don't overwhelm him
    
    for(int i=0; i<4; i++){
    nibble1[i]=bitRead(t_data[j],7-i);
    //Serial.print(nibble1[i]); // T E S T --------------------
    }
    for(int i=0; i<4; i++){
    nibble2[i]=bitRead(t_data[j],3-i);
    //Serial.print(nibble2[i]); // T E S T --------------------
    }
    j++;
    //calculate for nibble1:
    parity_bits[0]=nibble1[0]^nibble1[1]^nibble1[3];
    //Serial.print(parity_bits[0]); // T E S T --------------------
    parity_bits[1]=nibble1[0]^nibble1[2]^nibble1[3];
    //Serial.print(parity_bits[1]); // T E S T --------------------
    parity_bits[2]=nibble1[1]^nibble1[2]^nibble1[3];
    //Serial.print(parity_bits[2]); // T E S T --------------------
    
    //calculate for nibble2:
    parity_bits[3]=nibble2[0]^nibble2[1]^nibble2[3];
    //Serial.print(parity_bits[3]); // T E S T --------------------
    parity_bits[4]=nibble2[0]^nibble2[2]^nibble2[3];
    //Serial.print(parity_bits[4]); // T E S T --------------------
    parity_bits[5]=nibble2[1]^nibble2[2]^nibble2[3];
    //Serial.print(parity_bits[5]); // T E S T --------------------
    
    t_buffer[0]=parity_bits[0];
    t_buffer[1]=parity_bits[1];
    t_buffer[2]=nibble1[0];
    t_buffer[3]=parity_bits[2];
    t_buffer[4]=nibble1[1];
    t_buffer[5]=nibble1[2];
    t_buffer[6]=nibble1[3];
    t_buffer[7]=parity_bits[3];
    t_buffer[8]=parity_bits[4];
    t_buffer[9]=nibble2[0];
    t_buffer[10]=parity_bits[5];
    t_buffer[11]=nibble2[1];
    t_buffer[12]=nibble2[2];
    t_buffer[13]=nibble2[3];
    
    //for(int a=0;a<14;a++){
    //  Serial.print(buffer[a]); // T E S T --------------------
    //}
    layer_2_tx_request = 1; //after calculation set a request to send the frame 
  } 
}

 
//lab_1 tx
void layer1_usart_tx(){
   if(layer_2_tx_request == 1){ //if there is a frame to send 
	curr_time = millis();
    if(millis() - t_time_ref > BIT_TIME){	
      t_time_ref = curr_time;
      currClk = !currClk;
      if(layer_1_tx_busy == 0){
        digitalWrite(clk_out,currClk);
          if(currClk == 1){
            t_bit = t_buffer[t_counter];
            digitalWrite(TX, t_bit);
            //Serial.print(t_bit);
            t_counter++;
            if(t_counter == 14){
              //Serial.print("      ");
              layer_1_tx_busy = 0;
              layer_2_tx_request = 0;
              t_counter = 0;
            }
          }
      }  
      return;
    }
  }
}

//lab_1 rx
void layer1_usart_rx(){
  r_clk_now = digitalRead(clk_in);
  if(r_clk_now - r_clk_ref == 1){
    r_clk_ref = r_clk_now;  
    r_hamming_data[r_counter] = digitalRead(RX);    
    r_counter++;
    layer_1_rx_busy = 1;
  }
  else{
    r_clk_ref = r_clk_now;
    layer_1_rx_busy = 1;
    return;
  }
  if(r_counter == 14){
    r_counter = 0;
    layer_1_rx_busy = 0;
    return;
  }
}



void layer2_Hamming47_rx(){
  
  if(layer_1_rx_busy == 0){ //if layer 1 tx done to receive the frame
    layer_1_rx_busy = 1;

    
    //----------------------- FIRST HALF OF THE BUFFER -----------------------
    
    e1_1=r_hamming_data[0]^r_hamming_data[2]^r_hamming_data[4]^r_hamming_data[6];
    e2_1=r_hamming_data[1]^r_hamming_data[2]^r_hamming_data[5]^r_hamming_data[6];
    e2_1=r_hamming_data[3]^r_hamming_data[4]^r_hamming_data[5]^r_hamming_data[6];
    //Serial.print(e1_1); // T E S T --------------------
    //Serial.print(e2_1); // T E S T --------------------
    //Serial.print(e3_1); // T E S T --------------------
    
    if(e1_1==1 && e2_1==0 && e3_1==0){  //001 -> P1 has an error flip to correct
      r_hamming_data[0]=!(r_hamming_data[0]);
    }
    if(e1_1==0 && e2_1==1 && e3_1==0){  //010 -> P2 has an error flip to correct
      r_hamming_data[1]=!(r_hamming_data[1]);
    }
    if(e1_1==1 && e2_1==1 && e3_1==0){  //011 -> D1 has an error flip to correct
      r_hamming_data[2]=!(r_hamming_data[2]);
    }
    if(e1_1==0 && e2_1==0 && e3_1==1){  //100 -> P3 has an error flip to correct
      r_hamming_data[3]=!(r_hamming_data[3]);
    }
    if(e1_1==1 && e2_1==0 && e3_1==1){  //101 -> D2 has an error flip to correct
      r_hamming_data[4]=!(r_hamming_data[4]);
    }
    if(e1_1==0 && e2_1==1 && e3_1==1){  //110 -> D3 has an error flip to correct
      r_hamming_data[5]=!(r_hamming_data[5]);
    }
    if(e1_1==1 && e2_1==1 && e3_1==1){  //111 -> D4 has an error flip to correct
      r_hamming_data[6]=!(r_hamming_data[6]);
    }
    
    //final_result[0]=r_data[2];
    //final_result[1]=r_data[4];
    //final_result[2]=r_data[5];
    //final_result[3]=r_data[6];
    
    //----------------------- SECOND HALF OF THE BUFFER -----------------------
    
    e1_2=r_hamming_data[7]^r_hamming_data[9]^r_hamming_data[11]^r_hamming_data[13];
    e2_2=r_hamming_data[8]^r_hamming_data[9]^r_hamming_data[12]^r_hamming_data[13];
    e2_2=r_hamming_data[10]^r_hamming_data[11]^r_hamming_data[12]^r_hamming_data[13];
    //Serial.print(e1_2); // T E S T --------------------
    //Serial.print(e2_2); // T E S T --------------------
    //Serial.print(e3_2); // T E S T --------------------
    
    if(e1_2==1 && e2_2==0 && e3_2==0){  //001 -> P1 has an error flip to correct
      r_hamming_data[7]=!(r_hamming_data[7]);
    }
    if(e1_2==0 && e2_2==1 && e3_2==0){  //010 -> P2 has an error flip to correct
      r_hamming_data[8]=!(r_hamming_data[8]);
    }
    if(e1_2==1 && e2_2==1 && e3_2==0){  //011 -> D1 has an error flip to correct
      r_hamming_data[9]=!(r_hamming_data[9]);
    }
    if(e1_2==0 && e2_2==0 && e3_2==1){  //100 -> P3 has an error flip to correct
      r_hamming_data[10]=!(r_hamming_data[10]);
    }
    if(e1_2==1 && e2_2==0 && e3_2==1){  //101 -> D2 has an error flip to correct
      r_hamming_data[11]=!(r_hamming_data[11]);
    }
    if(e1_2==0 && e2_2==1 && e3_2==1){  //110 -> D3 has an error flip to correct
      r_hamming_data[12]=!(r_hamming_data[12]);
    }
    if(e1_2==1 && e2_2==1 && e3_2==1){  //111 -> D4 has an error flip to correct
      r_hamming_data[13]=!(r_hamming_data[13]);
    }
    //final_result[4]=r_data[9];
    //final_result[5]=r_data[11];
    //final_result[6]=r_data[12];
    //final_result[7]=r_data[13];
    
    final_result[0]=r_hamming_data[13];
    final_result[1]=r_hamming_data[12];
    final_result[2]=r_hamming_data[11];
    final_result[3]=r_hamming_data[9];
    final_result[4]=r_hamming_data[6];
    final_result[5]=r_hamming_data[5];
    final_result[6]=r_hamming_data[4];
    final_result[7]=r_hamming_data[2];
    
    //for(int i=0; i<8; i++){  
    //  Serial.print(final_result[i]); // T E S T --------------------  
    //}
    //for(int i=0; i<8; i++){  
    //  Serial.print(" "); // T E S T --------------------  
    //}
    //char_index++;
    //if(char_index==14){
    //  Serial.print("  done   "); 
    //  char_index=0;
   // }
    
    for(r_counter = 0; r_counter < 8 ; r_counter++){
     bitWrite(r_char,r_counter, final_result[r_counter]);
     //Serial.print(final_result[r_counter]); 
    }
    //Serial.print(" "); 
    r_counter = 0;
    //Serial.print(r_char); //printing the char
    
    r_data[char_counter]=r_char;
    //Serial.print(r_data[char_counter]); 
    char_counter++;
    
    if(char_counter==14){
     Serial.print(r_data);
     r_data[char_counter]='\0';
     char_counter=0;
    }
  }
}
            
          

