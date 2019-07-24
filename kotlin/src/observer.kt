// animal choir (observer pattern) sample based on map

class Bat {
	fun screech() {
		println("Eeeeeeee")
	}
}

class Turkey {
	fun gobble() {
		println("Gob-gob")
	}
}

class Dog {
	fun bark() {
		println("Woof")
	}

	fun howl() {
		println("Auuuuu")
	}
}

typealias Times = Int

class Cat {

	fun conduct(n: Times) {
		for (p in _participants.values)
			for (i in 1..n)
				p()
	}

	fun joinChoir(whatToCall: () -> Unit) {
		_participants.put(whatToCall, whatToCall)
	}

	fun leaveChoir(whatNotToCall: () -> Unit) {
		_participants.remove(whatNotToCall)
	}

	private val _participants = mutableMapOf<() -> Unit, () -> Unit>()
}

fun main(args: Array<String>) {
	val catTheConductor = Cat()
	val bat = Bat()
	val dog = Dog()
	val turkey = Turkey()
	
	catTheConductor.joinChoir(bat::screech)
	catTheConductor.joinChoir(dog::howl)
	catTheConductor.joinChoir(dog::bark)
	catTheConductor.joinChoir(turkey::gobble)
	catTheConductor.conduct(1)

	println("\nafter bat leaved\n")

	catTheConductor.leaveChoir(bat::screech)
	catTheConductor.conduct(1)

	println("\ndone!")
}
