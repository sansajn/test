val oneTwoTree = List(1,2,3)

val oneTwo = List(1,2)
val threeFour = List(3,4)
val oneTwoThreeFour = oneTwo ::: threeFour
println("" + oneTwo + " and " + threeFour + " were not mutated.")
println("Thus, " + oneTwoThreeFour + " is a new list.")

val twoThree = List(2,3)
val oneTwoThree2 = 1 :: twoThree
println(oneTwoThree2)  // List(1, 2, 3)

val oneTwoThree3 = 1 :: 2 :: 3 :: Nil
println(oneTwoThree3)  // List(1, 2, 3)
