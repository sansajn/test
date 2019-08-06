// router sample using ZMsg API (it can be used with dealer sample Dealer.kt)
import org.zeromq.ZContext
import org.zeromq.ZFrame
import org.zeromq.ZMQ
import org.zeromq.ZMsg

fun main(args: Array<String>) {
	val ctx = ZContext()

	val router = ctx.createSocket(ZMQ.ROUTER)
	router.bind("tcp://*:5557")
	println("waiting on tcp://*:5557 ...")

	while (true) {
		val msg: ZMsg = ZMsg.recvMsg(router)
		val identity: ZFrame = msg.pop()
		val d: ZFrame = msg.pop()
		msg.destroy()

		val asStr = d.toString()
		println("received $asStr")

		d.destroy()

		val answer = ZFrame(asStr.reversed())

		identity.send(router, ZFrame.MORE)
		answer.send(router, 0)

		identity.destroy()
	}

	println("done!")
}
