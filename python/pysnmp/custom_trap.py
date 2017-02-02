# posiela trap s vlastnimy datami
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
            ('1.3.6.1.2.1.1.1.0', OctetString('Tereza Lisbon')),
            ('1.3.6.1.2.1.1.1.1', OctetString('Patrik Jane'))
        )
    )
)

if errorIndication:
    print(errorIndication)
