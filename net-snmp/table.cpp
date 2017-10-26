// http://www.net-snmp.org/dev/agent/data_set_8c-example.html
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/net-snmp-features.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

netsnmp_feature_require(table_set_multi_add_default_row)
netsnmp_feature_require(unregister_auto_data_table)
netsnmp_feature_require(delete_table_data_set)
netsnmp_feature_require(table_dataset)
netsnmp_feature_require(table_set_multi_add_default_row)
netsnmp_feature_require(table_dataset_unregister_auto_data_table)

static netsnmp_table_data_set * table_set;

void init_data_set()
{
	netsnmp_table_row * row;
	oid my_registration_oid[] = {1, 3, 6, 1, 4, 1, 8072, 2, 2, 1};
	table_set = netsnmp_create_table_data_set("netSnmpIETFWGTable");
	table_set->allow_creation = 1;  // create new rows via SNMP SET
	netsnmp_table_dataset_add_index(table_set, ASN_OCTET_STR);

	// define table columns
	netsnmp_table_set_multi_add_default_row(table_set,
		/*	column 2 = OCTET STRING
		writable = 1
		default value = NULL
		default value len = 0 */
		2, ASN_OCTET_STR, 1, nullptr, 0,
		// similar
		3, ASN_OCTET_STR, 1, nullptr, 0,
		0 /* done */);

	// register the table
	netsnmp_register_table_data_set(
		netsnmp_create_handler_registration(
			"netSnmpIETFWGTable", nullptr, my_registration_oid, OID_LENGTH(my_registration_oid),
			HANDLER_CAN_RWRITE
		), table_set, nullptr);

	// add some data
	row = netsnmp_create_table_data_row();

	// set the index to the IETF WG name "snmpv3"
	netsnmp_table_row_add_index(row, ASN_OCTET_STR, "snmpv3", strlen("snmpv3"));

	// set column 2 to be the WG chair name "Russ Mundy"
	netsnmp_set_row_column(row, 2, ASN_OCTET_STR, "Russ Mundy", strlen("Russ Mundy"));
	netsnmp_mark_row_column_writable(row, 3, 1);  // make writable via SET

	// set column 3 to be the WG chair name "David Harrington"
	netsnmp_set_row_column(row, 3, ASN_OCTET_STR, "David Harrington", strlen("David Harrington"));
	netsnmp_mark_row_column_writable(row, 3, 1);

	// add row to the table
	netsnmp_table_dataset_add_row(table_set, row);

	// allows the 'add_row' in snmpd.conf
	netsnmp_register_auto_data_table(table_set, nullptr);
}

void shutdown_data_set()
{
	netsnmp_unregister_auto_data_table(table_set, nullptr);
	netsnmp_delete_table_data_set(table_set);
	table_set = nullptr;
}


int main(int argc, char * argv[])
{
	return 0;
}
