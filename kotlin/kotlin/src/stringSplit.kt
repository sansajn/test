
fun main(args: Array<String>) {

	val s = "/home/hlavatovic/Music/2018/ANNA - Rave On Snow 2017 (BE-AT.TV)-7LNsnWbLcFU.opus"

	println(s)
	for (e in s.split("/"))
		println(e)

	println(s)
	val pathElems: List<String> = s.split("/")
	for (n in 1 until pathElems.size)
		println(pathElems[n])

	println("done!")
}
