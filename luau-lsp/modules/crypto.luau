local crypto = {}

export type cryptolib = {
	sha256: (toHash: string) -> string,
	sha512: (toHash: string) -> string,

	encryptaes: (toEncrypt: string, key: string) -> string,
	decryptaes: (encrypted: string, key: string) -> string,
}

return crypto :: cryptolib
