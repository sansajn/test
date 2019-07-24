
fun foo(f: () -> Unit) {
	f()
}

fun foo(f: (Int) -> Unit, n: Int) {
	f(n)
}

fun goo() {
	println("goo()")
}

fun goo(n: Int) {
	println("goo($n)")
}

class Goo {
	fun g() {
		println("Goo::g()")
	}

	fun h(n: Int) {
		println("Goo::h($n)")
	}
}

class Foo(private val f: (Int) -> Unit) {

	fun delegate(n: Int) {
		f(n)
	}
}

class Producer(private val consume: (MutableList<String>) -> Unit) {

	fun produce() {
		consume(mutableListOf("one", "two", "three"))
	}
}

class Consumer {

	fun consume(content: MutableList<String>) {
		for (s in content)
			print("$s, ")
	}
}


fun main(args: Array<String>) {
	foo(::goo)  // goo()
	foo(::goo, 10)  // goo(10)

	val g = Goo()
	foo(g::g)  // Goo::g()
	foo(g::h, 11)  // Goo::h(11)

	val f = Foo(g::h)
	f.delegate(12)  // Goo::h(12)

	val consumer = Consumer()
	val producer = Producer(consumer::consume)
	producer.produce()   // one, two, three, 

}
