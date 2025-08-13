	.include "asm/macros.inc"
	.section .rodata
	.global gbs_Music_HoennWildVictory
	.align 2

gbs_Music_HoennWildVictory:
	channel_count 3
	channel 1, Music_HoennWildVictory_Ch1
	channel 2, Music_HoennWildVictory_Ch2
	channel 3, Music_HoennWildVictory_Ch3

gbs_Music_HoennWildVictory_Ch1:
	gbs_switch 0
Music_HoennWildVictory_Ch1:
	duty_cycle 0
	vibrato 18, 1, 5
	pitch_offset 1
	tempo 136
	note_type 12, 8, 0
	octave 3
	note Gs, 1
	note A_, 1
	note B_, 1
	octave 4
	note Ds, 1
	note E_, 2
	volume_envelope 8, 2
	octave 5
	note Ds, 2
	note E_, 2
	volume_envelope 6, 2
	note E_, 2
	volume_envelope 8, 2
	note E_, 2
	volume_envelope 6, 2
	note E_, 2
Music_HoennWildVictory_Ch1_loop:
	sound_call Music_HoennWildVictory_Ch1_branch_1
	transpose 0, 1
	sound_call Music_HoennWildVictory_Ch1_branch_1
	transpose 0, 0
	sound_loop 0, Music_HoennWildVictory_Ch1_loop
	
Music_HoennWildVictory_Ch1_branch_1:
	rest 2
	volume_envelope 7, 2
	octave 3
	note E_, 4
	note E_, 4
	note E_, 4
	note E_, 2
	volume_envelope 6, 2
	octave 4
	note A_, 2
	note A_, 2
	volume_envelope 6, 0
	note A_, 4
	volume_envelope 6, 2
	note Fs, 2
	note Fs, 2
	note Fs, 6
	volume_envelope 7, 2
	octave 3
	note E_, 4
	note E_, 4
	note E_, 4
	note E_, 2
	volume_envelope 6, 2
	octave 4
	note A_, 2
	note A_, 2
	volume_envelope 6, 0
	note A_, 4
	volume_envelope 6, 2
	note Gs, 8
	sound_ret
	
gbs_Music_HoennWildVictory_Ch2:
	gbs_switch 1
Music_HoennWildVictory_Ch2:
	duty_cycle 3
	vibrato 18, 1, 5
	pitch_offset 1
	note_type 12, 10, 0
	octave 4
	note E_, 1
	note Fs, 1
	note Gs, 1
	note A_, 1
	note B_, 2
	volume_envelope 10, 7
	note B_, 10
Music_HoennWildVictory_Ch2_loop:
	sound_call Music_HoennWildVictory_Ch2_branch_1
	note B_, 2
	note B_, 2
	note B_, 4
	sound_call Music_HoennWildVictory_Ch2_branch_1
	note B_, 4
	transpose 0, 1
	octave 3
	note B_, 1
	octave 4
	note Ds, 1
	note Fs, 1
	note A_, 1
	sound_call Music_HoennWildVictory_Ch2_branch_1
	note B_, 2
	note B_, 2
	note B_, 4
	sound_call Music_HoennWildVictory_Ch2_branch_1
	note B_, 4
	transpose 0, 0
	octave 3
	note B_, 1
	octave 4
	note Ds, 1
	note Fs, 1
	note A_, 1
	sound_loop 0, Music_HoennWildVictory_Ch2_loop
	
Music_HoennWildVictory_Ch2_branch_1:
	volume_envelope 8, 2
	note B_, 2
	note A_, 2
	note Gs, 2
	note A_, 2
	note B_, 2
	note B_, 2
	note B_, 4
	octave 5
	note Cs, 2
	note Cs, 2
	volume_envelope 8, 7
	note Cs, 4
	volume_envelope 8, 2
	octave 4
	sound_ret
	
gbs_Music_HoennWildVictory_Ch3:
	gbs_switch 2
Music_HoennWildVictory_Ch3:
	note_type 12, 2, 2
	rest 4
	octave 3
	note E_, 4
	octave 2
	note B_, 4
	note Gs, 4
Music_HoennWildVictory_Ch3_loop:
	sound_call Music_HoennWildVictory_Ch3_branch_1
	transpose 0, 1
	octave 2
	note B_, 4
	sound_call Music_HoennWildVictory_Ch3_branch_1
	transpose 0, 0
	octave 2
	note B_, 4
	sound_loop 0, Music_HoennWildVictory_Ch3_loop
	
Music_HoennWildVictory_Ch3_branch_1:
	octave 3
	note E_, 4
	octave 2
	note B_, 4
	octave 3
	note E_, 4
	octave 2
	note B_, 4
	octave 3
	note A_, 8
	note B_, 8
	note E_, 4
	octave 2
	note B_, 4
	octave 3
	note E_, 4
	octave 2
	note B_, 4
	octave 3
	note A_, 4
	rest 4
	note E_, 4
	sound_ret
