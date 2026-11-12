local random = {}

export type rng = {}
export type randomlib = {
	create: (seed: number) -> rng,

	nextnumber: (rng: rng) -> number,
	nextinteger: (rng: rng) -> number,
	nextunitvector: (rng: rng) -> vector,
}

return random :: randomlib
