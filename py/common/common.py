import subprocess

def output(cmd):
	p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
	return p.communicate()[0].strip()

def fwrite(string, path):
	fil = open(path, "w")
	fil.write(string)
	fil.close()

def judge(aout, bout, silence=False):
	if not silence:
		print aout
		print bout
	if aout == bout:
		print "Ok"
		return True
	print "Wrong Answer"
	return False 
