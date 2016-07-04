import scala.collection.mutable.Map

val treasure_map = Map[Int, String]()
treasure_map += (1 -> "Go to island.")
treasure_map += (2 -> "Find big X on ground.")
treasure_map += (3 -> "Dig.")
println(treasure_map(2))  // "Find big X on ground."
