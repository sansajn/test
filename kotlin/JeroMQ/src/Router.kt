// router sample (it can be used with dealer sample Dealer.kt)
import org.zeromq.ZContext
import org.zeromq.ZMQ

fun main(args: Array<String>) {
	val ctx = ZContext()

	val router = ctx.createSocket(ZMQ.ROUTER)
	router.bind("tcp://*:5557")
	println("waiting on tcp://*:5557 ...")

	while (true) {
		val identity: ByteArray = router.recv()
		val d = router.recvStr()
		println("received: $d")

		router.sendMore(identity)
		router.send(d.reversed())
	}

	println("done!")
}
