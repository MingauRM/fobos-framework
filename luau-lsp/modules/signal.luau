local signal = {}

export type scriptsignal = {}
export type scriptconnection = {}
export type signallib = {
	create: () -> scriptsignal,

	connect: (s: scriptsignal, fn: (...any) -> ...any) -> scriptconnection,
	once: (s: scriptsignal, fn: (...any) -> ...any) -> scriptconnection,
	await: (s: scriptsignal) -> ...any,

	disconnect: (c: scriptconnection) -> (),
	destroy: (s: scriptsignal) -> (),

	fire: (s: scriptsignal, ...any) -> (),
}

return signal :: signallib
