
/* store patches in a filesystem like structure
\note for large number of patches list based implementation can be slow */
class ViewFSNode(val label: String) {

	fun addPath(path: String) {

		var parent: ViewFSNode = this

		for (pathElem in path.split("/")) {
			if (pathElem.isEmpty())
				continue  // ignore first ""

			val node = parent.children.find { it.label == pathElem }
			if (node == null) {
				val newNode = ViewFSNode(pathElem)
				parent.children.add(newNode)
				parent = newNode
			}
			else
				parent = node
		}
	}

	fun getPath(path: String): ViewFSNode? {

		var parent: ViewFSNode = this

		for (pathElem in path.split("/")) {
			if (pathElem.isEmpty())
				continue  // ignore first ""

			val node = parent.children.find { it.label == pathElem }
			if (node != null)
				parent = node
			else
				return null  // not found
		}

		return parent
	}

	fun list(): List<String> {
		val result = mutableListOf<String>()
		for (node in children)
			result.add(node.label)
		return result
	}

	val children: MutableList<ViewFSNode> = mutableListOf()
}


fun main(args: Array<String>) {

	val files = listOf(
		"/home/hlavatovic/Music/Position Music - The State [Epic Music Album - Adam Peters - Powerful Hybrid Sci Fi]-XeUxDDPCbBA.opus",
		"/home/hlavatovic/Music/TomorrowWorld 2014 _ The Chainsmokers-aDo8adoNIww.opus",
		"/home/hlavatovic/Music/Tiësto - Live @ Electric Daisy Carnival Las Vegas 2016-t7cnigvw98Q.opus",
		"/home/hlavatovic/Music/breakfast in the hut/Steve Paradise - Zultan (Pablo Fierro Remix)-Jnl7PbP-9JE.opus",
		"/home/hlavatovic/Music/breakfast in the hut/Stavroz - The Finishing (original mix)-iAhd7z4TM9I.opus",
		"/home/hlavatovic/Music/breakfast in the hut/Quantic - Time Is The Enemy-nvUeo5sagkA.opus",
		"/home/hlavatovic/Music/breakfast in the hut/Route 94 - My Love (Official Video) ft. Jess Glynne-BS46C2z5lVE.opus","/home/hlavatovic/Music/2018/Deborah De Luca @ Alltimeclubbing Bucharest (BE-AT.TV)-TwON9KsqyDk.opus",
		"/home/hlavatovic/Music/2018/100 Days until Tomorrowland _ Paul Kalkbrenner - LIVE-wMFy7vgfBFc.opus",
		"/home/hlavatovic/Music/2018/Solomun DJ set @ Diynamic Outdoor - Off Week Barcelona 2018 (BE-AT.TV)-No6pJpR6gwk.opus",
		"/home/hlavatovic/Music/2018/ANNA - Rave On Snow 2017 (BE-AT.TV)-7LNsnWbLcFU.opus",
		"/home/hlavatovic/Music/2017/MaRLo @ Tomorrowland 2017.opus",
		"/home/hlavatovic/Music/2017/David Guetta @ Tomorrowland 2017.opus",
		"/home/hlavatovic/Music/2017/Camo & Krooked @ Tomorrowland 2017.opus",
		"/home/hlavatovic/Music/David Guetta live Tomorrowland 2017-sSX9YlNuUpc.opus",
		"/home/hlavatovic/Music/Armin Van Buuren @ Ultra Europe 2016-7CZDOT8aW4Y.mp4",
		"/home/hlavatovic/Music/Tomorrowland 2017 Special Madness Mix Official Warm Up-qVpz5Dfo5mw.opus")


	val root = ViewFSNode("/")
	for (file in files)
		root.addPath(file)

	val music = root.getPath("/home/hlavatovic/Music")
	if (music != null) {
		println("music:")
		val musicFiles = music.list()
		musicFiles.forEach { println("  $it") }
	}

	println("done!")
}