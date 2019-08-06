// java.util.observable based observer implementation

import java.util.*

class Burger(val name: String)

class Bob : Observable() {

	fun cookBurger(name: String) {
		val burger = Burger(name)
		setChanged()
		notifyObservers(burger)
	}

	val name = "Bob"
}

class Tina : Observer {

	override fun update(o: Observable?, arg: Any?) {
		when (o) {
			is Bob -> {
				if (arg is Burger)
					println("$name is serving the ${arg.name} cooked by ${o.name}")
			}
			else -> println(o?.javaClass.toString())
		}
	}

	val name = "Tina"
}

fun main(args: Array<String>) {
	val bob = Bob()
	bob.addObserver(Tina())
	bob.cookBurger("It takes Bun to Know Bun Burger")
}
