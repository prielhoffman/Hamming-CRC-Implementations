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
layer2_CRC_tx();
layer2_CRC_rx();
layer1_usart_tx(); 
layer1_usart_rx(); 

}

static char t_data[] = "Shaked & Priel";

//for layer1_tx
static int t_buffer[12] = {0};
static int t_bit;
static int t_counter = 0;
unsigned static long t_time_ref = 0 ;
unsigned static long curr_time = 0;
static int currClk = 0;

//for layer1_rx
static char r_data[15];
static int r_buffer[8] = {0};
static int r_counter = 0;
static int  r_clk_ref = 0;
static int  r_clk_now = 0;
static int char_counter = 1;

//for layer2_CRC_tx 
static int t_crc_data[12];
static int t_crc4[4] = {0};

//for layer2_CRC_rx
static int r_crc_data[12];
static int r_crc4[4] = {0};
static char r_char;
//static int r_char_counter;

static int i = 0;

static int j = 0;
static int bit_counter = 0;

static int generator[5] = {1, 0, 0, 1, 1};
static int status = 0;

//flags
static int layer_1_tx_busy = 0;
static int layer_2_tx_request = 0;
static int layer_2_tx_wait = 0;
static int layer_1_rx_busy = 1;


//************************************************************************************



void layer2_CRC_tx(){
  
  if(t_data[j] == '\0'){  //if we done sending the frame 
    j = 0;
  }
  
  if(layer_2_tx_request == 0 && layer_1_tx_busy == 0){

      
    for(i = 0; i < 8; i++){
      t_crc_data[i+4] = bitRead(t_data[j], bit_counter);//converting char into bits in int array 
      t_buffer[i+4] = t_crc_data[i+4];//saving the data bits in the buffer
      bit_counter++;
    }
	i = 0;
    j++;
    bit_counter = 0;
    
	t_crc_data[0] = 0;
    t_crc_data[1] = 0;
    t_crc_data[2] = 0;
    t_crc_data[3] = 0;
    
    for(int i = 0 ; i < 8 ; i++){ //the crc4 calculation loop
      if(t_crc_data[11-i] == 1){
        t_crc_data[10-i] = t_crc_data[10-i]^generator[1];
        t_crc_data[9-i] = t_crc_data[9-i]^generator[2];
        t_crc_data[8-i] = t_crc_data[8-i]^generator[3];
        t_crc_data[7-i] = t_crc_data[7-i]^generator[4];
        }
      else{
        t_crc_data[10-i] = t_crc_data[10-i]^0;
        t_crc_data[9-i] = t_crc_data[9-i]^0;
        t_crc_data[8-i] = t_crc_data[8-i]^0;
        t_crc_data[7-i] = t_crc_data[7-i]^0;
      }
      if(i == 7){
        //after the calculation adding the crc4 bits to the buffer
        t_crc4[0] = t_crc_data[7-i];
        t_buffer[0] = t_crc4[0];
        t_crc4[1] = t_crc_data[8-i];
        t_buffer[1] = t_crc4[1];
        t_crc4[2] = t_crc_data[9-i];
        t_buffer[2] = t_crc4[2];
        t_crc4[3] = t_crc_data[10-i];
        t_buffer[3] = t_crc4[3];
      }
    }
    //t_buffer[5] = !t_buffer[5]; // <- CHANGE BIT IN THE DATA AFTER CRC CALCULATION
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
            if(t_counter == 12){
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
    r_crc_data[r_counter] = digitalRead(RX);  
    //Serial.print(r_crc_data[r_counter]);
    r_counter++;
    layer_1_rx_busy = 1;
  }
  else{
    r_clk_ref = r_clk_now;
    layer_1_rx_busy = 1;
    return;
  }
  if(r_counter == 12){
    //Serial.print("   ");
    r_counter = 0;
    layer_1_rx_busy = 0;
    return;
  }
}



void layer2_CRC_rx(){
  if(layer_1_rx_busy == 0){ //if layer 1 tx done to receive the frame
  	
    for(i = 0; i < 8; i++){ //save the data bits in buffer
      r_buffer[i] = r_crc_data[i+4];
    }
    for(i = 0 ; i < 8 ; i++){ //crc4 calculation loop
      if(r_crc_data[11-i] == 1){
        r_crc_data[10-i] = r_crc_data[10-i]^generator[1];
        r_crc_data[9-i] = r_crc_data[9-i]^generator[2];
        r_crc_data[8-i] = r_crc_data[8-i]^generator[3];
        r_crc_data[7-i] = r_crc_data[7-i]^generator[4];
        }
      else{
        r_crc_data[10-i] = r_crc_data[10-i]^0;
        r_crc_data[9-i] = r_crc_data[9-i]^0;
        r_crc_data[8-i] = r_crc_data[8-i]^0;
        r_crc_data[7-i] = r_crc_data[7-i]^0;
      }
      if(i == 7){
        r_crc4[0] = r_crc_data[7-i];
        r_crc4[1] = r_crc_data[8-i];
        r_crc4[2] = r_crc_data[9-i];
        r_crc4[3] = r_crc_data[10-i];

        
        if(char_counter == 0 && !(r_crc4[0] == 0 && r_crc4[1] == 0 && r_crc4[2] == 0 && r_crc4[3] == 0)){
          char_counter = 1;
          return;
        }

        for(r_counter = 0; r_counter < 8 ; r_counter++){ //converting the data bits into char
  			bitWrite(r_char,r_counter,r_buffer[r_counter]);
        }
        r_counter = 0;
        Serial.print(r_char); //printing the char
        Serial.print(" ");
        
        if(r_crc4[0] == 0 && r_crc4[1] == 0 && r_crc4[2] == 0 && r_crc4[3] == 0){
          Serial.print(" success ");
        }
        else{
          Serial.print(" fail ");
        }

        r_data[char_counter-1] = r_char;
        char_counter++;
        
        if(char_counter == 15){ //if we received the array of chars
          Serial.print(" ");
          Serial.print(r_data);//printing the sting
          Serial.print(" ");
          r_data[char_counter-1] = '\0';
          i = 0;
          char_counter = 0;
        }
      }
    }
  }
}
            
          

