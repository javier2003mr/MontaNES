#include "APU.hpp"
#include <stdio.h>

APU :: APU(){
    player = nullptr;
}

APU :: ~APU(){
    if (player){
        player->Stop();
        delete player;
        player = nullptr;
    }
}

void APU :: reset(){
    
    // Inicialización de los registros
    for (int i = 0; i < TAM_APU_REGISTERS; ++i){
        regPulse1[i] = 0x00;
        regPulse2[i] = 0x00;
        regTriangle[i] = 0x00;
        regNoise[i] = 0x00;
        regDMC[i] = 0x00;
    }

    reg4015 = 0x00;
    reg4017 = 0x00;

    regPulse1[0] = regPulse2[0] = regNoise[0] = 0x30;
    regPulse1[1] = regPulse2[1] = 0x08;
    regTriangle[0] = 0x80;

    // Initialize new member variables
    for (int i = 0; i < 2; ++i) {
        pulse_timer_counter[i] = 0;
        pulse_current_sequence_step[i] = 0;
        pulse_envelope_volume[i] = 0;
        pulse_length_counter[i] = 0;
        pulse_sweep_counter[i] = 0;
        pulse_output[i] = 0.0f;
        pulse_envelope_divider[i] = 0;
        pulse_envelope_counter[i] = 0;
        pulse_envelope_start_flag[i] = true;
        pulse_sweep_divider[i] = 0;
        pulse_sweep_reload_flag[i] = true;
        pulse_current_period[i] = getPulseTimer(i); // Initialize with current timer value
    }

    // Initialize new member variables for Triangular Wave
    triangle_timer_counter = 0;
    triangle_linear_counter = 0;
    triangle_length_counter = 0;
    triangle_current_sequence_step = 0;
    triangle_output = 0.0f;
    triangle_linear_counter_reload_value = 0;
    triangle_linear_counter_reload_flag = false;

    // Initialize frame sequencer
    frame_sequencer_counter = 0;
    frame_sequencer_timer = 0;

    //Parte del codigo de BSoundPlayer
    is_playing = false;
    
    media_raw_audio_format format;
    
    format.frame_rate = SAMPLE_RATE;
    format.channel_count = 1; // Mono
    format.format = media_raw_audio_format::B_AUDIO_FLOAT;
    format.buffer_size = BUFFER_SIZE;
    format.byte_order = B_MEDIA_LITTLE_ENDIAN;

    if (player != nullptr){
        delete player;
    }
    
    player = new BSoundPlayer (&format, "MontaNES Audio", &APU::AudioCallback, NULL, this);

    if (player->InitCheck() != B_OK) {
        printf("La inicialización de la APU ha fallado: %s\n", strerror(player->InitCheck()));
        delete player;
        player = nullptr;
    } else {
        player->SetVolume(AMPLITUDE);
        printf("APU inicializada con exito\n");
    }
}

unsigned char APU :: getAudioValue(unsigned short dir){

    unsigned char temp = 0;

    dir = dir - 0x4000;

    if (dir < 0x04){
        temp = regPulse1[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x08){
        temp = regPulse2[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x0C){
        temp = regTriangle[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x10){
        temp = regNoise[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x15){
        temp = regDMC[dir % TAM_APU_REGISTERS];
    }else if (dir == 0x15){
        temp = reg4015;
    }else if (dir == 0x17){
        temp = reg4017;
    }

    return temp;

}

unsigned char * APU :: getAudioDir(unsigned short dir){

    unsigned char *temp = nullptr;

    dir = dir - 0x4000;

    if (dir < 0x04){
        temp = &regPulse1[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x08){
        temp = &regPulse2[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x0C){
        temp = &regTriangle[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x10){
        temp = &regNoise[dir % TAM_APU_REGISTERS];
    }else if (dir < 0x15){
        temp = &regDMC[dir % TAM_APU_REGISTERS];
    }else if (dir == 0x15){
        temp = &reg4015;
    }else if (dir == 0x17){
        temp = &reg4017;
    }

    return temp;

}

void APU :: setAudioValue(unsigned short dir, unsigned char value){

    dir = dir - 0x4000;

    if (dir < 0x04){
        regPulse1[dir % TAM_APU_REGISTERS] = value;
        if (dir % TAM_APU_REGISTERS == 3) { // Write to $4003
            pulse_length_counter[0] = LENGTH_COUNTER_TABLE[(value >> 3) & 0x1F];
            pulse_envelope_start_flag[0] = true; // Restart envelope
            pulse_current_period[0] = getPulseTimer(0); // Update current period
        } else if (dir % TAM_APU_REGISTERS == 2) { // Write to $4002
            pulse_current_period[0] = getPulseTimer(0); // Update current period
        } else if (dir % TAM_APU_REGISTERS == 1) { // Write to $4001 (sweep)
            pulse_sweep_reload_flag[0] = true;
        }
    }else if (dir < 0x08){
        regPulse2[dir % TAM_APU_REGISTERS] = value;
        if (dir % TAM_APU_REGISTERS == 3) { // Write to $4007
            pulse_length_counter[1] = LENGTH_COUNTER_TABLE[(value >> 3) & 0x1F];
            pulse_envelope_start_flag[1] = true; // Restart envelope
            pulse_current_period[1] = getPulseTimer(1); // Update current period
        } else if (dir % TAM_APU_REGISTERS == 2) { // Write to $4006
            pulse_current_period[1] = getPulseTimer(1); // Update current period
        } else if (dir % TAM_APU_REGISTERS == 1) { // Write to $4005 (sweep)
            pulse_sweep_reload_flag[1] = true;
        }
    }else if (dir < 0x0C){
        regTriangle[dir % TAM_APU_REGISTERS] = value;
        if (dir % TAM_APU_REGISTERS == 0) { // Write to $4008
            triangle_linear_counter_reload_value = value & 0x7F;
            triangle_linear_counter_reload_flag = true;
        } else if (dir % TAM_APU_REGISTERS == 3) { // Write to $400B
            triangle_length_counter = LENGTH_COUNTER_TABLE[(value >> 3) & 0x1F];
            triangle_linear_counter_reload_flag = true;
        }
    }else if (dir < 0x10){
        regNoise[dir % TAM_APU_REGISTERS] = value;
    }else if (dir < 0x15){
        regDMC[dir % TAM_APU_REGISTERS] = value;
    }else if (dir == 0x15){
        reg4015 = value;
    }else if (dir == 0x17){
        reg4017 = value;
    }

}

// Pulse Wave
double APU :: getDutyCycle(int pulse_index){
    
    unsigned char duty_cycle_raw;
    double duty_cycle;

    duty_cycle_raw = (!pulse_index) ? (regPulse1[0] & 0xC0) >> 6 : (regPulse2[0] & 0xC0) >> 6;

    switch (duty_cycle_raw)
    {
    case 0b00:
        duty_cycle = 0.125;
        break;
    case 0b01:
        duty_cycle = 0.25;
        break;
    case 0b10:
        duty_cycle = 0.50;
        break;
    case 0b11:
        duty_cycle = 0.75;
        break;
    default:
        duty_cycle = 0;
        break;
    }

    return duty_cycle;

}

unsigned char APU :: getLengthCounter(int pulse_index){
    return (!pulse_index) ? ((regPulse1[3] & 0xF8) >> 3) : ((regPulse2[3] & 0xF8) >> 3);
}

bool APU :: getLengthCounterHalt(int pulse_index){
    return (!pulse_index) ? (regPulse1[0] & 0x20) != 0 : (regPulse2[0] & 0x20) != 0;
}

bool APU :: getEnvelopeFlag(int pulse_index){
    return (!pulse_index) ? (regPulse1[0] & 0x10) != 0 : (regPulse2[0] & 0x10) != 0;
}

double APU :: getVolume (int pulse_index){
    
    unsigned char reg0 = (!pulse_index) ? regPulse1[0] : regPulse2[0];
    bool constant_volume_flag = (reg0 >> 4) & 0x01; // Bit 4 (C)
    unsigned char volume_envelope_period = reg0 & 0x0F; // Bits 3-0 (V/P)

    if (constant_volume_flag) {
        return (double)volume_envelope_period / 0x0F;
    } else {
        return (double)pulse_envelope_volume[pulse_index] / 0x0F;
    }
}

unsigned short APU :: getPulseTimer (int pulse_index){

    unsigned short timer;

    timer = (!pulse_index) ? regPulse1[2] | ((regPulse1[3] & 0x07) << 8) : regPulse2[2] | ((regPulse2[3] & 0x07) << 8);

    return timer;

}

bool APU :: getSweepEnableFlag(int pulse_index){
    return (!pulse_index) ? (regPulse1[1] & 0x80) != 0 : (regPulse2[1] & 0x80) != 0;
}

unsigned char APU :: getSweepDividersPeriod(int pulse_index){
    return (!pulse_index) ? (regPulse1[1] & 0x70) >> 4 : (regPulse2[1] & 0x70) >> 4;
}

bool APU :: getSweepNegativeFlag(int pulse_index){
    return (!pulse_index) ? (regPulse1[1] & 0x04) != 0 : (regPulse2[1] & 0x04) != 0;
}

unsigned char APU::getSweepShiftCount(int pulse_index) {
    if (pulse_index == 0) {
        return (regPulse1[1] >> 0) & 0x07;
    } else {
        return (regPulse2[1] >> 0) & 0x07;
    }
}

float APU::generatePulseSample(int pulse_index) {

    // Get current duty cycle pattern
    unsigned char duty_cycle_raw = (pulse_index == 0) ? (regPulse1[0] & 0xC0) >> 6 : (regPulse2[0] & 0xC0) >> 6;
    const unsigned char* current_duty_sequence = DUTY_CYCLE_SEQUENCES[duty_cycle_raw];

    // Update timer counter
    if (pulse_timer_counter[pulse_index] == 0) {
        pulse_timer_counter[pulse_index] = pulse_current_period[pulse_index];
        pulse_current_sequence_step[pulse_index] = (pulse_current_sequence_step[pulse_index] + 1) % 8;
    } else {
        pulse_timer_counter[pulse_index]--;

        //if (pulse_timer_counter[pulse_index] < 8){
          //  pulse_output[pulse_index] = 0.0f;
          //  return pulse_output[pulse_index];
        //}
    }

    // Get volume (now uses envelope volume)
    double volume = getVolume(pulse_index);

    // If length counter is 0, silence the channel
    // Muting conditions from sweep unit
    // Determine output based on duty cycle and volume
    if (current_duty_sequence[pulse_current_sequence_step[pulse_index]] == 1 &&
        !(pulse_current_period[pulse_index] < 8 || 
        pulse_current_period[pulse_index] > 0x7FF || 
        pulse_length_counter[pulse_index] == 0 || 
        pulse_timer_counter[pulse_index] < 8)) {
        pulse_output[pulse_index] = volume;
    } else {
        pulse_output[pulse_index] = 0.0f;
    }

    return pulse_output[pulse_index];
}

//Noise wave
bool APU::getNoiseEnvelopeLoop() {
    return (regNoise[0] >> 5) & 0x01;
}

bool APU::getNoiseConstantVolume() {
    return (regNoise[0] >> 4) & 0x01;
}

unsigned char APU::getNoiseVolumeEnvelope() {
    return regNoise[0] & 0x0F;
}

bool APU::getNoiseMode() {
    return (regNoise[2] >> 7) & 0x01;
}

unsigned char APU::getNoisePeriod() {
    return regNoise[2] & 0x0F;
}

unsigned char APU::getNoiseLengthCounterLoad() {
    return (regNoise[3] >> 3) & 0x1F;
}

//Triangular wave
bool APU::getTriangleLengthCounterHalt() {
    return (regTriangle[0] & 0x80);
}

unsigned char APU::getTriangleLinearCounterLoad() {
    return regTriangle[0] & 0x7F;
}

unsigned short APU::getTriangleTimer() {
    return (unsigned short)regTriangle[2] | (((unsigned short)regTriangle[3] & 0x07) << 8);
}

unsigned char APU::getTriangleLengthCounterLoad() {
    return (regTriangle[3] >> 3) & 0x1F;
}

float APU::generateTriangleSample() {

    if (triangle_linear_counter == 0 || triangle_length_counter == 0){
        triangle_output = 0.0f;
        return triangle_output;
    }
    // Get timer period
    unsigned short timer_period = getTriangleTimer();

    // Update timer counter
    if (triangle_timer_counter == 0) {
        triangle_timer_counter = timer_period;
        triangle_current_sequence_step = (triangle_current_sequence_step + 1) % 32;
    } else {
        triangle_timer_counter--;
    }

    
    triangle_output = ((float)TRIANGLE_SEQUENCE[triangle_current_sequence_step] / 15.0f);// - 1.0f;

    return triangle_output;
}

float APU::generateNoiseSample() {
    // Get timer period
    unsigned char period_index = getNoisePeriod();
    unsigned short timer_period = 0;//NOISE_PERIODS[period_index];

    // Update timer counter
    if (noise_timer_counter == 0) {
        noise_timer_counter = timer_period;

        // Shift LFSR
        unsigned short feedback_bit;
        if (getNoiseMode()) { // Short mode (mode bit is 1)
            feedback_bit = (noise_shift_register & 0x0001) ^ ((noise_shift_register & 0x0040) >> 6); // Bit 0 XOR Bit 6
        } else { // Long mode (mode bit is 0)
            feedback_bit = (noise_shift_register & 0x0001) ^ ((noise_shift_register & 0x0002) >> 1); // Bit 0 XOR Bit 1
        }
        noise_shift_register >>= 1;
        noise_shift_register |= (feedback_bit << 14); // Set bit 14

    } else {
        noise_timer_counter--;
    }

    // Get volume
    unsigned char volume_raw = getNoiseVolumeEnvelope();
    float volume = (float)volume_raw / 15.0f; // Normalize 0-15 to 0-1

    // Determine output based on LFSR's first bit and volume
    if (!(noise_shift_register & 0x0001)) { // If bit 0 is 0
        noise_output = volume;
    } else {
        noise_output = -volume;
    }

    return noise_output;
}

void APU::updateEnvelope(int channel_index) {
    unsigned char reg0 = (channel_index == 0) ? regPulse1[0] : regPulse2[0];
    bool envelope_loop_halt = (reg0 >> 5) & 0x01; // Bit 5 (L/H)
    bool constant_volume_flag = (reg0 >> 4) & 0x01; // Bit 4 (C)
    unsigned char volume_envelope_period = reg0 & 0x0F; // Bits 3-0 (V/P)

    if (pulse_envelope_start_flag[channel_index]) {
        pulse_envelope_start_flag[channel_index] = false;
        pulse_envelope_volume[channel_index] = 0x0F;
        pulse_envelope_divider[channel_index] = volume_envelope_period;
    } else {
        if (pulse_envelope_divider[channel_index] > 0) {
            pulse_envelope_divider[channel_index]--;
        } else {
            pulse_envelope_divider[channel_index] = volume_envelope_period;
            if (pulse_envelope_volume[channel_index] > 0) {
                pulse_envelope_volume[channel_index]--;
            } else if (envelope_loop_halt) {
                pulse_envelope_volume[channel_index] = 0x0F;
            }
        }
    }

    if (constant_volume_flag) {
        pulse_envelope_volume[channel_index] = volume_envelope_period;
    }
}

void APU::updateSweep(int channel_index) {
    unsigned char reg1 = (channel_index == 0) ? regPulse1[1] : regPulse2[1];
    bool sweep_enabled = (reg1 >> 7) & 0x01;
    unsigned char sweep_period = ((reg1 >> 4) & 0x07) + 1; // Period is 1-8
    bool sweep_negative = (reg1 >> 3) & 0x01;
    unsigned char sweep_shift = reg1 & 0x07;

    if (pulse_sweep_reload_flag[channel_index]) {
        pulse_sweep_divider[channel_index] = sweep_period;
        pulse_sweep_reload_flag[channel_index] = false;
    } else {
        if (pulse_sweep_divider[channel_index] > 0) {
            pulse_sweep_divider[channel_index]--;
        } else {
            pulse_sweep_divider[channel_index] = sweep_period;
            if (sweep_enabled && sweep_shift > 0) {
                unsigned short current_period = pulse_current_period[channel_index];
                unsigned short change_amount = current_period >> sweep_shift;
                unsigned short target_period;

                if (sweep_negative) {
                    target_period = current_period - change_amount;
                    if (channel_index == 0) { // Pulse 1 has 1's complement
                        target_period--;
                    }
                } else {
                    target_period = current_period + change_amount;
                }

                if (target_period < 8 || target_period > 0x7FF) {
                    // Mute channel
                    pulse_length_counter[channel_index] = 0;
                } else {
                    pulse_current_period[channel_index] = target_period;
                }
            }
        }
    }
}

void APU::updateLengthCounter(int channel_index) {
    bool length_counter_halt;
    if (channel_index < 2) { // Pulse channels
        length_counter_halt = getLengthCounterHalt(channel_index);
        if (!length_counter_halt && pulse_length_counter[channel_index] > 0) {
            pulse_length_counter[channel_index]--;
        }
    } else if (channel_index == 2) { // Triangle channel
        length_counter_halt = getTriangleLengthCounterHalt();
        if (!length_counter_halt && triangle_length_counter > 0) {
            triangle_length_counter--;
        }
    }
    // Noise and DMC length counters would go here
}

void APU::updateTriangleLinearCounter() {
    if (triangle_linear_counter_reload_flag) {
        triangle_linear_counter = triangle_linear_counter_reload_value;
    } else {
        if (triangle_linear_counter > 0) {
            triangle_linear_counter--;
        }
    }
    if (!getTriangleLengthCounterHalt()) { // If control flag is clear
        triangle_linear_counter_reload_flag = false;
    }
}

bool APU :: getPulseStatus(int pulse_index){
    return (!pulse_index) ? (reg4015 & 0x01 != 0) : (reg4015 & 0x02 != 0);
}

bool APU :: getTriangleStatus(){
    return (reg4015 & 0x04 != 0);
}

bool APU :: getNoiseStatus(){
    return (reg4015 & 0x08 != 0);
}

bool APU :: getStepMode(){
    return (reg4017 & 0x80 != 0);
}

bool APU :: getIRQInhibitFlag(){
    return (reg4017 & 0x40 != 0);
}

void APU :: updatePulseTimer(int pulse_index){
    // Update timer counter
    if (pulse_timer_counter[pulse_index] == 0) {
        pulse_timer_counter[pulse_index] = pulse_current_period[pulse_index];
        pulse_current_sequence_step[pulse_index] = (pulse_current_sequence_step[pulse_index] + 1) % 8;
    } else {
        pulse_timer_counter[pulse_index]--; 
    }
}

void APU :: clock(){
    updatePulseTimer(0);
    updatePulseTimer(1);
}

void APU :: step(){
    
    // Mode 0: 4 Steps
    if (!getStepMode()){

        updateEnvelope(0); // Pulse 1
        updateEnvelope(1); // Pulse 2
        
        if (frame_sequencer_counter == 1 || frame_sequencer_counter == 3){
            updateLengthCounter(0); // Pulse 1
            updateLengthCounter(1); // Pulse 2
            updateLengthCounter(2); // Triangle
            updateSweep(0); // Pulse 1
            updateSweep(1); // Pulse 2
        }

        // IRQ if enabled at step 3
        if (frame_sequencer_counter == 3){
            if (!getIRQInhibitFlag()){
                // CPU IRQ
            }
        }

        frame_sequencer_counter = (frame_sequencer_counter + 1) % 4;

    // Mode 1: 5 Steps
    }else{
        
        if (frame_sequencer_counter != 3){
            updateEnvelope(0); // Pulse 1
            updateEnvelope(1); // Pulse 2
            updateTriangleLinearCounter();
        }

        if (frame_sequencer_counter == 1 || frame_sequencer_counter == 4){
            updateLengthCounter(0); // Pulse 1
            updateLengthCounter(1); // Pulse 2
            updateLengthCounter(2); // Triangle
            updateSweep(0); // Pulse 1
            updateSweep(1); // Pulse 2
        }

        frame_sequencer_counter = (frame_sequencer_counter + 1) % 5;

    }

}

void APU :: start(){
    if (player && !is_playing){
        player->Start();
        player->SetHasData(true);
        is_playing = true;
    }
}

void APU :: stop(){
    if (player && is_playing){
        player->Stop();
        player->SetHasData(false);
        is_playing = false;
    }
}

void APU::AudioCallback(void* cookie, void* buffer, size_t size, const media_raw_audio_format& format) {

    APU* apu = static_cast<APU*>(cookie);
    float* float_buffer = static_cast<float*>(buffer);
    size_t num_samples = size / sizeof(float);

    float pulse1_sample = 0;
    float pulse2_sample = 0;
    float triangle_sample = 0;
    float noise_sample = 0;

    for (size_t i = 0; i < num_samples; ++i) {
        
        float average = 0;

        for (int j = 0; j < 20; ++j){
            pulse1_sample = (apu->getPulseStatus(0)) ? apu->generatePulseSample(0) : 0;
            pulse2_sample = (apu->getPulseStatus(1)) ? apu->generatePulseSample(1) : 0;
            triangle_sample = (apu->getTriangleStatus()) ? apu->generateTriangleSample() : 0;
            //noise_sample = (apu->getNoiseStatus()) ? apu->generateNoiseSample() : 0;
            
            average += (pulse1_sample + pulse2_sample + triangle_sample) / 4.0f;
        }
        float_buffer[i] = average / 20;
    }

}