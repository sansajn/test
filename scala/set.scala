var jet_set = Set("Boeing", "Airbus")  // immutable set used
jet_set += "Lear"  // a new Set("Boeing", "Airbus", "Lear") created
println(jet_set.contains("Cessna"))
