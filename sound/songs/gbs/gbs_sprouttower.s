	.include "asm/macros.inc"
	.section .rodata
	.global gbs_Music_SproutTower
	.align 2

gbs_Music_SproutTower:
	channel_count 4
	channel 1, Music_SproutTower_Ch1
	channel 2, Music_SproutTower_Ch2
	channel 3, Music_SproutTower_Ch3
	channel 4, Music_SproutTower_Ch4

gbs_Music_SproutTower_Ch1:
	gbs_switch 0
Music_SproutTower_Ch1:
	tempo 176
	volume 7, 7
	duty_cycle 3
	pitch_offset 2
	vibrato 8, 4, 5
	stereo_panning FALSE, TRUE
	note_type 12, 10, 3
	rest 16
	rest 16
Music_SproutTower_branch_f6994:
	rest 6
	octave 3
	note F_, 2
	note Fs, 4
	note Fs, 4
	note F_, 4
	note C_, 4
	note Cs, 4
	note Ds, 4
	note Ds, 4
	note Ds, 4
	note Fs, 4
	note Fs, 4
	note F_, 2
	note Fs, 2
	note Gs, 2
	note Fs, 2
	note F_, 4
	note Fs, 2
	note Ds, 10
	note Fs, 2
	note F_, 2
	note Ds, 2
	note Cs, 2
	volume_envelope 10, 5
	note C_, 8
	volume_envelope 10, 3
	note Cs, 2
	note C_, 2
	octave 2
	note As, 2
	octave 3
	note Cs, 2
	volume_envelope 10, 7
	note C_, 16
	volume_envelope 10, 3
	note F_, 4
	note F_, 4
	note Fs, 4
	note Fs, 4
	note F_, 4
	note F_, 4
	note Ds, 4
	note Ds, 4
	sound_jump Music_SproutTower_branch_f6994

gbs_Music_SproutTower_Ch2:
	gbs_switch 1
Music_SproutTower_Ch2:
	duty_cycle 3
	pitch_offset 1
	vibrato 8, 4, 8
	note_type 12, 11, 2
	octave 3
	note F_, 4
	note F_, 4
	note Fs, 4
	note Fs, 4
	note F_, 4
	note F_, 4
	note Ds, 4
	note Ds, 4
Music_SproutTower_branch_f69dc:
	volume_envelope 11, 5
	octave 4
	note C_, 4
	octave 3
	note As, 4
	octave 4
	note Cs, 4
	note C_, 2
	octave 3
	note As, 2
	octave 4
	note C_, 2
	octave 3
	note As, 2
	note Fs, 4
	volume_envelope 11, 7
	note F_, 8
	volume_envelope 11, 5
	note Fs, 4
	note As, 4
	octave 4
	note C_, 2
	note Cs, 2
	note C_, 2
	octave 3
	note As, 2
	octave 4
	note C_, 2
	note Cs, 2
	note Ds, 2
	note Cs, 2
	note C_, 8
	volume_envelope 11, 5
	note Cs, 2
	note C_, 2
	octave 3
	note As, 2
	note Fs, 2
	volume_envelope 11, 7
	note As, 8
	volume_envelope 11, 5
	octave 4
	note C_, 2
	octave 3
	note As, 2
	note Fs, 2
	note Ds, 2
	volume_envelope 11, 7
	note F_, 8
	volume_envelope 11, 3
	note F_, 4
	note F_, 4
	note Fs, 4
	note As, 2
	note F_, 1
	note As, 1
	volume_envelope 11, 7
	octave 4
	note C_, 16
	rest 16
	sound_jump Music_SproutTower_branch_f69dc

gbs_Music_SproutTower_Ch3:
	gbs_switch 2
Music_SproutTower_Ch3:
	vibrato 20, 14, 8
	note_type 12, 1, 4
	octave 3
	note F_, 2
	rest 2
	note C_, 2
	note F_, 2
	note Fs, 2
	note Ds, 2
	rest 2
	note Fs, 2
	note F_, 2
	rest 2
	note C_, 2
	note F_, 2
	note Fs, 2
	note Ds, 2
	rest 2
	note Fs, 2
	stereo_panning TRUE, FALSE
Music_SproutTower_branch_f6a3e:
	rest 4
	note C_, 2
	note F_, 2
	note Fs, 2
	note Ds, 2
	rest 2
	note Fs, 2
	note F_, 2
	rest 2
	note C_, 2
	note F_, 2
	note Fs, 2
	note Ds, 2
	rest 2
	note F_, 2
	note Ds, 2
	rest 2
	octave 2
	note As, 2
	octave 3
	note Ds, 2
	note Fs, 2
	note Ds, 2
	rest 2
	note Fs, 2
	note F_, 2
	rest 2
	note C_, 2
	note F_, 2
	note Fs, 2
	note Ds, 2
	rest 2
	note Fs, 2
	note F_, 8
	note Fs, 4
	note As, 4
	note Ds, 8
	note F_, 4
	note Fs, 4
	note F_, 16
	note F_, 2
	rest 2
	note C_, 2
	note F_, 2
	note Fs, 2
	note Ds, 2
	rest 2
	note F_, 2
	note F_, 2
	rest 2
	note C_, 2
	note F_, 2
	note Fs, 2
	note Ds, 2
	rest 2
	note F_, 2
	sound_jump Music_SproutTower_branch_f6a3e

gbs_Music_SproutTower_Ch4:
	gbs_switch 3
Music_SproutTower_Ch4:
	toggle_noise 0
	drum_speed 12
	rest 4
Music_SproutTower_branch_f6a7f:
	drum_note 7, 4
	stereo_panning TRUE, FALSE
	drum_note 4, 4
	drum_note 7, 2
	stereo_panning FALSE, TRUE
	drum_note 4, 2
	stereo_panning TRUE, TRUE
	drum_note 7, 4
	drum_note 7, 4
	stereo_panning FALSE, TRUE
	drum_note 4, 4
	drum_note 7, 2
	stereo_panning TRUE, FALSE
	drum_note 4, 2
	stereo_panning TRUE, TRUE
	drum_note 7, 4
	sound_jump Music_SproutTower_branch_f6a7f
