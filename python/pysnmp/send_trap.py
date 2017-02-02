# snmptrap -v2c -c public 127.0.0.1:162 123 1.3.6.1.6.3.1.1.5.1 1.3.6.1.2.1.1.5.0 s test

from pysnmp.hlapi import *

g = sendNotification(
	SnmpEngine(),
	CommunityData('public'),
	UdpTransportTarget(('127.0.0.1', 162)),
	ContextData(),
	'trap',
	NotificationType(ObjectIdentity('1.3.6.1.6.3.1.1.5.1'), instanceIndex=(123,))
)

print(next(g))
