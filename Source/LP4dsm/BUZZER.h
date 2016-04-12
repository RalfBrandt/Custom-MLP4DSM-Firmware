#ifndef BUZZER_H
#define BUZZER_H

/**********defines ******************************************************************************************/
#define NOTE_DURATION(BPM,RATIO) ((60000UL)/(BPM*RATIO))

#define NOTE_BUFFER_SIZE 16
#define BUZZER_PIN PORTD3
#define BUZZER_PORT PORTD
#define BUZZER_DDR DDRD

typedef struct
{
	uint8_t octave;
	uint8_t cnt;
	uint16_t duration;
}note_t;


//prototypes
void InitBuzzer(void);
void buz_PlayMelody(uint8_t cnt,const note_t *notes);
void buz_PlayNote(note_t note);
void BuzzerCyclic(void);
uint8_t buz_SequenceCount(void);
void Buzz(uint8_t octave,uint8_t cnt);


//global data 


//the notes
//%32
#define OCT4  ((1<< CS20)|(1<< CS21))
#define C4_CNT	250 //261,626Hz
#define CS4_CNT	235	//277,183Hz
#define D4_CNT	222 //293,665Hz
#define DS4_CNT	209 //311,127Hz
#define E4_CNT	198 //329,628Hz
#define F4_CNT	186 //349,228Hz
#define FS4_CNT	176 //369,994Hz
#define G4_CNT	166 //391,995Hz
#define GS4_CNT 157 //415,305Hz
#define A4_CNT  148 //440,000Hz
#define AS4_CNT 140 //466,164Hz
#define B4_CNT	132 //493,883Hz

//%32
#define OCT5 ((1<< CS20)|(1<< CS21))	
#define C5_CNT	122 //523,251Hz
#define CS5_CNT	117 //554,365Hz
#define D5_CNT	111 //587,330Hz
#define DS5_CNT	104 //622,254Hz
#define E5_CNT	98  //659,255Hz
#define F5_CNT 	93  //698,456Hz
#define FS5_CNT	88  //739,989Hz
#define G5_CNT	83  //783,991Hz
#define GS5_CNT	78  //830,609Hz
#define A5_CNT	73  //880,000Hz
#define AS5_CNT	69  //932,328Hz
#define B5_CNT	66  //987,767Hz

//%8
#define OCT6 (1<<CS21)	
#define C6_CNT	249 //1046,50Hz
#define CS6_CNT	236 //1108,73Hz
#define D6_CNT	222 //1174,66Hz
#define DS6_CNT	210 //1244,51Hz
#define E6_CNT	198 //1318,51Hz
#define F6_CNT 	187 //1396,91Hz
#define FS6_CNT	176 //1479,98Hz
#define G6_CNT	166 //1567,98Hz
#define GS6_CNT	157 //1661,22Hz
#define A6_CNT	148 //1760,00Hz
#define AS6_CNT	139 //1864,66Hz
#define B6_CNT	132 //1975,53Hz


//%8
#define OCT7 (1<<CS21)
#define C7_CNT	124 //2093,00Hz
#define C7S_CNT 117 //2217,46Hz
#define D7_CNT	111 //2349,32Hz
#define DS7_CNT	104 //2489,02Hz
#define E7_CNT	98  //2637,02Hz
#define F7_CNT 	93  //2793,83Hz
#define FS7_CNT	88  //2959,96Hz
#define G7_CNT	83  //3135,96Hz
#define GS7_CNT	78  //3322,44Hz
#define A7_CNT	73  //3520,00Hz
#define AS7_CNT	69  //3729,31Hz
#define B7_CNT	65 //3951,07Hz

//%8
#define OCT8 (1<<CS21)
#define C8_CNT	61 //4186,01Hz
#define CS8_CNT	58 //4434.92Hz	
#define D8_CNT	55 //4698.64Hz	
#define DS8_CNT	52 //4978.04Hz
#define E8_CNT	49 //5274.04Hz
#define F8_CNT 	46 //5587.66Hz
#define FS8_CNT	43 //5919.92Hz
#define G8_CNT	41 //6271.92Hz
#define GS8_CNT	38 //6644.88Hz
#define A8_CNT	36 //7040.00Hz
#define AS8_CNT	34 //7458.62Hz
#define B8_CNT	32 //7902.14Hz

#endif //BUZZER