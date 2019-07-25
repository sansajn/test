// any samples

fun main(srgs: Array<String>) {
	val n: Any = 42

	if (n is Int)
		println("we have Int($n) there")

	if (n is Double)
		println("we have Double($n) there")

	val s: Any = "Hello!"

	if (s is String)
		println("we have String('$s') there")
}
