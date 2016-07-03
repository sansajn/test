# posle post request a ziska vysledok
import urllib.parse
import urllib.request

url = 'http://pythonprogramming.net'
values = {'s':'basic', 'submit':'search'}
data = urllib.parse.urlencode(values)
data = data.encode('utf-8')
req = urllib.request.Request(url, data)
resp = urllib.request.urlopen(req)
resp_data = resp.read()

print(resp_data)
