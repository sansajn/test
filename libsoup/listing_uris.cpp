

// list uris
	GSList * uris;
	for (uris = soup_server_get_uris(server); uris; uris = uris->next)
	{
		char * str = soup_uri_to_string((SoupURI *)uris->data, FALSE);
		cout << "listenning on " << str << "\n";
		g_free(str);
		soup_uri_free((SoupURI *)uris->data);
	}
	g_slist_free(uris);
