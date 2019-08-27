// ukazka pouzitia sortedWith a custom komparatora

data class MyDate(val year: Int, val month: Int, val day: Int)

class CompareObjects {
	companion object : Comparator<MyDate> {
		override fun compare(a: MyDate, b: MyDate): Int = when {
			a.year != b.year -> a.year - b.year
			a.month != b.month -> a.month - b.month
			else -> a.day - b.day
		}
	}
}

fun dateSortTest() {
	val dates = listOf(
			MyDate(2010, 4, 3),
			MyDate(2006, 5, 16),
			MyDate(2007, 6, 29))

	// custom comparator
	dates.sortedWith(CompareObjects).forEach { println(it) }

	println("---")

	// compare-by
	dates.sortedWith(compareBy({ it.year }, { it.month }, { it.day })).forEach { println(it) }
}

fun comparatorTest() {
	val names = listOf("kim", "julia", "jim", "hala")

	names.sortedWith(Comparator<String>{ a, b ->
		when {
			a > b -> 1
			a < b -> -1
			else -> 0
		}
	}).forEach { println(it) }
}

fun main(args: Array<String>) {
	dateSortTest()
	println("---")
	comparatorTest()
}
