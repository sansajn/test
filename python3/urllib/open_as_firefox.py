# stiahne stranku ako firefox
import urllib.request

try:
	url = 'https://www.google.com/search?q=test'
	
	headers = {'User-Agent':'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.27 Safari/537.17'}
	req = urllib.request.Request(url, headers=headers)
	resp = urllib.request.urlopen(req)
	resp_data = resp.read()  # akeho typu su resp_data ?
	
	fout = open('dump_as_firefox.txt', 'w')
	fout.write(str(resp_data))
	fout.close()
	
	print('done')
	
except Exception as e:
	print(str(e))
