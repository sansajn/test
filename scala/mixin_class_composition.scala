/* http://docs.scala-lang.org/tutorials/tour/mixin-class-composition.html
spusti ako $ scala mixin_class_composition.scala hello */

abstract class AbsIterator {
	type T
	def hasNext: Boolean
	def next: T
}

trait RichIterator extends AbsIterator {
	def foreach(f: T => Unit) = while (hasNext) f(next)
}

class StringIterator(s: String) extends AbsIterator {
	type T = Char
	private var i = 0
	def hasNext = i < s.length()
	def next = {val ch = s charAt i; i += 1; ch}
}

object StringIteratorTest {
	def main(args: Array[String]) {
		class Iter extends StringIterator(args(0)) with RichIterator
		val iter = new Iter
		iter foreach println
	}
}

StringIteratorTest.main(args)
