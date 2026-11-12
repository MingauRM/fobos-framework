local audio = {}

export type audiotrack = {}
export type audiolib = {
	load: (audiobuf: buffer) -> audiotrack,
	playtrack: (track: audiotrack) -> (),
	stoptrack: (track: audiotrack) -> (),

	pause: (track: audiotrack) -> (),
	resume: (track: audiotrack) -> (),

	setvolume: (track: audiotrack, volume: number) -> (),
	setpitch: (track: audiotrack, pitch: number) -> (),
	setspeed: (track: audiotrack, speed: number) -> (),
}

return audio :: audiolib
