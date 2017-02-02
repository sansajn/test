from pysnmp.hlapi import *

g = sendNotification(SnmpEngine(),
	CommunityData('public'),
	UdpTransportTarget(('demo.snmplabs.com', 162)),
	ContextData(),
	'inform',
	NotificationType(ObjectIdentity('IF-MIB', 'linkUp'), instanceIndex=(123,))
)

print(next(g))
