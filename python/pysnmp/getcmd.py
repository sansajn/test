# ekvivalent 
#	$ snmpget -v2c -c public demo.snmplabs.com SNMPv2-MIB::sysDescr.0' 

from pysnmp.hlapi import *

g = getCmd(
	SnmpEngine(),
	CommunityData('public'),
	UdpTransportTarget(('demo.snmplabs.com', 161)),
	ContextData(),
	ObjectType(ObjectIdentity('SNMPv2-MIB', 'sysDescr', 0))
)

print(next(g)) 
