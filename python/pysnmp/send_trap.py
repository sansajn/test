# $ snmptrap -v1 -c public demo.snmplabs.com 1.3.6.1.4.1.20408.4.1.1.2 127.0.0.1 6 432 12345 1.3.6.1.2.1.1.1.0 s 'my system'
# http://pysnmp.sourceforge.net/examples/hlapi/asyncore/sync/agent/ntforg/snmp-v1-trap-variants.html
from pysnmp.hlapi import *

errorIndication, errorStatus, errorIndex, varBinds = next(
    sendNotification(
        SnmpEngine(),
        CommunityData('public', mpModel=0),
        UdpTransportTarget(('127.0.0.1', 162)),
        ContextData(),
        'trap',
        NotificationType(
            ObjectIdentity('1.3.6.1.4.1.20408.4.1.1.2.0.432'),
        ).addVarBinds(
            #('1.3.6.1.2.1.1.3.0', 12345),
            #('1.3.6.1.6.3.18.1.3.0', '127.0.0.1'),
            #('1.3.6.1.6.3.1.1.4.3.0', '1.3.6.1.4.1.20408.4.1.1.2'),
            ('1.3.6.1.2.1.1.1.0', OctetString('my system')),
            ('1.3.6.1.2.1.1.1.1', OctetString('my custom message'))
        )
    )
)

if errorIndication:
    print(errorIndication)
