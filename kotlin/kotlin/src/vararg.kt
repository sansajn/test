
fun foo(vararg opts: Any) {
	for (opt in opts) {
		if (opt is Int)
			println("we have Int($opt) there")
		else if (opt is Double)
			println("we have Double($opt) there")
		else if (opt is String)
			println("we have String('$opt') there")
		else
			println("$opt")
	}
}

fun main(args: Array<String>) {
	foo(1, 10.1, "Hello!")
}