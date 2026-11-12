local file = {}

export type filelib = {
	root: () -> string,
	appdata: () -> string,

	exists: (path: string) -> boolean,
	isfile: (path: string) -> boolean,
	isdir: (path: string) -> boolean,

	parentof: (path: string) -> string,
	childrenof: (dir: string) -> { string },
	descendantsof: (dir: string) -> { string },

	writeto: (file: string, data: string | buffer) -> (),
	readfrom: (file: string) -> buffer,

	copy: (from: string, to: string) -> (),
	move: (from: string, to: string) -> (),
	remove: (path: string) -> (),
}

return file :: filelib
