// directory sort sample

fun isDirectory(path: String): Boolean {
	return !path.contains('.')
}

fun lessCompare(a: String, b: String): Int = when {
	a < b -> -1
	a > b -> 1
	else -> 0
}

fun main(args: Array<String>) {
	val files = listOf("test.txt", "build", "changelog.txt", "abcd.txt", "app")

	files.sortedWith(compareBy( {it} )).forEach {println(it)}

	println("---")

	files.sortedWith(Comparator<String>{ a, b ->
		val aDir = isDirectory(a)
		val bDir = isDirectory(b)

		when {
			aDir && bDir -> lessCompare(a, b)
			aDir -> -1
			bDir -> 1
			else -> lessCompare(a, b)
		}

	}).forEach {println(it)}
}
