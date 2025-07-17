	.include "asm/macros.inc"
	.section .rodata
	.global gbs_Music_HumilauCity
	.align 2

gbs_Music_HumilauCity: @ ef2d0
	channel_count 4
	channel 1, Music_HumilauCity_Ch1
	channel 2, Music_HumilauCity_Ch2
	channel 3, Music_HumilauCity_Ch3
	channel 4, Music_HumilauCity_Ch4
@ ef2d9

gbs_Music_HumilauCity_Ch1: 	gbs_switch 0
Music_HumilauCity_Ch1: @ ef2d9
	tempo 160
	volume 7, 7
	stereo_panning FALSE, TRUE
Music_HumilauCity_Ch1_loop:
	vibrato 18, 4, 5 @ ef2e5
	duty_cycle 2
	note_type 6, 3, 4
	octave 4
	note B_, 1
	note Fs, 1
	note Cs, 1
	octave 3
	note Gs, 1
	note Ds, 1
	note A_, 1
	volume_envelope 4, 4
	octave 1
	note Fs, 1
	note A_, 1
	octave 2
	note Ds, 1
	note Gs, 1
	octave 3
	volume_envelope 5, 4
	note Cs, 1
	note Fs, 1
	note B_, 1
	octave 4
	note E_, 1
	note A_, 1
	octave 5
	note Ds, 1
	octave 3
	note_type 12, 10, 0
	note B_, 15
	volume_envelope 10, 7
	note B_, 6
	note_type 6, 6, 7
	note A_, 1
	octave 4
	volume_envelope 8, 7
	note Cs, 1
	note_type 12, 10, 7
	note Ds, 4
	note D_, 2
	note Cs, 2
	octave 3
	note As, 2
	volume_envelope 10, 0
	note B_, 16
	volume_envelope 10, 7
	note B_, 6
	octave 4
	note Cs, 2
	octave 3
	note Gs, 2
	note F_, 2
	note D_, 2
	note As, 2
	volume_envelope 10, 0
	note B_, 15
	volume_envelope 10, 7
	note B_, 6
	note_type 6, 6, 7
	note A_, 1
	octave 4
	volume_envelope 8, 7
	note Cs, 1
	note_type 12, 10, 7
	note Ds, 4
	note D_, 2
	note Cs, 2
	octave 3
	note As, 2
	volume_envelope 10, 0
	note B_, 10
	volume_envelope 10, 7
	note B_, 4
	note As, 1
	note B_, 1
	octave 4
	note Cs, 1
	rest 1
	note Cs, 1
	rest 1
	note Cs, 1
	rest 1
	note Cs, 1
	rest 3
	octave 3
	note A_, 6
@;;;;;;;;;;;;mark
	note Gs, 2
	note E_, 1
	rest 1
	note Fs, 2
	octave 2
	note B_, 1
	rest 1
	octave 3
	volume_envelope 10, 0
	note E_, 8
	volume_envelope 10, 7
	note E_, 4
	volume_envelope 6, 7
	vibrato 18, 15, 2
	note E_, 4
	volume_envelope 10, 7
	rest 2
	vibrato 18, 4, 5
	note Ds, 2
	note Fs, 2
	volume_envelope 10, 0
	note Ds, 4
	volume_envelope 10, 7
	vibrato 18, 15, 2
	note Ds, 6
	rest 2
	octave 2
	note B_, 2
	octave 3
	note Cs, 2
	note D_, 4
	note Cs, 2
	rest 2
	note F_, 2
	rest 2
	note Fs, 4
	rest 2
	note Gs, 2
	vibrato 18, 4, 5
	note E_, 1
	rest 1
	note Fs, 2
	octave 2
	note B_, 1
	rest 1
	octave 3
	volume_envelope 10, 0
	note E_, 8
	volume_envelope 10, 7
	note E_, 4
	vibrato 18, 15, 2
	volume_envelope 6, 7
	note E_, 4
	rest 2
	vibrato 18, 4, 5
	volume_envelope 10, 7
	note Ds, 2
	note Fs, 2
	volume_envelope 10, 0
	note Ds, 4
	volume_envelope 10, 7
	vibrato 18, 15, 2
	note Ds, 6
	rest 2
	octave 2
	note B_, 2
	octave 3
	note Cs, 2
	note D_, 6
	note Cs, 1
	rest 1
	note F_, 1
	rest 3
	note Gs, 4
	note A_, 2
	octave 4
@;;;;;;;;;;;;;mark
	duty_cycle 0
	vibrato 18, 4, 5
	note B_, 8
	note E_, 8
	note D_, 8
	octave 3
	note A_, 2
	octave 4
	note E_, 2 
	note Fs, 2
	note G_, 2
	note A_, 2
	note Fs, 2
	rest 2
	note D_, 2
	rest 2
	note A_, 2
	rest 2
	volume_envelope 12, 7
	note Gs, 9
	note_type 6, 12, 7
	note Fs, 1
	note F_, 1
	note_type 12, 12, 7
	note E_, 8
	note G_, 7
	note_type 6, 12, 7
	note Fs, 1
	note F_, 1
	note_type 12, 12, 7
	note E_, 8
	note A_, 7
	note_type 6, 12, 7
	note Gs, 1
	note G_, 1
	note_type 12, 12, 7
	note Fs, 8
	note B_, 6
	rest 4
	note B_, 2
	octave 5
	note C_, 2
	octave 4
	note A_, 2
	note E_, 1
	rest 1
	note E_, 1
	rest 1
	note E_, 1
	rest 1
	note Fs, 1
	rest 1
	duty_cycle 2
	note_type 6, 3, 4
	octave 4
	note B_, 1
	note Fs, 1
	note Cs, 1
	octave 3
	note Gs, 1
	note Ds, 1
	note A_, 1
	volume_envelope 4, 4
	octave 1
	note Fs, 1
	note A_, 1
	octave 2
	note Ds, 1
	note Gs, 1
	octave 3
	volume_envelope 5, 4
	note Cs, 1
	note Fs, 1
	note B_, 1
	octave 4
	note E_, 1
	note A_, 1
	octave 5
	note Ds, 1
	octave 3
	note_type 12, 11, 7
@;;;;;;;;;;;;;;mark
	note Gs, 6
	vibrato 18, 15, 2
	note E_, 6
	rest 2
	vibrato 18, 5, 5
	note D_, 1
	note E_, 1
	note Fs, 2
	rest 4
	octave 4
	volume_envelope 11, 0
	note Ds, 2
	volume_envelope 11, 7
	vibrato 18, 15, 2
	note Ds, 6
	note D_, 1
	note Cs, 1
	octave 3
	note B_, 2
	vibrato 18, 5, 5
	rest 4
	octave 4
	note Fs, 6
	note Ds, 2
	octave 3
	note B_, 2
	octave 4
	note E_, 6
	vibrato 18, 15, 2
	note Cs, 2
	vibrato 18, 5, 5
	rest 6
	note Ds, 2
	note E_, 2
	rest 2
	note Cs, 2
	note E_, 2
	rest 2
	note Cs, 2
	note E_, 2
	note Cs, 1
	note Ds, 1
	vibrato 18, 15, 2
	note Fs, 6
	note Ds, 2
	rest 2
	note Cs, 2
	rest 2
	vibrato 18, 5, 5
	note Ds, 1
	note Cs, 1
	octave 3
	note B_, 6
	vibrato 18, 15, 2
	volume_envelope 6, 7
	note B_, 6
	vibrato 18, 5, 5
	volume_envelope 11, 7
	note Gs, 2
	note A_, 2
	octave 4
	vibrato 18, 15, 2
	note D_, 6
	vibrato 18, 5, 5
	note Cs, 2
	rest 2
	octave 3
	note Gs, 2
	rest 2
	note A_, 2
@;;;;;;;;;;;;;;;mark
	duty_cycle 0
	vibrato 18, 4, 5
	octave 4
	note B_, 6
	note E_, 2
	rest 6
	note Ds, 1
	note E_, 1
	note Fs, 3
	rest 3
	octave 5
	volume_envelope 11, 0
	note Ds, 2
	volume_envelope 11, 7
	note Ds, 6
	note D_, 1
	note Cs, 1
	octave 4
	note B_, 3
	rest 3
	octave 5
	note Fs, 6
	note A_, 2
	rest 2
	note Gs, 2
	note A_, 2
	note B_, 2
	note F_, 2
	note Fs, 2
	note Gs, 2
	note D_, 2
	note Cs, 2
	note E_, 2
	rest 2
	note Cs, 2
	note E_, 2
	rest 2
	note Cs, 2
	note E_, 2
	note E_, 1
	note Fs, 1
	volume_envelope 11, 0
	note Gs, 4
	volume_envelope 11, 7
	note Gs, 5
	rest 1
	note A_, 2
	rest 2
	note Fs, 2
	volume_envelope 11, 0
	note E_, 5
	volume_envelope 11, 7
	note E_, 7
	rest 2
	note E_, 1
	note Fs, 1
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	sound_loop 0, Music_HumilauCity_Ch1_loop

gbs_Music_HumilauCity_Ch2: 	gbs_switch 1
Music_HumilauCity_Ch2: @ ef3eb
	vibrato 18, 2, 5
	stereo_panning TRUE, FALSE
	pitch_offset 2
Music_HumilauCity_Ch2_loop:
	duty_cycle 2
	note_type 12, 8, 3
	octave 4
	note B_, 2
	note Gs, 2
	note A_, 2
	note Fs, 2
	octave 3
	volume_envelope 8, 0
	duty_cycle 0
	note Cs, 2
	volume_envelope 8, 7
	note Cs, 6
	volume_envelope 8, 0
	note E_, 2
	volume_envelope 8, 7
	note E_, 6
	note Fs, 5
	note_type 3, 8, 7
	note Fs, 1
	note Gs, 1
	note A_, 1
	note As, 1
	note_type 12, 8, 7
	note B_, 4
	note As, 2
	note A_, 2
	note G_, 2
	note Ds, 6
	note D_, 1
	note Ds, 1
	note Fs, 6
	note F_, 1
	note Fs, 1
	note Gs, 6
	note B_, 2
	note D_, 2
	octave 2
	note B_, 2
	note Gs, 2
	octave 3
	note C_, 2
	note Cs, 6
	note C_, 1
	note Cs, 1
	note E_, 6
	note Ds, 1
	note E_, 1
	note Fs, 2
	note Fs, 3
	note_type 3, 8, 7
	rest 1
	note Gs, 1
	note A_, 1
	note As, 1
	note_type 12, 8, 7
	note B_, 4
	note As, 2
	note A_, 2
	note G_, 2
	note Ds, 6
	note D_, 1
	note Ds, 1
	note Fs, 6
	note Gs, 1
	note A_, 1
	note As, 1
	rest 1
	note As, 1
	rest 1
	note As, 1
	rest 1
	note As, 1
	rest 3
	note A_, 6
@;;;;;;;;;;;;;;mark
	volume_envelope 6, 4
	note Gs, 2
	note Gs, 2
	note Gs, 2
	note G_, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	octave 4
	note Cs, 2
	octave 3
	note Gs, 2
	note B_, 2
	note A_, 2
	note Gs, 2
	note As, 2
	note As, 2
	note As, 1
	note As, 1
	note As, 2
	note Fs, 2
	octave 4
	note Fs, 2
	note Ds, 2
	octave 3
	note B_, 2
	note A_, 2
	note A_, 2
	note A_, 2
	note A_, 2
	note A_, 2
	note A_, 3
	rest 1
	note G_, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	note G_, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	note Gs, 2
	octave 4
	note Ds, 2
	octave 3
	note Gs, 2
	note Gs, 2
	note A_, 2
	note Gs, 2
	note As, 2
	note As, 2
	note As, 2
	note As, 2
	note Fs, 2
	octave 4
	note Fs, 2
	note Ds, 2
	octave 3
	note B_, 2
	note A_, 2
	note A_, 2
	note A_, 2
	note G_, 4
	note A_, 2
	note A_, 4
@;;;;;;;;;;;;mark
	volume_envelope 8, 2
	octave 4
	note E_, 1
	octave 3
	note B_, 1
	note G_, 1
	note E_, 1
	octave 4
	note E_, 1
	rest 1
	octave 3
	note G_, 1
	note E_, 1
	octave 4
	note G_, 1
	note E_, 1
	octave 3
	note B_, 1
	note G_, 1
	note B_, 1
	note G_, 1
	note E_, 1
	octave 2
	note B_, 1
	volume_envelope 10, 7
	octave 3
	note Fs, 8
	note A_, 6
	volume_envelope 8, 2
	octave 3
	note As, 1
	octave 4
	note C_, 1
	note D_, 1
	octave 3
	note A_, 1
	note Fs, 1
	note D_, 1
	octave 4
	note Fs, 1
	rest 1
	note D_, 1
	octave 3
	note A_, 1
	octave 4
	note A_, 1
	note D_, 1
	octave 3
	note A_, 1
	note Fs, 1
	note A_, 1
	note Fs, 1
	note D_, 1
	octave 4
	note A_, 1
	note_type 6, 10, 7
	octave 3
	note B_, 14
	note As, 1
	note A_, 1
	note Gs, 4
	note B_, 7
	note As, 1
	note A_, 2
	note Gs, 2
	octave 4
	note C_, 14
	octave 3
	note B_, 1
	note As, 1
	note A_, 4
	octave 4
	note C_, 8
	octave 3
	note B_, 2
	note A_, 2
	octave 4
	note D_, 14
	note Cs, 1
	note C_, 1
	octave 3
	note B_, 4
	octave 4
	note D_, 8
	note C_, 2
	octave 3
	note A_, 2
	octave 4
	note_type 12, 8, 2
	note E_, 1
	octave 3
	note B_, 1
	note G_, 1
	note E_, 1
	octave 4
	note G_, 1
	rest 1
	octave 3
	note B_, 1
	note G_, 1
	octave 4
	note B_, 1
	note G_, 1
	note E_, 1
	octave 3
	note B_, 1
	octave 4
	note G_, 1
	note E_, 1
	octave 3
	note B_, 1
	note G_, 1
	volume_envelope 10, 7
	octave 4
	note C_, 1
	rest 1
	note C_, 1
	rest 1
	note C_, 1
	rest 1
	octave 3
	note B_, 1
	rest 1
	volume_envelope 8, 3
	duty_cycle 2
	octave 4
	note B_, 2
	note Gs, 2
	note A_, 2
	note Fs, 2
@;;;;;;;;;;;;;mark
	volume_envelope 8, 7
	octave 3
	note E_, 6
	note Cs, 6
	rest 2
	octave 2
	note B_, 1
	octave 3
	note Cs, 1
	note Ds, 2
	rest 4
	note B_, 8
	note A_, 1
	note Gs, 1
	note Fs, 2
	rest 4
	octave 4
	note Ds, 6
	octave 3
	note B_, 2
	note Fs, 2
	octave 4
	note Cs, 6
	octave 3
	note Gs, 2
	rest 6
	octave 4
	note C_, 2
	note Cs, 2
	rest 2
	octave 3
	note A_, 2
	octave 4
	note Cs, 2
	rest 2
	octave 3
	note A_, 2
	octave 4
	note Cs, 2
	octave 3
	note A_, 2
	octave 4
	note Cs, 6
	octave 3
	note A_, 2
	rest 2
	note Fs, 2
	rest 2
	note A_, 1
	note Fs, 1
	note Gs, 6
	note G_, 2
	rest 2
	note Gs, 2
	octave 2
	note F_, 2
	note Fs, 2
	note Gs, 6
	note F_, 2
	rest 2
	note Gs, 2
	octave 3
	note D_, 2
	rest 2
	duty_cycle 0
	note E_, 6
	note Cs, 1
	note Ds, 1
	note E_, 2
	note Gs, 2
	rest 2
	note A_, 1
	note Gs, 1
	note Ds, 2
	rest 4
	octave 2
	note B_, 1
	octave 3
	note Cs, 1
	note Ds, 2
	note Fs, 2
	rest 2
	note A_, 1
	note Gs, 1
	note Fs, 2
	rest 4
	octave 2
	note B_, 1
	octave 3
	note Cs, 1
	note Ds, 2
	note Ds, 1
	note E_, 1
	note Fs, 2
	note Fs, 1
	note Gs, 1
	note B_, 2
	octave 4
	note Cs, 1
	rest 1
	octave 3
	note Gs, 2
	note B_, 1
	rest 1
	note F_, 2
	note Gs, 1
	rest 1
	note Cs, 2
	note F_, 1
	note Gs, 1
	octave 4
	note Cs, 2
	rest 2
	octave 3
	note A_, 2
	octave 4
	note Cs, 2
	rest 2
	octave 3
	note A_, 2
	octave 4
	note Cs, 2
	note Cs, 1
	note Ds, 1
	note E_, 8
	note C_, 4
	octave 3
	note A_, 4
	octave 4
	note C_, 4
	octave 3
	note A_, 4
	note E_, 4
	note C_, 4
	octave 4
	note C_, 1
	rest 1
	note C_, 1
	rest 1
	note C_, 1
	rest 1
	note C_, 1
	rest 1
	sound_loop 0, Music_HumilauCity_Ch2_loop

gbs_Music_HumilauCity_Ch3: 	gbs_switch 2
Music_HumilauCity_Ch3: @ ef4e1
	stereo_panning TRUE, TRUE
	vibrato 18, 1, 5
	note_type 12, 1, 3
	octave 2
	rest 8
Music_HumilauCity_Ch3_loop:
	note Fs, 4
	rest 2
	note Fs, 1
	rest 1
	note Fs, 1
	rest 1
	octave 3
	note Cs, 4
	note C_, 2
	octave 2
	note B_, 4
	rest 2
	note B_, 1
	rest 1
	note A_, 1
	rest 1
	note A_, 1
	rest 1
	note B_, 4
	note Gs, 4
	rest 2
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	note Ds, 2
	octave 3
	note Ds, 2
	note D_, 2
	note Cs, 4
	rest 2
	note Cs, 1
	rest 1
	octave 2
	note Cs, 1
	rest 1
	octave 3
	note Cs, 2
	rest 2
	octave 2
	note Cs, 1
	rest 1
	note Fs, 4
	rest 2
	note Fs, 1
	rest 1
	note Fs, 1
	rest 1
	octave 3
	note Cs, 2
	note C_, 2
	octave 2
	note B_, 1
	rest 1
	note B_, 1
	rest 1
	octave 1
	note B_, 4
	octave 2
	note B_, 1
	rest 1
	note A_, 1
	rest 1
	note A_, 1
	rest 1
	note Fs, 4
	note Gs, 4
	rest 2
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	octave 3
	note Ds, 4
	note D_, 2
	octave 2
	note Cs, 1
	rest 1
	note Cs, 1
	rest 1
	note Cs, 1
	rest 1
	note Cs, 1
	rest 3
	note Cs, 6
@;;;;;;;;;;;;;mark
	note Fs, 1
	rest 1
	note Fs, 1
	rest 1
	volume_envelope 2, 3
	note Fs, 1
	rest 1
	volume_envelope 1, 3
	note Fs, 1
	rest 1
	volume_envelope 2, 3
	note Fs, 1
	rest 1
	note Fs, 1
	rest 1
	volume_envelope 1, 3
	note Fs, 1
	rest 1
	volume_envelope 2, 3
	note Fs, 1
	rest 1
	volume_envelope 1, 3
	note B_, 1
	rest 1
	note B_, 1
	rest 1
	volume_envelope 2, 3
	note B_, 1
	rest 1
	volume_envelope 1, 3
	note B_, 1
	rest 1
	volume_envelope 2, 3
	note B_, 1
	rest 1
	note B_, 1
	rest 1
	volume_envelope 1, 3
	note B_, 1
	rest 1
	volume_envelope 2, 3
	note B_, 1
	rest 1
	volume_envelope 1, 3
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	volume_envelope 2, 3
	note Gs, 1
	rest 1
	volume_envelope 1, 3
	note Gs, 1
	rest 1
	volume_envelope 2, 3
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	volume_envelope 1, 3
	note Gs, 1
	rest 1
	volume_envelope 2, 3
	note Gs, 1
	rest 1
	volume_envelope 1, 3
	note Cs, 1
	rest 1
	note Cs, 1
	rest 1
	volume_envelope 2, 3
	note Cs, 1
	rest 1
	volume_envelope 1, 3
	note Cs, 1
	rest 1
	volume_envelope 2, 3
	note Cs, 1
	rest 1
	volume_envelope 1, 3
	note Cs, 4
	note F_, 2
	note Fs, 1
	rest 1
	note Fs, 1
	rest 1
	volume_envelope 2, 3
	note Fs, 1
	rest 1
	volume_envelope 1, 3
	note Fs, 1
	rest 1
	note Fs, 2
	note B_, 1
	rest 1
	note A_, 2
	octave 3
	note Cs, 1
	rest 1
	octave 2
	note B_, 1
	rest 1
	note B_, 1
	rest 1
	volume_envelope 2, 3
	note B_, 1
	rest 1
	volume_envelope 1, 3
	note B_, 1
	rest 1
	volume_envelope 2, 3
	note A_, 1
	rest 1
	note A_, 1
	rest 1
	volume_envelope 1, 3
	note B_, 1
	rest 1
	volume_envelope 2, 3
	note B_, 1
	rest 1
	volume_envelope 1, 3
	note Gs, 1
	rest 1
	note Gs, 1
	rest 1
	volume_envelope 2, 3
	note Gs, 1
	rest 1
	volume_envelope 1, 3
	note Gs, 1
	rest 1
	volume_envelope 2, 3
	note Fs, 1
	rest 1
	note Fs, 1
	rest 1
	volume_envelope 1, 3
	note Gs, 1
	rest 1
	volume_envelope 2, 3
	note F_, 1
	rest 1
	volume_envelope 1, 3
	note Cs, 1
	rest 1
	note Cs, 1
	rest 1
	volume_envelope 2, 3
	note Cs, 1
	rest 1
	volume_envelope 1, 3
	note Cs, 1
	rest 1
	volume_envelope 2, 3
	note Cs, 1
	rest 1
	note Cs, 1
	rest 1
	volume_envelope 1, 3
	note B_, 2
	note D_, 1
	note Cs, 1
@;;;;;;;;;;;;mark
	note C_, 4
	octave 3
	note C_, 1
	rest 1
	octave 2
	note D_, 1
	note C_, 1
	rest 2
	note C_, 2
	octave 3
	note C_, 2
	octave 2
	note D_, 1
	note Cs, 1
	note C_, 4
	rest 2
	note C_, 1
	note D_, 1
	octave 3
	note C_, 2
	octave 2
	note G_, 2
	note E_, 2
	note D_, 2
	octave 1
	note B_, 4
	octave 2
	note B_, 1
	rest 1
	note C_, 1
	octave 1
	note B_, 1
	rest 2
	note B_, 2
	octave 2
	note B_, 2
	note Fs, 1
	note F_, 1
	note D_, 2
	note B_, 1
	rest 3
	octave 1
	note B_, 2
	octave 2
	note B_, 2
	note Gs, 2
	note E_, 2
	octave 1
	note B_, 1
	note As, 1
	note A_, 4
	octave 2
	note A_, 1
	rest 1
	octave 1
	note B_, 1
	note A_, 1
	rest 2
	note A_, 2
	octave 2
	note A_, 2
	octave 1
	note A_, 2
	note B_, 1
	octave 2
	note B_, 1
	octave 1
	note B_, 2
	octave 2
	note B_, 1
	rest 1
	note C_, 1
	octave 1
	note B_, 1
	rest 2
	note B_, 1
	rest 1
	note B_, 1
	octave 2
	note B_, 1
	octave 1
	note B_, 1
	octave 2
	note B_, 1
	octave 3
	note C_, 1
	octave 2
	note C_, 1
	rest 1
	note C_, 1
	octave 3
	note C_, 1
	rest 1
	octave 2
	note C_, 1
	note B_, 1
	octave 3
	note C_, 1
	octave 2
	note C_, 3
	octave 3
	note C_, 2
	octave 2
	note A_, 2
	octave 1
	note B_, 2
	octave 3
	note Ds, 2
	note C_, 2
	octave 2
	note B_, 2
	octave 1
	note B_, 2
	note As, 2
	octave 2
	note B_, 4
@;;;;;;;;;;;;;;mark
	note A_, 2
	note Gs, 3
	rest 1
	note A_, 2
	rest 8
	note A_, 2
	note Gs, 3
	rest 1
	note A_, 2
	rest 8
	note Gs, 2
	note Ds, 3
	rest 1
	note B_, 2
	rest 4
	octave 3
	note C_, 2
	rest 2
	note Cs, 2
	note C_, 2
	rest 2
	note Cs, 2
	rest 2
	octave 2
	note Cs, 2
	octave 3
	note Cs, 3
	rest 1
	octave 2
	note Fs, 2
	note F_, 2
	rest 2
	note Fs, 2
	rest 4
	note Fs, 2
	rest 2
	note Fs, 2
	note F_, 2
	rest 2
	note Fs, 2
	rest 2
	note B_, 2
	octave 3
	note Ds, 3
	rest 1
	octave 2
	note E_, 2
	rest 4
	note Ds, 2
	rest 2
	note E_, 2
	note B_, 2
	note Ds, 2
	note Cs, 2
	note C_, 2
	rest 2
	note Cs, 2
	rest 2
	note F_, 2
	note Gs, 2
	rest 2
	note A_, 2
	rest 4
	note A_, 2
	rest 8
	note A_, 1
	rest 1
	note A_, 2
	rest 2
	note A_, 2
	rest 8
	note Gs, 2
	rest 4
	note Gs, 2
	rest 2
	note Cs, 2
	note B_, 2
	rest 2
	octave 3
	note Cs, 1
	rest 1
	note Cs, 2
	rest 2
	octave 2
	note Cs, 2
	rest 2
	note Cs, 2
	note E_, 2
	note F_, 2
	note Fs, 3
	rest 3
	octave 3
	note E_, 2
	rest 2
	note Ds, 2
	note D_, 2
	note Cs, 2
	octave 2
	note B_, 2
	octave 1
	note B_, 2
	rest 2
	octave 2
	note B_, 2
	rest 2
	octave 1
	note B_, 2
	octave 2
	note B_, 2
	rest 2
	note E_, 2
	rest 4
	note E_, 2
	rest 4
	note E_, 2
	note D_, 2
	octave 1
	note B_, 2
	octave 2
	note B_, 2
	octave 1
	note B_, 2
	octave 2
	note B_, 2
	rest 2
	octave 1
	note B_, 6
	sound_loop 0, Music_HumilauCity_Ch3_loop

gbs_Music_HumilauCity_Ch4: 	gbs_switch 3
Music_HumilauCity_Ch4: @ f517a
	toggle_noise 3
	stereo_panning FALSE, TRUE
	drum_speed 12
	rest 8
Music_HumilauCity_Ch4_loop:
	drum_note 4, 4
	drum_note 1, 2
	drum_note 4, 2
	drum_note 4, 2
	drum_note 4, 2
	drum_note 1, 2
	drum_note 4, 2
	sound_loop 0, Music_HumilauCity_Ch4_loop
