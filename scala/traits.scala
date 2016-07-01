// http://docs.scala-lang.org/tutorials/tour/traits.html

trait Similarity {
	def isSimilar(x: Any): Boolean
	def isNotSimilar(x: Any): Boolean = !isSimilar(x)
}


class Point(var x: Int, var y: Int) extends Similarity {
	def isSimilar(obj: Any) = {
		obj.isInstanceOf[Point] && obj.asInstanceOf[Point].x == x
	}
}

object TraitsTest {
	def main(args: Array[String]) = {
		val p1 = new Point(2,3)
		val p2 = new Point(2,4)
		val p3 = new Point(3,3)
		println(p1.isNotSimilar(p2))  // false
		println(p1.isNotSimilar(p3))  // true
		println(p1.isNotSimilar(2))   // true
	}
}

TraitsTest.main(args)
