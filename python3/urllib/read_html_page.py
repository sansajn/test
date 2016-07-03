# v pythone3 precita stranku
import urllib.request

page_req = urllib.request.urlopen('https://www.google.com')
print(page_req.read())
