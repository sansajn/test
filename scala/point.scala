/* http://docs.scala-lang.org/tutorials/tour/classes.html 
spusti ako $ scala point.scala */

class Point(var x: Int, var y: Int) {
	def move(dx: Int, dy: Int) {
		x += dx
		y += dy
	}
	
	override def toString(): String = "(" + x + ", " + y + ")";
}

object Classes {
	def main(args: Array[String]) {
		val pt = new Point(1, 2)
		println(pt)
		pt.move(10, 10)
		println(pt)
	}
}

Classes.main(args)
