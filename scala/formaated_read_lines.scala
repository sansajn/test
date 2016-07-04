import scala.io.Source

def width_of_length(s: String) = s.length.toString.length

if (args.length > 0) {
	val lines = Source.fromFile(args(0)).getLines.toList
	
	val longest_line = lines.reduceLeft(
		(a,b) => if (a.length > b.length) a else b
	)
	val max_width = width_of_length(longest_line)
	
	for (line <- lines) {
		val num_spaces = max_width - width_of_length(line)
		val padding = " " * num_spaces
		println(padding + line.length + " | " + line)
	}
}
else
	Console.err.println("Please enter filename")
	
