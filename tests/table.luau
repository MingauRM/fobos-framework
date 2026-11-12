local tbl = { 1, 4, 5, 2, 7, 12, 8 }
table.sort(tbl, function(a, b): boolean
	return a < b
end)

for key, value in ipairs(tbl) do
	print(`[{key}] = {value}`)
end
