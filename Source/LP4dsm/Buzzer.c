#include "LP4DSM.h"
#include "BUZZER.h"

//global data *********************************************************
 note_t buz_notes[NOTE_BUFFER_SIZE];
uint8_t buz_note_idx;
 uint8_t buz_note_ptr;
 uint16_t buz_note_time;

//return number of elements in buffer
uint8_t buz_SequenceCount(void)
{
	return (buz_note_ptr-buz_note_idx+NOTE_BUFFER_SIZE)%NOTE_BUFFER_SIZE;
}


//turn on the Buzzer
void Buzz(uint8_t octave,uint8_t cnt)
{
	TCCR2B=octave;
	OCR2B=cnt;
	OCR2A=cnt;
	TCCR2A = (1<<WGM21)|(1<<COM2B0); // CTC mode toggle OC2B
}

//turn off the Buzzer
void BuzzOff()
{
	TCCR2B = 0x00; // disconnect OC2B
	BUZZER_PORT &= ~(1<<BUZZER_PIN); //  set PD3 low
}

//initialize the buzzer logic
 void InitBuzzer(void)
{
	buz_note_idx=0;
	buz_note_ptr=0;
	buz_note_time=0;
	BUZZER_DDR|=(1<<BUZZER_PIN); //PD3 as output 
	BuzzOff();
}


//add a note to the play buffer
 void buz_PlayNote(note_t note)
{
	note_t* n=&buz_notes[buz_note_ptr];
	buz_note_ptr=(buz_note_ptr+1)%NOTE_BUFFER_SIZE;
	n->octave =note.octave;
	n->cnt=note.cnt;
	n->duration=note.duration;  
	//enable interrupts
	//cyclic callback  will start the note
}

//add a group of notes to the buffer
void buz_PlayMelody(uint8_t cnt,const note_t *notes)
{
	for(uint8_t n=0;n<cnt;n++)
	{
		buz_PlayNote(notes[n]);
	}
}


//load the next tone to play
inline  void buz_PlayNextNote(void)
{
	buz_note_time=buz_notes[buz_note_idx].duration;
	Buzz( buz_notes[buz_note_idx].octave, buz_notes[buz_note_idx].cnt);
	buz_note_idx=(buz_note_idx+1)%NOTE_BUFFER_SIZE;
}


//called once per ms
void BuzzerCyclic(void)
{
	if(buz_note_time) //a tone is on
	{	//if done turn off
		if(0==(--buz_note_time))
		{
		BuzzOff();	
		}
	}
	//check for next note
	else 
	{
		if(buz_note_idx!=buz_note_ptr)
		{
			buz_PlayNextNote();
		}
	}
}

