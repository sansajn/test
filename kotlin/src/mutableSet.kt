// set sample

fun main(args: Array<String>) {
	val set = mutableSetOf(1, 2, 5, 10, 11)
	for (s in set)
		println("$s, ")

	if (set.contains(5))
		println("there is a 5 in a set")

	println("done!")
}