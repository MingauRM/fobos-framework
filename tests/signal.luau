-- NOT DONE YET!! LOOK INTO `src/mod/lsignallib.cpp` AND FIX THE MEMORY LEAK PROBLEMS!
-- Possibly fixed.

local s = signal.create()
local conn = signal.connect(s, function(health: number)
	print(health)
end)

signal.fire(s, 100)

signal.disconnect(conn)
signal.destroy(s)

s = nil
conn = nil
