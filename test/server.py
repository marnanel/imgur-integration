#!/usr/bin/python
# TODO:
#  - Parse incoming content on POST
#  - Checksum it and compare it against lsm.jpg
#  - Return good and bad error messages
#  - Make DBus service check environment
#  - Write test harness

import random
import SimpleHTTPServer
import SocketServer
import getopt
import os
import sys
import cgi
import hashlib

def usage():
	print "-h - show this help"
	print "-b - launch a browser"
	print "other arguments: run this command with environment set up"
	sys.exit(1)

child_pid = 0
port = 1024+ int(64511*random.random())
launch_browser = 0
launch_shell = 0

filename = 'lsm.jpg'
if not os.path.exists(filename):
	print "We need the image %s to be available." % (filename)
	sys.exit(1)

picture = file(filename).read()

picture_md5 = hashlib.md5()
picture_md5.update(picture)

options = getopt.getopt(sys.argv[1:], 'bhs:')

for option in options[0]:
	if option[0]=='-b':
		launch_browser = 1
	elif option[0]=='-h':
		usage()

if not options[1]:
	usage()

class FakeImgur(SimpleHTTPServer.SimpleHTTPRequestHandler):
	def do_GET(self):
		if self.path == '/abc123.jpg':
			self.show_picture()
		elif self.path == '/':
			self.show_main()
		else:
			self.show_404()

	def do_POST(self):
		if self.path == '/upload.xml':
			self.receive_image()
		else:
			self.show_404()

	def show_picture(self):
		self.wfile.write('HTTP/1.1 200 Found\n')
		self.wfile.write('Content-Type: image/jpeg\n')
		self.wfile.write('Content-Length: %d\n' % (len(picture)))
		self.wfile.write('\n')
		self.wfile.write(picture)

	def show_main(self):
		page = '<html><head><title>imgur integration</title></head>\n'
		page += '<body><h1>imgur integration server</h1>\n'
		page += '<p>Upload an image:</p>\n'
		page += '<form method="POST" enctype="multipart/form-data" action="/upload.xml">\n'
		page += '<input type="file" name="image"/>\n'
		page += '<input type="hidden" name="key" value="dummy" />\n'
		page += '<input type="submit" />\n'
		page += '</form>\n'
		page += '<img src="/abc123.jpg" width="500" height="438" alt="" />'
		page += '</body></html>'

		self.wfile.write('HTTP/1.1 200 Found\n')
		self.wfile.write('Content-Type: text/html\n')
		self.wfile.write('Content-Length: %d\n' % (len(page)))
		self.wfile.write('\n')
		self.wfile.write(page)

	def receive_image(self):
		
		error = None
		length = int(self.headers.getheader('Content-Length'))

		ctype, pdict = cgi.parse_header(self.headers.getheader('Content-Type'))

		fs = cgi.parse_multipart(self.rfile, pdict)

		fsk = fs.keys()

		if len(fsk)!=2 or not 'image' in fsk or not 'key' in fsk:
			error = 'Invalid request'
		else:
			m = hashlib.md5()
			m.update(fs['image'][0])
			if m.hexdigest()!=picture_md5.hexdigest():
				error = 'Not the picture I was expecting'

		outertag = 'rsp'
		results = {}

		if error is None:
			url = 'http://localhost:%d/abc123.jpg' % (port,)
			results['image_hash'] = 'abc123'
			results['delete_hash'] = 'dummy'
			results['original_image'] = url
			results['large_thumbnail'] = url
			results['small_thumbnail'] = url
			results['imgur_page'] = url
			results['delete_page'] = 'http://example.com'
		else:
			outertag = 'error'
			results = {'error_code':177, 'error_msg': error}
			
		page = '<%s>\n' % (outertag,)
		for line in sorted(results.keys()):
			page += '  <%s>%s</%s>\n' % (line, results[line], line)
		page += '</%s>\n' % (outertag,)
		
		self.wfile.write('HTTP/1.1 200 Found\n')
		self.wfile.write('Content-Type: text/plain\n')
		self.wfile.write('Content-Length: %d\n' % (len(page)))
		self.wfile.write('\n')
		self.wfile.write(page)


	def show_404(self):
		page = '<html><head><title>Not found</title>\n'
		page += '</head><body><h1>Not found</h1>\n'
		page += '<p>This is a test server for imgur.com '
		page += 'integration into Eye of GNOME.  More '
		page += 'information may be found '
		page += '<a href="http://thomasthurman.org">at\n'
		page += 'thomasthurman.org</a>.</p>'
		page += '<img src="/abc123.jpg" width="500" height="438" alt="" />'
		page += '</body></html>'
		page += '\n' * 3000 # idiot browsers that hide 404s

		self.wfile.write('HTTP/1.1 404 Not found\n')
		self.wfile.write('Content-Type: text/html\n')
		self.wfile.write('Content-Length: %d\n' % (len(page)))
		self.wfile.write('\n')
		self.wfile.write(page)


httpd = SocketServer.TCPServer(('', port), FakeImgur)

if launch_browser:
	os.system("xdg-open http://localhost:%d/" % (port,))

env = os.environ
env['IMGUR_URL'] = 'http://localhost:%d/upload.xml' % (port,)

child_pid = os.spawnvpe(os.P_NOWAIT, options[1][0], options[1], env)

httpd.serve_forever()

