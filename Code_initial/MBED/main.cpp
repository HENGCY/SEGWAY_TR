#include "mbed.h"

Serial pc(USBTX, USBRX); //Serial connection for the PC
Serial rpi(p9, p10); //Serial connection for the Raspberry


AnalogIn pot(p20); // ADC for the potentiometer



Timer timer_latency; // Timer that measure the latency of the raspberry


// Tickers execute a function periodically
Ticker tick_rpi; // ticker for updating raspbery pi (every 20ms)
Ticker tick_pc; // ticker for updating PC (every 50ms)
Ticker tick_adc_pot; // ticker to read potentiometer value and mean it (every 1ms)

DigitalOut test_led(LED1); // LED used for debug

int latency;
float torque = 0;

bool text_mode = false;
bool use_potentiometer = true;
bool simu_enabled = false;

float A[2][2] = {1.003f, 0.02002,0.3166f, 1.003f};
float B[2][2] = {-0.0002483f, 0.00312f, -0.02485f, 0.3122f};
float x_k[2] = {0,0};
float u_k[2] = {0,0};

float angle_user = 0;
float pot_values[256];
int pot_values_index = 0;

//Serial communication with raspberry Pi
unsigned char rpi_message_buffer[256];
short int rpi_message_buffer_ptr = 0;

//Serial communication with PC
unsigned char pc_message_buffer[256];
short int pc_message_buffer_ptr = 0;


float bytes_to_float(unsigned char * bytes)
{
    unsigned long value;
    value = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | (bytes[0]);
    return *(float *)&value;
}

void get_ADC()
{
    pot_values[pot_values_index] = pot.read();
    pot_values_index = (pot_values_index +1)%255;
}

float getMeanPot()
{
    float val = 0;
    for(int i=0; i<256 ; ++i) {
        val = val + pot_values[i];
    }
    return val/256;
}


void reset_values()
{
    torque=0;
    x_k[0] = 0;
    x_k[1] = 0;
    u_k[0] = 0;
    u_k[1] = 0;
}

// Converts a floating point value (between 0 and 1) to an angle in radiant
// between -0.349066 and 0.349066 radians (-20 to 20 degrees)
float potToAngle(const float & fl)
{
    return ((fl-0.5f)*0.698132f);
}

// Sends an Int32 to a serial port with proper formating
void send_int32_serial(Serial &ser, const int & val, const char & tag)
{
    unsigned char * b = (unsigned char *) &val;
    ser.putc('<');
    ser.putc(tag);
    ser.putc(b[0]);
    ser.putc(b[1]);
    ser.putc(b[2]);
    ser.putc(b[3]);
    ser.putc('\n');
}

// Sends an float to a serial port with proper formating
void send_float_serial(Serial &ser, const float & fl, const char & tag)
{
    unsigned char * b = (unsigned char *) &fl;
    ser.putc('<');
    ser.putc(tag);
    ser.putc(b[0]);
    ser.putc(b[1]);
    ser.putc(b[2]);
    ser.putc(b[3]);
    ser.putc('\n');
}

//Function called when periodically updating the PC
// Updates latency, user angle, torque, segway angle, segway angular speed
void update_pc()
{
    send_int32_serial(pc, latency, 'l');
    send_float_serial(pc,angle_user,'f');
    send_float_serial(pc,torque,'t');
    send_float_serial(pc,x_k[0],'p');
    send_float_serial(pc,x_k[1],'s');
}

// Function called when periodically updating the RPI
// Compiles new values for the model
// Updates segway angle and segway angular speed
void update_rpi()
{
    timer_latency.reset();
    timer_latency.start();
    if(simu_enabled) {

        if(use_potentiometer) {
            float fl = getMeanPot();
            angle_user = potToAngle(fl);
        }

        float xk0, xk1;
        float angle_user_relatif = angle_user - x_k[0] ;

        xk0 = x_k[0]*A[0][0] + x_k[1]*A[0][1] + torque*B[0][0] + angle_user_relatif*B[0][1];
        xk1 = x_k[0]*A[1][0] + x_k[1]*A[1][1] + torque*B[1][0] + angle_user_relatif*B[1][1];

        x_k[0] = xk0;
        x_k[1] = xk1;

        if(text_mode) {
            pc.printf("%f %f\n",x_k[0],x_k[1]);
        }

        send_float_serial(rpi,x_k[0],'p');
        send_float_serial(rpi,x_k[1],'s');

        if(x_k[0] > 1.57 || x_k[0] < -1.57) {
            reset_values();
        }
    }
}


// Function called when values are received from the serial PC serial port
void read_pc()
{
    char c = pc.getc();
    pc_message_buffer[pc_message_buffer_ptr++] = c;

    if(pc_message_buffer_ptr >6) {

        //pc.putc('|');
        if (pc_message_buffer[1] == 'c' && pc_message_buffer[6] == '\n') {

            if(pc_message_buffer[2] == 'r') {
                reset_values();
            } else if(pc_message_buffer[2] == 's') {
                test_led = !test_led;
                reset_values();
                simu_enabled = true;
            } else if(pc_message_buffer[2] == 'e') {
                simu_enabled = false;
            }
            pc_message_buffer_ptr = 0;
        } else if(pc_message_buffer[1] == 'p' && pc_message_buffer[6] == '\n') {
            use_potentiometer = true;
            pc_message_buffer_ptr = 0;
        } else if(pc_message_buffer[1] == 't' && pc_message_buffer[6] == '\n') {
            use_potentiometer = false;
            angle_user = bytes_to_float(&pc_message_buffer[2]);
            pc_message_buffer_ptr = 0;
        }

        else {
            pc.printf("Fail : %c%c\n", pc_message_buffer[0],pc_message_buffer[1]);
        }
    } else {

    }
}

// Function called when bytes are received from the RPI serial Port
void read_rpi()
{
    char c = rpi.getc();
    rpi_message_buffer[rpi_message_buffer_ptr++] = c;
    if(rpi_message_buffer_ptr >6) {
        //pc.putc('|');
        if (rpi_message_buffer[1] == 't' && rpi_message_buffer[6] == '\n') {
            timer_latency.stop();
            latency = timer_latency.read_us();
            torque = bytes_to_float(&rpi_message_buffer[2]);
            rpi_message_buffer_ptr = 0;
        } else {
            pc.printf("Fail : %c%c\n", rpi_message_buffer[0],rpi_message_buffer[1]);
        }
    }
}



int main()
{
    //Setting baudrates
    pc.baud(921600);
    rpi.baud(115200);
    if(text_mode) {
        pc.printf("Started\n");
    }

    // attaching function to read ADC every ms
    tick_adc_pot.attach(&get_ADC, 0.0001);
    // attaching function to update RPI every 20ms
    tick_rpi.attach(&update_rpi,0.02);
    // attaching function to update PC every 50ms
    tick_pc.attach(&update_pc, 0.05);
    // chosing a function to be executed everytime a byte is received from the RPI serial
    rpi.attach(&read_rpi);
    // chosing a function to be executed everytime a byte is received from the PC serial
    pc.attach(&read_pc);
    latency = -1;


    while(1) {
        // Everything is done by interruptions
    }
}