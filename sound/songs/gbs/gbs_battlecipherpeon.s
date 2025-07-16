	.include "asm/macros.inc"
	.section .rodata
	.global gbs_Music_CipherPeonBattle
	.align 2

gbs_Music_CipherPeonBattle:
	channel_count 4
	channel 1, Music_CipherPeonBattle_Ch1
	channel 2, Music_CipherPeonBattle_Ch2
	channel 3, Music_CipherPeonBattle_Ch3
	channel 4, Music_CipherPeonBattle_Ch4

gbs_Music_CipherPeonBattle_Ch1:
	gbs_switch 0
Music_CipherPeonBattle_Ch1:
	duty_cycle 2
	vibrato 18, 1, 5
	pitch_offset 1
	tempo 128
	note_type 12, 8, 1
	octave 6
	note C_, 2
	octave 5
	note Ds, 2
	octave 6
	note D_, 2
	octave 5
	note As, 2
	octave 6
	note C_, 2
	octave 5
	note Ds, 2
	octave 6
	note D_, 2
	octave 5
	note F_, 2
	octave 6
	note C_, 2
	octave 5
	note Ds, 2
	octave 6
	note Ds, 2
	octave 5
	note Gs, 2
	octave 6
	note D_, 2
	octave 5
	note D_, 2
	note B_, 2
	note G_, 2
	octave 6
	note C_, 2
	octave 5
	note Ds, 2
	octave 6
	note C_, 2
	octave 5
	note Gs, 2
	octave 6
	note D_, 2
	octave 5
	note G_, 2
	octave 6
	note D_, 2
	octave 5
	note B_, 2
	volume_envelope 7, 2
	octave 4
	note C_, 1
	note C_, 1
	note C_, 1
	note C_, 2
	note C_, 2
	note C_, 1
	note C_, 4
	volume_envelope 5, 6
	note C_, 4
	volume_envelope 8, 1
Music_CipherPeonBattle_Ch1_loop_1:
	sound_call Music_CipherPeonBattle_Ch1_branch_1
	sound_loop 2, Music_CipherPeonBattle_Ch1_loop_1
	octave 6
	note C_, 2
	octave 5
	note A_, 2
	octave 6
	note C_, 2
	octave 5
	note F_, 2
	octave 6
	note C_, 2
	octave 5
	note A_, 2
	octave 6
	note C_, 2
	octave 5
	note C_, 2
Music_CipherPeonBattle_Ch1_loop_2:
	sound_call Music_CipherPeonBattle_Ch1_branch_1
	sound_loop 3, Music_CipherPeonBattle_Ch1_loop_2
	octave 6
	note C_, 2
	octave 5
	note A_, 2
	octave 6
	note C_, 2
	octave 5
	note F_, 2
	octave 6
	note C_, 2
	octave 5
	note A_, 2
	octave 6
	note C_, 2
	octave 5
	note C_, 2
	sound_call Music_CipherPeonBattle_Ch1_branch_1
	sound_call Music_CipherPeonBattle_Ch1_branch_2
	rest 2
	note Gs, 2
	octave 6
	note Cs, 2
	octave 5
	note F_, 2
	octave 6
	note Cs, 2
	octave 5
	note Gs, 2
	octave 6
	note Cs, 1
	octave 5
	note Gs, 1
	note F_, 4
	note B_, 2
	octave 6
	note D_, 2
	octave 5
	note G_, 2
	octave 6
	note D_, 2
	octave 5
	note B_, 2
	octave 6
	note D_, 1
	octave 5
	note B_, 1
	note G_, 2
	sound_call Music_CipherPeonBattle_Ch1_branch_2
	volume_envelope 9, 2
	octave 2
	note F_, 1
	note G_, 1
	note Gs, 2
	note G_, 1
	note Gs, 1
	note As, 2
	note Gs, 1
	note As, 1
	octave 3
	note C_, 2
	octave 2
	note B_, 1
	octave 3
	note C_, 1
	note D_, 2
	volume_envelope 10, 2
	note C_, 1
	note D_, 1
	note Ds, 2
	note D_, 1
	note Ds, 1
	note F_, 2
	volume_envelope 11, 2
	note Ds, 1
	note F_, 1
	note G_, 2
	note Fs, 1
	note G_, 1
	note A_, 6
	duty_cycle 1
	volume_envelope 9, 7
	note A_, 8
	volume_envelope 9, 1
	note A_, 1
	note A_, 2
	note A_, 1
	volume_envelope 9, 0
	note A_, 4
	volume_envelope 9, 7
	note A_, 12
	octave 4
	volume_envelope 9, 0
	note C_, 4
	volume_envelope 9, 7
	note C_, 12
	octave 3
	volume_envelope 9, 0
	note B_, 4
	volume_envelope 9, 7
	note B_, 12
	sound_loop 0, Music_CipherPeonBattle_Ch1
	
Music_CipherPeonBattle_Ch1_branch_1:
	octave 6
	note C_, 2
	octave 5
	note G_, 2
	octave 6
	note C_, 2
	octave 5
	note Ds, 2
	octave 6
	note C_, 2
	octave 5
	note G_, 2
	octave 6
	note C_, 2
	octave 5
	note C_, 2
	sound_ret
	
Music_CipherPeonBattle_Ch1_branch_2:
	rest 2
	octave 6
	note C_, 2
	note Ds, 2
	octave 5
	note Gs, 2
	octave 6
	note Ds, 2
	note C_, 2
	note Ds, 1
	note C_, 1
	octave 5
	note Gs, 4
	note As, 2
	octave 6
	note D_, 2
	octave 5
	note G_, 2
	octave 6
	note D_, 2
	octave 5
	note As, 2
	octave 6
	note D_, 1
	octave 5
	note As, 1
	note G_, 2
	sound_ret
	
gbs_Music_CipherPeonBattle_Ch2:
	gbs_switch 1
Music_CipherPeonBattle_Ch2:
	duty_cycle 3
	vibrato 18, 1, 5
	pitch_offset 1
	note_type 8, 9, 4
	octave 3
	note Ds, 2
	note C_, 2
	octave 2
	note Gs, 2
	note As, 2
	octave 3
	note D_, 2
	note F_, 2
	note G_, 2
	note Ds, 2
	note C_, 2
	note D_, 2
	note F_, 2
	note As, 2
	note Gs, 2
	note Ds, 2
	note C_, 2
	note Ds, 2
	note Gs, 2
	octave 4
	note C_, 2
	octave 3
	note B_, 2
	note G_, 2
	note D_, 2
	note G_, 2
	note B_, 2
	octave 4
	note D_, 2
	note C_, 2
	octave 3
	note Gs, 2
	note Ds, 2
	note Gs, 2
	octave 4
	note C_, 2
	note Ds, 2
	note D_, 2
	octave 3
	note B_, 2
	note G_, 2
	note B_, 2
	octave 4
	note D_, 2
	note F_, 2
	note_type 12, 9, 2
	note G_, 1
	note G_, 1
	note G_, 1
	note G_, 2
	note G_, 2
	note G_, 1
	note G_, 4
	octave 3
	volume_envelope 5, 6
	note G_, 4
	volume_envelope 11, 4
	octave 3
	note C_, 4
	octave 2
	note G_, 2
	octave 3
	note C_, 4
	volume_envelope 11, 3
	note D_, 4
	note Ds, 4
	note F_, 4
	volume_envelope 11, 6
	note G_, 8
	note As, 2
	note A_, 1
	note F_, 1
	note C_, 10
	volume_envelope 11, 4
	note A_, 4
	volume_envelope 11, 7
	note G_, 16
	volume_envelope 11, 4
	octave 3
	note C_, 4
	octave 2
	note G_, 2
	octave 3
	note C_, 4
	volume_envelope 11, 3
	note D_, 4
	note Ds, 4
	note F_, 4
	volume_envelope 11, 6
	note G_, 8
	note As, 2
	note A_, 1
	note As, 1
	octave 4
	note C_, 10
	octave 3
	volume_envelope 11, 4
	note A_, 4
	volume_envelope 11, 7
	note G_, 12
	duty_cycle 0
	octave 4
	note C_, 2
	note D_, 2
	note Ds, 4
	note C_, 2
	octave 3
	note Gs, 4
	volume_envelope 11, 2
	note Ds, 2
	note Gs, 1
	octave 4
	note C_, 1
	note Ds, 2
	volume_envelope 11, 7
	note D_, 4
	octave 3
	note As, 2
	note G_, 4
	volume_envelope 11, 2
	note D_, 2
	note G_, 1
	note As, 1
	octave 4
	note D_, 2
	octave 3
	volume_envelope 11, 7
	note F_, 6
	note Gs, 6
	octave 4
	note Cs, 4
	note C_, 6
	octave 3
	note B_, 6
	octave 4
	note C_, 2
	note D_, 2
	note Ds, 4
	note C_, 2
	octave 3
	note Gs, 4
	volume_envelope 11, 2
	note Ds, 2
	note Gs, 1
	octave 4
	note C_, 1
	note Ds, 2
	volume_envelope 11, 7
	note D_, 4
	octave 3
	note As, 2
	note G_, 4
	volume_envelope 11, 2
	note D_, 2
	note G_, 1
	note As, 1
	octave 4
	note D_, 2
	octave 3
	note F_, 1
	note G_, 1
	note Gs, 2
	note G_, 1
	note Gs, 1
	note As, 2
	note Gs, 1
	note As, 1
	octave 4
	note C_, 2
	octave 3
	note B_, 1
	octave 4
	note C_, 1
	note D_, 2
	volume_envelope 12, 2
	note C_, 1
	note D_, 1
	note Ds, 2
	note D_, 1
	note Ds, 1
	note F_, 2
	volume_envelope 13, 2
	note Ds, 1
	note F_, 1
	note G_, 2
	note Fs, 1
	note G_, 1
	note A_, 2
	volume_envelope 5, 0
	duty_cycle 1
	octave 5
	vibrato 18, 2, 4
	note D_, 16
	note D_, 8
	volume_envelope 5, 7
	note D_, 8
	volume_envelope 5, 0
	note G_, 16
	note G_, 8
	volume_envelope 5, 7
	note G_, 8
	sound_loop 0, Music_CipherPeonBattle_Ch2
	
gbs_Music_CipherPeonBattle_Ch3:
	gbs_switch 2
Music_CipherPeonBattle_Ch3:
Music_CipherPeonBattle_Ch3_loop_1:
	note_type 6, 1, 9
	octave 2
	note C_, 3
	rest 1
	sound_loop 24, Music_CipherPeonBattle_Ch3_loop_1
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	note G_, 1
	rest 1
	note G_, 2
	rest 2
	note G_, 2
	rest 2
	note G_, 1
	rest 1
	note G_, 2
	rest 14
Music_CipherPeonBattle_Ch3_loop_2:
	note C_, 3
	rest 1
	sound_loop 16, Music_CipherPeonBattle_Ch3_loop_2
Music_CipherPeonBattle_Ch3_loop_3:
	note F_, 3
	rest 1
	sound_loop 8, Music_CipherPeonBattle_Ch3_loop_3
Music_CipherPeonBattle_Ch3_loop_4:
	note C_, 3
	rest 1
	sound_loop 24, Music_CipherPeonBattle_Ch3_loop_4
Music_CipherPeonBattle_Ch3_loop_5:
	note F_, 3
	rest 1
	sound_loop 8, Music_CipherPeonBattle_Ch3_loop_5
Music_CipherPeonBattle_Ch3_loop_6:
	note C_, 3
	rest 1
	sound_loop 8, Music_CipherPeonBattle_Ch3_loop_6
	note Gs, 11
	rest 1
	note Gs, 7
	rest 1
	note Gs, 3
	rest 1
	note Gs, 3
	rest 1
	note Gs, 3
	rest 1
	note G_, 11
	rest 1
	note G_, 7
	rest 1
	note G_, 3
	rest 1
	note G_, 3
	rest 1
	note G_, 3
	rest 1
	note Cs, 11
	rest 1
	note Cs, 7
	rest 1
	note Cs, 3
	rest 1
	note Cs, 3
	rest 1
	note Cs, 3
	rest 1
	note G_, 11
	rest 1
	note G_, 7
	rest 1
	note G_, 3
	rest 1
	note G_, 3
	rest 1
	note G_, 3
	rest 1
	note Gs, 11
	rest 1
	note Gs, 7
	rest 1
	note Gs, 3
	rest 1
	note Gs, 3
	rest 1
	note Gs, 3
	rest 1
	note G_, 11
	rest 1
	note G_, 7
	rest 1
	note G_, 3
	rest 1
	note G_, 3
	rest 1
	note G_, 3
	rest 1
	note Cs, 6
	rest 2
	note Ds, 6
	rest 2
	note F_, 6
	rest 2
	note G_, 6
	rest 2
	note Gs, 3
	rest 1
	note Gs, 3
	rest 1
	note As, 3
	rest 1
	note As, 3
	rest 1
	octave 3
	note C_, 3
	rest 1
	note C_, 3
	rest 1
	note D_, 3
	rest 1
	note D_, 4
	octave 1
	note D_, 8
	octave 2
	note D_, 15
	rest 1
	note D_, 1
	rest 1
	note D_, 2
	rest 2
	note D_, 1
	rest 1
	note D_, 16
	volume_envelope 2, 9
	note D_, 16
	octave 1
	volume_envelope 1, 9
	note G_, 16
	volume_envelope 2, 9
	note G_, 16
	volume_envelope 1, 9
	note G_, 16
	volume_envelope 2, 9
	note G_, 16
	sound_loop 0, Music_CipherPeonBattle_Ch3
	
gbs_Music_CipherPeonBattle_Ch4:
	gbs_switch 3
Music_CipherPeonBattle_Ch4:
	toggle_noise 3
	drum_speed 12
Music_CipherPeonBattle_Ch4_loop_main:
	drum_note 12, 4
	drum_note 3, 4
	drum_note 3, 4
	drum_note 3, 4
	drum_note 3, 4
	drum_note 3, 2
	drum_note 1, 2
	drum_note 4, 4
	drum_note 3, 2
	drum_note 1, 2
	drum_note 3, 4
	drum_note 3, 4
	drum_note 3, 4
	drum_note 3, 4
	drum_note 1, 1
	drum_note 1, 1
	drum_note 1, 1
	drum_note 1, 2
	drum_note 1, 2
	drum_note 4, 1	
	drum_note 1, 8
Music_CipherPeonBattle_Ch4_loop_1:
	sound_call Music_CipherPeonBattle_Ch4_branch_1
	sound_loop 7, Music_CipherPeonBattle_Ch4_loop_1
	drum_note 4, 2
	drum_note 1, 2
	drum_note 2, 2
	drum_note 1, 2
	drum_note 2, 1
	drum_note 2, 1
	drum_note 1, 2
	drum_note 1, 2
	drum_note 1, 2
Music_CipherPeonBattle_Ch4_loop_2:
	drum_note 3, 1
	drum_note 2, 1
	drum_note 3, 1
	drum_note 9, 1
	drum_note 1, 2
	drum_note 2, 1
	drum_note 2, 1
	drum_note 4, 1
	drum_note 1, 2
	drum_note 4, 1
	drum_note 1, 2
	drum_note 2, 1
	drum_note 2, 1
	sound_loop 6, Music_CipherPeonBattle_Ch4_loop_2
Music_CipherPeonBattle_Ch4_loop_3:
	drum_note 4, 2
	drum_note 1, 2
	sound_loop 8, Music_CipherPeonBattle_Ch4_loop_3
Music_CipherPeonBattle_Ch4_loop_4:
	sound_call Music_CipherPeonBattle_Ch4_branch_1
	sound_loop 4, Music_CipherPeonBattle_Ch4_loop_4
	sound_loop 0, Music_CipherPeonBattle_Ch4_loop_main
	
Music_CipherPeonBattle_Ch4_branch_1:
	drum_note 4, 2
	drum_note 4, 2
	drum_note 1, 2
	drum_note 2, 1
	drum_note 2, 1
	drum_note 4, 1
	drum_note 2, 1
	drum_note 4, 2
	drum_note 1, 2
	drum_note 2, 1
	drum_note 2, 1
	sound_ret
