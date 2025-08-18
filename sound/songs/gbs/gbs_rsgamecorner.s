	.include "asm/macros.inc"
	.section .rodata
	.global gbs_Music_RSGameCorner
	.align 2

gbs_Music_RSGameCorner:
	channel_count 4
	channel 1, Music_RSGameCorner_Ch1
	channel 2, Music_RSGameCorner_Ch2
	channel 3, Music_RSGameCorner_Ch3
	channel 4, Music_RSGameCorner_Ch4

gbs_Music_RSGameCorner_Ch1:
	gbs_switch 0
Music_RSGameCorner_Ch1:
	tempo 104
	duty_cycle 3
	vibrato 18, 1, 5
	note_type 8, 11, 2
	octave 3
	note F_, 1
	note G_, 1
Music_RSGameCorner_Ch1_loop_main:
	duty_cycle 3
	volume_envelope 11, 2
	octave 4
	note Ds, 16
	rest 2
	octave 3
	note Ds, 6
	octave 4
	note E_, 4
	note Cs, 6
	octave 3
	note As, 6
	note G_, 2
	volume_envelope 11, 5
	note Fs, 4
	note G_, 2
	note F_, 2
	note G_, 2
	note Gs, 2
	note As, 4
	volume_envelope 11, 2
	octave 4
	note C_, 6
	note Ds, 12
	volume_envelope 9, 0
	note D_, 8
	note Ds, 2
	rest 4
	note Ds, 6
	note E_, 4
	rest 2
	note E_, 2
	rest 2
	note F_, 2
	rest 4
	note As, 2
	volume_envelope 9, 7
	octave 2
	note As, 5
	duty_cycle 0
	volume_envelope 8, 1
	octave 3
	note As, 1
	octave 4
	note C_, 4
	sound_call Music_RSGameCorner_Ch1_branch_1
	note Cs, 2
	note E_, 2
	note Cs, 2
	note E_, 2
	note F_, 4
	note C_, 2
	note Gs, 4
	note Ds, 2
	note Ds, 4
	note C_, 2
	note Gs, 4
	note Ds, 2
	note A_, 1
	note As, 1
	note A_, 1
	note As, 1
	note A_, 1
	note As, 1
	note Gs, 4
	note Gs, 2
	note F_, 2
	note D_, 2
	note F_, 2
	note D_, 2
	note C_, 2
	octave 3
	note As, 2
	octave 4
	note Ds, 4
	sound_call Music_RSGameCorner_Ch1_branch_1
	note Cs, 2
	note E_, 2
	note Cs, 2
	note E_, 2
	note F_, 4
	volume_envelope 11, 1
	octave 3
	note Gs, 8
	volume_envelope 11, 4
	note Gs, 6
	volume_envelope 11, 6
	note Gs, 6
	volume_envelope 11, 0
	note As, 3
	volume_envelope 8, 0
	note A_, 1
	volume_envelope 5, 0
	note Gs, 1
	volume_envelope 3, 2
	note Fs, 13
	duty_cycle 3
	volume_envelope 7, 0
	note Cs, 1
	note D_, 1
	note Ds, 1
	note E_, 1
	note F_, 1
	note Fs, 1
	note G_, 12
	volume_envelope 7, 7
	note G_, 12
	note F_, 12
	note Ds, 12
	note F_, 16
	volume_envelope 7, 1
	note D_, 2
	volume_envelope 7, 7
	note F_, 4
	volume_envelope 7, 1
	note E_, 2
	volume_envelope 7, 0
	note Ds, 12
	volume_envelope 7, 7
	note Ds, 10
	note C_, 2
	volume_envelope 7, 0
	note Ds, 8
	volume_envelope 7, 7
	note Ds, 10
	note F_, 4
	volume_envelope 7, 1
	note Ds, 2
	volume_envelope 7, 7
	note D_, 12
	note C_, 12
	volume_envelope 7, 0
	note D_, 12
	volume_envelope 7, 7
	note D_, 10
	volume_envelope 7, 1
	octave 2
	note As, 2
	octave 3
	volume_envelope 7, 7
	note Cs, 12
	note E_, 8
	duty_cycle 2
	volume_envelope 6, 2
	octave 5
	note Cs, 2
	octave 4
	note As, 2
	note Gs, 2
Music_RSGameCorner_Ch1_loop_1:
	octave 5
	note C_, 2
	note Ds, 2
	note G_, 2
	note Ds, 2
	note C_, 2
	octave 4
	note Gs, 2
	sound_loop 2, Music_RSGameCorner_Ch1_loop_1
	octave 5
	note C_, 2
	note D_, 2
	note F_, 2
	note D_, 2
	note C_, 2
	octave 4
	note Gs, 2
	octave 5
	note C_, 2
	note D_, 2
	note F_, 2
	note D_, 2
	note C_, 2
	octave 4
	note G_, 2
Music_RSGameCorner_Ch1_loop_2:
	note As, 2
	octave 5
	note D_, 2
	note F_, 2
	note D_, 2
	octave 4
	note As, 2
	note G_, 2
	sound_loop 2, Music_RSGameCorner_Ch1_loop_2
	note As, 2
	octave 5
	note C_, 2
	note Ds, 2
	note C_, 2
	octave 4
	note As, 2
	note G_, 2
	note As, 2
	octave 5
	note C_, 2
	note Ds, 2
	note C_, 2
	octave 4
	note As, 2
	note F_, 2
Music_RSGameCorner_Ch1_loop_3:
	note Gs, 2
	octave 5
	note C_, 2
	note Ds, 2
	note C_, 2
	octave 4
	note Gs, 2
	note F_, 2
	sound_loop 2, Music_RSGameCorner_Ch1_loop_3
	note Gs, 2
	note As, 2
	octave 5
	note D_, 2
	octave 4
	note As, 2
	note Gs, 2
	note F_, 2
	note Gs, 2
	note As, 2
	octave 5
	note D_, 2
	octave 4
	note As, 2
	note Gs, 2
	note Ds, 2
	note G_, 2
	note As, 2
	octave 5
	note D_, 2
	octave 4
	note As, 2
	note G_, 2
	note Ds, 2
	note G_, 2
	note As, 2
	octave 5
	note D_, 2
	octave 4
	note As, 2
	note G_, 2
Music_RSGameCorner_Ch1_loop_4:
	note E_, 2
	note G_, 2
	note As, 2
	octave 5
	note Cs, 2
	octave 4
	note As, 2
	note G_, 2
	sound_loop 2, Music_RSGameCorner_Ch1_loop_4
	duty_cycle 0
	volume_envelope 9, 5
	note Gs, 4
	octave 5
	note C_, 2
	volume_envelope 7, 2
	note Ds, 4
	volume_envelope 7, 1
	note G_, 6
	volume_envelope 7, 0
	octave 6
	note C_, 14
	vibrato 0, 3, 4
	volume_envelope 7, 7
	note C_, 16
	vibrato 18, 1, 5
	note D_, 2
	octave 5
	note B_, 1
	note As, 9
	volume_envelope 7, 2
	note G_, 2
	note Gs, 4
	volume_envelope 7, 0
	note As, 8
	note G_, 2
	note Gs, 2
	note As, 2
	vibrato 0, 3, 4
	volume_envelope 7, 0
	octave 6
	note D_, 10
	vibrato 18, 1, 5
	volume_envelope 7, 7
	note C_, 2
	octave 5
	note As, 4
	note G_, 2
	note A_, 1
	note Gs, 9
	volume_envelope 7, 2
	note F_, 2
	note G_, 4
	volume_envelope 7, 1
	note Gs, 6
	volume_envelope 7, 2
	note F_, 2
	note G_, 4
	volume_envelope 7, 7
	note Gs, 8
	octave 6
	note C_, 3
	octave 5
	note As, 1
	octave 6
	note C_, 1
	octave 5
	note As, 1
	note Gs, 4
	volume_envelope 7, 1
	note F_, 6
	volume_envelope 7, 6
	note G_, 8
	note Gs, 6
	note Gs, 6
	volume_envelope 7, 2
	note A_, 6
	note A_, 4
	volume_envelope 7, 6
	note As, 8
	volume_envelope 7, 0
	note B_, 4
	note Gs, 1
	note As, 1
	octave 6
	note C_, 12
	vibrato 0, 3, 4
	note C_, 12
	volume_envelope 7, 7
	note C_, 16
	rest 6
	vibrato 18, 1, 5
	volume_envelope 7, 0
	note D_, 1
	note C_, 1
	octave 5
	note As, 12
	vibrato 0, 3, 4
	volume_envelope 7, 7
	note As, 11
	vibrato 18, 1, 5
	volume_envelope 7, 0
	note G_, 1
	note As, 12
	volume_envelope 7, 7
	vibrato 0, 3, 4
	note As, 10
	vibrato 18, 1, 5
	volume_envelope 7, 0
	note As, 1
	note B_, 1
	octave 6
	note Ds, 8
	volume_envelope 7, 7
	vibrato 0, 3, 4
	note Ds, 10
	vibrato 18, 1, 5
	note D_, 4
	note Ds, 2
	octave 5
	note B_, 1
	volume_envelope 7, 0
	note As, 10
	vibrato 0, 3, 4
	volume_envelope 7, 7
	note As, 11
	vibrato 18, 1, 5
	volume_envelope 7, 0
	note G_, 1
	note Fs, 1
	note F_, 12
	volume_envelope 7, 7
	vibrato 0, 3, 4
	note F_, 10
	vibrato 18, 1, 5
	note G_, 1
	note Gs, 1
	note As, 10
	note B_, 1
	octave 6
	note C_, 1
	note D_, 12
	sound_loop 0, Music_RSGameCorner_Ch1_loop_main
	
Music_RSGameCorner_Ch1_branch_1:
	octave 3
	note As, 2
	octave 4
	note G_, 4
	note Ds, 2
	note Ds, 4
	octave 3
	note As, 2
	octave 4
	note G_, 4
	note Ds, 2
	note E_, 4
	octave 3
	note As, 2
	octave 4
	note G_, 4
	note E_, 2
	note E_, 4
	note Cs, 2
	note E_, 2
	note Cs, 2
	note E_, 2
	note F_, 4
	note C_, 2
	note Gs, 4
	note Ds, 2
	note Ds, 4
	note C_, 2
	note Gs, 4
	note Ds, 2
	note As, 4
	note As, 2
	note Gs, 4
	note Gs, 2
	note F_, 4
	note F_, 2
	note D_, 2
	note C_, 2
	note D_, 2
	note Ds, 4
	octave 3
	note As, 2
	octave 4
	note G_, 2
	note Ds, 2
	note G_, 2
	note Ds, 4
	octave 3
	note As, 2
	octave 4
	note G_, 4
	note Ds, 2
	note E_, 4
	octave 3
	note As, 2
	octave 4
	note G_, 4
	note E_, 2
	note E_, 4
	sound_ret
	
gbs_Music_RSGameCorner_Ch2:
	gbs_switch 1
Music_RSGameCorner_Ch2:
	duty_cycle 3
	vibrato 18, 2, 5
	note_type 8, 11, 2
	octave 3
	note As, 1
	octave 4
	note C_, 1
Music_RSGameCorner_Ch2_loop_main:
	note As, 16
	rest 2
	octave 3
	note As, 6
	octave 4
	note Gs, 4
	note G_, 6
	note F_, 6
	note Ds, 2
	volume_envelope 11, 5
	note D_, 4
	note Ds, 2
	octave 3
	note B_, 2
	octave 4
	note C_, 2
	note Cs, 2
	note D_, 4
	volume_envelope 11, 2
	note Ds, 6
	note G_, 12
	duty_cycle 0
	volume_envelope 9, 0
	octave 3
	note As, 8
	octave 4
	note C_, 2
	rest 4
	note C_, 6
	note Cs, 4
	rest 2
	note Cs, 2
	rest 2
	note D_, 2
	rest 4
	note As, 2
	volume_envelope 9, 7
	octave 1
	note As, 10
	duty_cycle 3
	volume_envelope 11, 2
	octave 4
	note G_, 14
	octave 3
	note As, 6
	octave 4
	note Gs, 4
	note G_, 6
	note Fs, 2
	note F_, 2
	volume_envelope 11, 1
	octave 3
	note As, 2
	octave 4
	volume_envelope 11, 5
	note F_, 2
	note G_, 10
	volume_envelope 11, 1
	note Gs, 8
	volume_envelope 11, 5
	note Gs, 12
	volume_envelope 11, 2
	note As, 4
	note Gs, 6
	volume_envelope 11, 7
	note G_, 2
	note F_, 6
	note Fs, 2
	note F_, 2
	note Fs, 2
	volume_envelope 11, 0
	note G_, 12
	volume_envelope 11, 5
	note G_, 12
	duty_cycle 2
	volume_envelope 9, 1
Music_RSGameCorner_Ch2_loop_1:
	note Cs, 1
	note As, 1
	sound_loop 12, Music_RSGameCorner_Ch2_loop_1
Music_RSGameCorner_Ch2_loop_2:
	note C_, 1
	note Gs, 1
	sound_loop 9, Music_RSGameCorner_Ch2_loop_2
	note D_, 1
	note As, 1
	note D_, 1
	note As, 1
	note C_, 1
	note Gs, 1
Music_RSGameCorner_Ch2_loop_3:
	note D_, 1
	note G_, 1
	sound_loop 6, Music_RSGameCorner_Ch2_loop_3
Music_RSGameCorner_Ch2_loop_4:
	octave 3
	note As, 1
	octave 4
	note F_, 1
	sound_loop 3, Music_RSGameCorner_Ch2_loop_4
	
	duty_cycle 0
	volume_envelope 11, 1
	note D_, 2
	note Ds, 2
	note F_, 2
	volume_envelope 11, 2
	octave 3
	note G_, 4
	octave 4
	volume_envelope 11, 0
	note G_, 2
	volume_envelope 9, 0
	note Fs, 1
	volume_envelope 7, 0
	note F_, 1
	volume_envelope 5, 0
	note E_, 1
	volume_envelope 4, 0
	note D_, 1
	volume_envelope 3, 7
	note Cs, 8
	volume_envelope 11, 2
	octave 3
	note As, 6
	volume_envelope 11, 7
	octave 4
	note Gs, 6
	volume_envelope 11, 2
	note G_, 4
	note E_, 2
	volume_envelope 11, 7
	note F_, 6
	volume_envelope 11, 2
	note G_, 4
	note E_, 6
	note Gs, 8
	volume_envelope 11, 5
	note Gs, 12
	note As, 6
	volume_envelope 11, 1
	note Gs, 2
	note As, 2
	note Gs, 2
	note G_, 2
	note Gs, 2
	note G_, 2
	note F_, 2
	note G_, 2
	note Ds, 2


	octave 3
	volume_envelope 11, 0
	note As, 12
	volume_envelope 11, 7
	note As, 6
	volume_envelope 11, 0
	octave 4
	note C_, 1
	note D_, 1
	note Ds, 1
	note F_, 1
	note G_, 1
	note Gs, 1
	note As, 12
	volume_envelope 11, 7
	note As, 16
	duty_cycle 3
	volume_envelope 13, 1
	note Gs, 8
	volume_envelope 13, 4
	note Gs, 6
	volume_envelope 13, 6
	note Gs, 6
	volume_envelope 13, 0
	note As, 2
	volume_envelope 10, 0
	note A_, 1
	volume_envelope 7, 0
	note Gs, 1
	volume_envelope 5, 2
	note Fs, 14
	rest 12
	duty_cycle 2
	volume_envelope 11, 4
	note G_, 6
	note F_, 6
	note Ds, 6
	note D_, 4
	note Ds, 6
	note E_, 6
	note F_, 2
	note D_, 4
	note C_, 2
	note Cs, 4
	note D_, 2
	octave 5
	note C_, 6
	octave 4
	note As, 6
	note Gs, 6
	note G_, 4
	note Gs, 6
	note A_, 6
	note As, 2
	note G_, 4
	volume_envelope 11, 2
	note Ds, 8
	volume_envelope 11, 4
	note Gs, 6
	note G_, 6
	note F_, 4
	note E_, 2
	note Ds, 4
	note D_, 6
	note C_, 2
	note Cs, 4
	note D_, 2
	note Ds, 4
	volume_envelope 11, 2
	note F_, 8
	volume_envelope 11, 4
	note As, 2
	note A_, 2
	note Gs, 2
	note G_, 4
	note F_, 2
	note Ds, 10
	note E_, 6
	note Ds, 2
	note E_, 4
	octave 3
	note As, 2
	octave 4
	note G_, 4
	volume_envelope 11, 2
	note E_, 8
	volume_envelope 11, 4
	note Gs, 6
	note G_, 6
	note F_, 6
	note Ds, 4
	note D_, 6
	note Cs, 2
	note D_, 4
	note Ds, 2
	note E_, 4
	volume_envelope 11, 2
	note F_, 8
	volume_envelope 11, 4
	note F_, 4
	note D_, 6
	note Cs, 2
	octave 5
	note D_, 6
	volume_envelope 11, 7
	note C_, 12
	octave 4
	note As, 10
	volume_envelope 11, 4
	note G_, 2
	note Gs, 6
	octave 5
	note Ds, 6
	note D_, 6
	note C_, 6
	note D_, 4
	note C_, 6
	note D_, 2
	octave 4
	note As, 4
	note D_, 2
	note Ds, 4
	note F_, 2
	volume_envelope 11, 7
	note As, 16
	volume_envelope 11, 4
	note D_, 2
	note Ds, 4
	note F_, 2
	volume_envelope 11, 7
	note As, 12
	octave 5
	note Cs, 12
	volume_envelope 10, 2
	octave 4
	note Ds, 10
	note C_, 2
	note G_, 6
	octave 3
	note Gs, 2
	octave 4
	note C_, 2
	note Ds, 2
	note G_, 4
	note Fs, 2
	note F_, 4
	note E_, 2
	note Ds, 4
	note D_, 2
	note C_, 4
	note D_, 2
	volume_envelope 10, 6
	octave 3
	note As, 10
	volume_envelope 10, 2
	note G_, 2
	note Gs, 4
	volume_envelope 10, 1
	note As, 2
	note G_, 2
	note Gs, 2
	note As, 2
	note G_, 2
	note Gs, 2
	note As, 2
	volume_envelope 10, 6
	octave 4
	note D_, 10
	note C_, 2
	octave 3
	note As, 4
	note G_, 2
	note Gs, 10
	volume_envelope 10, 2
	note F_, 2
	note G_, 4
	volume_envelope 10, 1
	note Gs, 6
	volume_envelope 10, 2
	note F_, 2
	note G_, 4
	volume_envelope 10, 6
	note Gs, 8
	octave 4
	note C_, 3
	octave 3
	note As, 1
	octave 4
	note C_, 1
	octave 3
	note As, 1
	note Gs, 4
	volume_envelope 10, 1
	note F_, 6
	volume_envelope 10, 5
	note G_, 8
	note Gs, 6
	note Gs, 6
	note A_, 6
	note A_, 4
	note As, 8
	note B_, 12
	note Gs, 4
	octave 4
	note C_, 2
	note Ds, 4
	volume_envelope 10, 2
	note G_, 8
	volume_envelope 10, 5
	octave 3
	note Gs, 4
	octave 4
	note C_, 2
	note Ds, 4
	note G_, 2
	octave 5
	note C_, 4
	octave 4
	note As, 1
	octave 5
	note C_, 1
	octave 4
	note F_, 4
	note C_, 2
	note D_, 6
	octave 3
	note G_, 4
	note As, 2
	octave 4
	note D_, 4
	volume_envelope 10, 2
	note F_, 8
	volume_envelope 10, 6
	octave 3
	note G_, 4
	note As, 2
	octave 4
	note D_, 4
	note As, 6
	note Gs, 1
	note As, 1
	note D_, 4
	note Ds, 2
	volume_envelope 10, 2
	note As, 4
	note Gs, 2
	note Fs, 4
	note Fs, 8
	volume_envelope 10, 5
	note F_, 4
	note Fs, 2
	note As, 6
	note Gs, 4
	note F_, 2
	note Fs, 6
	note D_, 4
	note Ds, 2
	volume_envelope 10, 0
	note F_, 6
	volume_envelope 10, 5
	note F_, 12
	note Ds, 4
	note F_, 2
	volume_envelope 10, 7
	note As, 12
	octave 5
	note D_, 6
	duty_cycle 3
	volume_envelope 11, 2
	octave 4
	note G_, 2
	note Gs, 2
	note A_, 2
	sound_loop 0, Music_RSGameCorner_Ch2_loop_main
	
gbs_Music_RSGameCorner_Ch3:
	gbs_switch 2
Music_RSGameCorner_Ch3:
	note_type 8, 1, 2
	rest 2
Music_RSGameCorner_Ch3_loop_main:
	volume_envelope 1, 2
	octave 2
	note Ds, 2
	rest 10
	rest 12
	note E_, 2
	rest 10
	rest 12
	note F_, 2
	rest 10
	rest 12
	octave 1
	note As, 2
	octave 2
	note F_, 1
	rest 1
	octave 1
	note As, 1
	rest 1
	note As, 4
	note A_, 2
	note As, 2
	octave 2
	note F_, 1
	rest 1
	octave 1
	note As, 1
	rest 1
	note As, 5
	rest 1
	note As, 2
	octave 2
	note F_, 1
	rest 1
	octave 1
	note As, 1
	rest 1
	note As, 2
	rest 2
	octave 2
	note As, 1
	rest 7
	octave 1
	note As, 4
	octave 2
	note D_, 2
	sound_call Music_RSGameCorner_Ch3_branch_1
	note D_, 2
	octave 2
	note As, 2
	note Gs, 2
	note F_, 4
	rest 2
	note As, 4
	rest 2
	octave 3
	note D_, 4
	rest 2
	octave 2
	sound_call Music_RSGameCorner_Ch3_branch_1
	octave 2
	note As, 2
	rest 4
	octave 1
	note As, 12
	note As, 6
	octave 2
	note Gs, 12
	volume_envelope 2, 2
	note Gs, 6
	volume_envelope 1, 2
	note Gs, 2
	rest 2
	note Gs, 2
	rest 4
	note Gs, 8
	octave 3
	note G_, 12
	note F_, 12
	volume_envelope 2, 2
	note F_, 4
	volume_envelope 1, 2
	note D_, 2
	note F_, 4
	note E_, 2
	note Ds, 12
	volume_envelope 2, 2
	note Ds, 10
	volume_envelope 1, 2
	octave 2
	note As, 2
	octave 3
	note F_, 12
	volume_envelope 2, 2
	note F_, 6
	volume_envelope 1, 2
	note G_, 2
	rest 2
	note F_, 2
	octave 2
	note As, 2
	rest 2
	note As, 2
	note G_, 2
	rest 2
	note Gs, 2
	note As, 2
	rest 2
	note As, 2
	octave 3
	note C_, 4
	note D_, 2
	note Ds, 10
	octave 2
	note As, 2
	octave 3
	note Ds, 4
	octave 2
	note As, 2
	octave 3
	note Ds, 2
	note D_, 1
	rest 1
	note Ds, 2
	note E_, 10
	note Cs, 2
	note G_, 10
	note E_, 2
	octave 2
	note Gs, 12
	note Gs, 2
	rest 2
	note Gs, 2
	octave 3
	note Gs, 2
	rest 2
	octave 2
	note Gs, 1
	rest 5
	note Gs, 8
	octave 3
	note Ds, 2
	note D_, 1
	rest 1
	note Cs, 2
	note C_, 6
	note D_, 12
	volume_envelope 2, 2
	note D_, 4
	volume_envelope 1, 2
	octave 2
	note G_, 1
	rest 1
	octave 3
	note D_, 4
	octave 2
	note As, 2
	octave 3
	note Ds, 3
	rest 1
	note Ds, 2
	octave 2
	note G_, 6
	octave 3
	note G_, 2
	rest 2
	note G_, 2
	note Ds, 2
	rest 2
	note C_, 2
	note F_, 12
	volume_envelope 2, 2
	note F_, 6
	volume_envelope 1, 2
	note G_, 2
	rest 2
	note F_, 2
	octave 2
	note As, 3
	rest 1
	note As, 2
	note G_, 2
	rest 2
	note Gs, 2
	note As, 2
	rest 2
	note As, 2
	octave 3
	note C_, 4
	note D_, 2
	note G_, 3
	rest 1
	note Ds, 2
	octave 2
	note As, 6
	octave 3
	note D_, 3
	rest 1
	note D_, 2
	note Ds, 6
	note E_, 4
	note Cs, 2
	octave 2
	note As, 6
	octave 3
	note E_, 4
	note Cs, 2
	note G_, 4
	octave 2
	note As, 2
	volume_envelope 1, 6
	sound_call Music_RSGameCorner_Ch3_branch_2
	note Fs, 2
	note F_, 6
	rest 12
	rest 6
	note F_, 4
	octave 3
	note F_, 1
	rest 5
	octave 2
	note F_, 2
	octave 3
	note F_, 4
	octave 2
	note F_, 1
	rest 1
	note F_, 4
	note E_, 2
	note Ds, 4
	octave 3
	note Ds, 6
	octave 2
	note E_, 2
	note F_, 4
	rest 6
	note F_, 1
	rest 1
	note F_, 2
	rest 2
	note B_, 1
	rest 5
	note As, 3
	rest 3
	note As, 1
	rest 1
	note As, 4
	note A_, 2
	sound_call Music_RSGameCorner_Ch3_branch_2
	note As, 2
	note B_, 6
	rest 12
	rest 6
	note B_, 4
	octave 3
	note Fs, 1
	rest 5
	octave 2
	note B_, 2
	octave 3
	note B_, 4
	octave 2
	note B_, 1
	rest 1
	note B_, 4
	note Fs, 2
	note As, 4
	rest 12
	rest 16
	rest 16
	sound_loop 0, Music_RSGameCorner_Ch3_loop_main
	
Music_RSGameCorner_Ch3_branch_1:
	note Ds, 2
	rest 2
	octave 3
	note Ds, 2
	note D_, 4
	rest 2
	note Cs, 4
	rest 2
	note C_, 4
	rest 2
	octave 2
	note As, 2
	note G_, 2
	note E_, 2
	note Cs, 4
	rest 2
	octave 3
	note Cs, 4
	rest 2
	note C_, 2
	rest 2
	octave 2
	note G_, 2
	note F_, 2
	rest 2
	octave 3
	note F_, 2
	note C_, 2
	note Ds, 2
	note E_, 2
	note F_, 4
	rest 2
	note E_, 2
	rest 2
	note Ds, 2
	note D_, 4
	rest 2
	note C_, 4
	rest 2
	octave 2
	note As, 4
	rest 2
	note Gs, 4
	rest 2
	note Ds, 2
	rest 2
	octave 3
	note Ds, 2
	note D_, 4
	rest 2
	note Cs, 4
	rest 2
	note C_, 4
	rest 2
	octave 2
	note As, 2
	note G_, 2
	note E_, 2
	note Cs, 4
	rest 2
	octave 3
	note Cs, 4
	rest 2
	note C_, 2
	rest 2
	octave 2
	note G_, 2
	note F_, 2
	rest 2
	octave 3
	note F_, 2
	note E_, 4
	rest 2
	note Ds, 4
	rest 2
	note D_, 4
	rest 2
	sound_ret
	
Music_RSGameCorner_Ch3_branch_2:
	note Gs, 6
	rest 12
	rest 6
	note Gs, 4
	octave 3
	note Gs, 1
	rest 5
	octave 2
	note Gs, 2
	octave 3
	note Gs, 4
	rest 2
	octave 2
	note Gs, 6
	note G_, 6
	rest 12
	rest 6
	note G_, 4
	octave 3
	note G_, 1
	rest 5
	octave 2
	note G_, 2
	octave 3
	note G_, 4
	octave 2
	note G_, 1
	rest 1
	note G_, 4
	sound_ret
	
gbs_Music_RSGameCorner_Ch4:
	gbs_switch 3
Music_RSGameCorner_Ch4:
	toggle_noise 3
	drum_speed 8
	rest 2
Music_RSGameCorner_Ch4_loop_main:
Music_RSGameCorner_Ch4_loop_1:
	drum_note 9, 4
	drum_note 2, 2
	drum_note 12, 6
	sound_loop 40, Music_RSGameCorner_Ch4_loop_1
	drum_note 11, 6
	drum_note 1, 6
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 2
	drum_note 3, 4
	drum_note 3, 2
Music_RSGameCorner_Ch4_loop_2:
	sound_call Music_RSGameCorner_Ch4_branch_1
	drum_note 3, 4
	drum_note 2, 2
	drum_note 12, 6
	drum_note 1, 4
	drum_note 1, 2
	drum_note 12, 6
	sound_call Music_RSGameCorner_Ch4_branch_1
	drum_note 3, 4
	drum_note 2, 2
	drum_note 12, 6
	drum_note 11, 2
	drum_note 11, 2
	drum_note 1, 6
	drum_note 1, 2
	sound_loop 2, Music_RSGameCorner_Ch4_loop_2
Music_RSGameCorner_Ch4_loop_3:
	drum_note 9, 6
	drum_note 9, 6
	drum_note 9, 6
	drum_note 2, 2
	drum_note 2, 2
	drum_note 2, 2
	sound_loop 8, Music_RSGameCorner_Ch4_loop_3
Music_RSGameCorner_Ch4_loop_4:
	drum_note 9, 4
	drum_note 2, 2
	drum_note 12, 6
	sound_loop 31, Music_RSGameCorner_Ch4_loop_4
	drum_note 9, 4
	drum_note 9, 2
	drum_note 1, 2
	drum_note 1, 2
	drum_note 1, 2
	sound_loop 0, Music_RSGameCorner_Ch4_loop_main
	
Music_RSGameCorner_Ch4_branch_1:
	drum_note 3, 4
	drum_note 2, 2
	drum_note 12, 6
	drum_note 1, 4
	drum_note 9, 2
	drum_note 12, 6
	sound_ret
