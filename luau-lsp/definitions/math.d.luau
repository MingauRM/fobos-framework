declare math: typeof(math) & {
    read smoothstep: (edge0: number, edge1: number, t: number) -> number,
    read frac: (x: number) -> number,
    read trunc: (x: number) -> number,
}