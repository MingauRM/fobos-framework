local task = {}

export type task = {}
export type tasklib = {
	create: (fn: (...any) -> ...any) -> task,

	pause: (task: task) -> (),
	resume: (task: task) -> (),
	cancel: (task: task) -> (),

	wait: (seconds: number?) -> (),
	delay: (task: task, seconds: number) -> (),

	parallel: (task: task) -> (),
	serial: (task: task) -> (),
}

return task :: tasklib
