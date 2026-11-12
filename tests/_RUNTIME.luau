print(_RUNTIME.name)
print(_RUNTIME.version.display)

if _RUNTIME.name == "fobos" then
	print("The current runtime is Fobos!")
elseif _RUNTIME.name == "luwow" then
	print("The current runtime is Luwow!")
end
